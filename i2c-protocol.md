# Motor controller I2C protocol

## Register map

| Address   | Register name     | R/W | Description                                       |
| ---------:| ----------------- |:---:| ------------------------------------------------- |
|         0 | VERSION\_ID       |  R  | Protocol version ID                               |
|         1 | ENABLE            | R/W | Enable motors                                     |
|         2 | CALIBRATE         | R/W | Calibrate motors                                  |
|   3 -  15 |                   |     | \<reserved\>                                      |
|        16 | PRECISION\_0\_L   |  R  | Number of divisions in one revolution (low byte)  |
|        17 | PRECISION\_0\_H   |  R  | Number of divisions in one revolution (high byte) |
|        18 | PRECISION\_1\_L   |  R  | ""                                                |
|        19 | PRECISION\_1\_H   |  R  | ""                                                |
|        20 | PRECISION\_2\_L   |  R  | ""                                                |
|        21 | PRECISION\_2\_H   |  R  | ""                                                |
|  22 -  31 |                   |     | \<reserved\>                                      |
|        32 | ACTUAL\_POS\_0\_L |  R  | Actual motor position (low byte)                  |
|        33 | ACTUAL\_POS\_0\_H |  R  | Actual motor position (high byte)                 |
|        34 | ACTUAL\_POS\_1\_L |  R  | ""                                                |
|        35 | ACTUAL\_POS\_1\_H |  R  | ""                                                |
|        36 | ACTUAL\_POS\_2\_L |  R  | ""                                                |
|        37 | ACTUAL\_POS\_2\_H |  R  | ""                                                |
|  38 -  47 |                   |     | \<reserved\>                                      |
|        48 | TARGET\_POS\_0\_L | R/W | Target motor position (low byte)                  |
|        49 | TARGET\_POS\_0\_H | R/W | Target motor position (high byte)                 |
|        50 | TARGET\_POS\_1\_L | R/W | ""                                                |
|        51 | TARGET\_POS\_1\_H | R/W | ""                                                |
|        52 | TARGET\_POS\_2\_L | R/W | ""                                                |
|        53 | TARGET\_POS\_2\_H | R/W | ""                                                |
|  54 -  63 |                   |     | \<reserved\>                                      |
|  64 - 255 |                   |     | \<reserved\>                                      |


### VERSION\_ID

Protocol version register.
- Contains the protocol version supported by the motor controller.

### ENABLE

Motor enable register.
- Each motor can be enabled or disabled by setting or clearing the corresponding bit in this register.
- While a motor is disabled, no PWM signals will be sent to it.

|  7  |  6  |  5  |  4  |  3  |  2  |  1  |  0  |
|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|
|     |     |     |     |     | EN2 | EN1 | EN0 |
|     |     |     |     |     | R/W | R/W | R/W |

### CALIBRATE

Motor calibrate register.
- Writing a `1` to a bit in this register will calibrate the corresponding motor, reseting it's position to zero.
- The bit will be cleared automatically once the motor is calibrated.
- Writing a `0` to any bit has no effect.

|  7  |  6  |  5  |  4  |  3  |   2  |   1  |   0  |
|:---:|:---:|:---:|:---:|:---:|:----:|:----:|:----:|
|     |     |     |     |     | CAL2 | CAL1 | CAL0 |
|     |     |     |     |     |  R/W |  R/W |  R/W |

### PRECISION\_n

Motor precision register.
- Specifies the number of divisions in one revolution of the motor.
- This is a 16-bit unsigned register. See `Accessing 16-bit registers` for more info.

### ACTUAL\_POS\_n

Actual position register.
- Read this register to get the actual position of the motor.
- The precision is given in the `PRECISION\_n` register.
- This is a 16-bit signed register. See `Accessing 16-bit registers` for more info.

### TARGET\_POS\_n

Target position register.
- Write to this register to set the target position of the motor.
- The precision is given in the `PRECISION\_n` register.
- This is a 16-bit signed register. See `Acessing 16-bit registers` for more info.


## Acessing 16-bit registers

16-bit registers are made up of two 8-bit registers, `REGISTER\_L` and `REGITER\_H` (low and high bytes respectively).
Accessing 16-bit registers takes two separate accesses of these underlying 8-bit registers. A temporary 8-bit register stores the first byte to keep the two bytes synchronised.
Reading the low byte also copies the high byte into the temporary register at the same time. Reading the high byte reads from this temporary register.
Writing to the low byte stores the value in the temporary register. Writing to the high byte then writes both bytes at the same time.
When accessing a 16-bit register, the low byte must be accessed first, then the high byte.
Note that not all 16-bit accesses use the temprary register. It is only used in cases where the two bytes must be synchronised.
