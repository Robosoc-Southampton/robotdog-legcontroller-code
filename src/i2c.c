#include "i2c.h"

#include "motors.h"

#include <avr/io.h>
#include <avr/interrupt.h>

#include <stdint.h>
#include <stdbool.h>

#define I2C_VERSION_ID 0

#define REG_VERSION_ID 0
#define REG_ENABLE     1
#define REG_CALIBRATE  2

#define REG_PRECISION  1
#define REG_ACTUAL_POS 2
#define REG_TARGET_POS 3

/**
 * Temporary variable to store a byte for a 16-bit access.
 */
static uint8_t temp_byte;

/**
 * Gets the device's slave address from the switches.
 */
static int8_t get_slave_address()
{
    static const uint8_t addresses[4] = { 4, 5, 6, 7 };
    
    // Set PA4:3 as inputs with pullups:
    DDRA &= ~(1 << DDA4) & ~(1 << DDA3);
    PORTA |= (1 << PORTA4) | (1 << PORTA3);
    
    uint8_t state = (PINA >> PINA3) & 0x03;
    
    return addresses[state];
}

static void write_register(uint8_t addr, uint8_t data)
{
    if (addr == REG_ENABLE)
    {
        motor_enabled_flags = data;
        return;
    }
    if (addr == REG_CALIBRATE)
    {
        motor_calibrate_flags |= data;
        return;
    }
    
    uint8_t reg = addr / 16;
    uint8_t port = addr / 2 % 8;
    bool is_high_byte = addr % 2;
    
    if (port >= MOTOR_PORTS)
        return;
    
    switch (reg)
    {
        case REG_TARGET_POS:
            if (!is_high_byte) temp_byte = data;
            else               motors[port].target_pos = (data << 8) | temp_byte;
    }
}

static uint8_t read_register(uint8_t addr)
{
    if (addr == REG_VERSION_ID) return I2C_VERSION_ID;
    if (addr == REG_ENABLE) return motor_enabled_flags;
    if (addr == REG_CALIBRATE) return motor_calibrate_flags;
    
    uint8_t reg = addr / 16; // r25
    uint8_t port = addr / 2 % 8; // r18
    bool is_high_byte = addr % 2; // r24
    
    if (port >= MOTOR_PORTS)
        return -1;
    
    switch (reg)
    {
        case REG_PRECISION:
            if (!is_high_byte) return (uint8_t) (motors[port].precision & 0xFF);
            else               return (uint8_t) (motors[port].precision >> 8);
        case REG_ACTUAL_POS:
            if (!is_high_byte)
            {
                int16_t position = motors[port].actual_pos;
                temp_byte = (uint8_t) (position >> 8);
                return (uint8_t) (position & 0xFF);
            }
            else return temp_byte;
        case REG_TARGET_POS:
            if (!is_high_byte) return (uint8_t) (motors[port].target_pos & 0xFF);
            else               return (uint8_t) (motors[port].target_pos >> 8);
    }
    
    return -1;
}

void init_i2c()
{
    cli();
    
    TWCR = (1 << TWEA)  // enable acknowledge bit
         | (1 << TWEN)  // enable TWI
         | (1 << TWIE); // enable interrupt
    TWAR = get_slave_address() << TWA0; // get address from the switches
    TWAMR = 0;          // no addess masking
    
    sei();
}

ISR(TWI_vect)
{
    static uint8_t register_address;
    static bool register_address_valid;
    
    // In order to not block other interrupts while processing
    // the data, re-enable global interrupts. We have to disable
    // the TWI interrupt because its interrupt flag is still
    // set which would keep firing this interrupt routine.
    // Also when writing to the TWCR register we need to make
    // sure not to write a `1` to TWINT.
    TWCR &= ~(1 << TWIE) & ~(1 << TWINT);
    sei();
    
    uint8_t twi_status = TWSR & 0xF8;
    
    switch (twi_status)
    {
        /*** Slave Receiver Mode ***/
        case 0x60: // `slave address + write` received
            // Clear the interrupt flag:
            TWCR |= (1 << TWINT);
            
            // `slave address + write` always starts a new transaction and will be
            // followed by the register address. Therefore invalidate the
            // address so we know the next data byte is the address.
            register_address_valid = false;
            break;
        case 0x80: // data received (ACK returned)
            if (!register_address_valid)
            {
                // First data byte after `slave address + write`
                // will be the register address.
                register_address = TWDR;
                
                // Clear the interrupt flag.
                TWCR |= (1 << TWINT);
                
                register_address_valid = true;
            }
            else
            {
                uint8_t data = TWDR;
                TWCR |= (1 << TWINT); // clear the interrupt flag to prevent blocking the TWI bus
                write_register(register_address, data);
                register_address++; // increment address for sequential data access
            }
            break;
        
        /*** Slave Transmitter Mode ***/
        case 0xA8: // `address + read` received
        case 0xB8: // data transmitted, ACK received
            TWDR = read_register(register_address);
            TWCR |= (1 << TWINT); // clear the interrupt flag
            register_address++; // increment address for sequential data access
            break;
        
        default:
            TWCR |= (1 << TWINT); // clear the interrupt flag
    }
    
    // Re-enable the TWI interrupt. Disable global interrupts
    // to ensure this interrupt routine is not fired again before
    // this instance returns. Global interrupts will be enabled
    // again by `reti`.
    cli();
    TWCR |= (1 << TWIE);
}
