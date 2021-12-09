BUILD_DIR = ./build
SOURCE_DIR = ./src

$(BUILD_DIR)/main.hex: $(BUILD_DIR)/main.elf
	avr-objcopy -O ihex $(BUILD_DIR)/main.elf $(BUILD_DIR)/main.hex

$(BUILD_DIR)/main.elf: $(SOURCE_DIR) | $(BUILD_DIR)
	avr-gcc -mmcu=atmega644p -DF_CPU=12000000 -Wall -Os -o $(BUILD_DIR)/main.elf $(SOURCE_DIR)/*.c

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/main.asm: $(BUILD_DIR)/main.elf
	avr-objdump -m avr5 -D $(BUILD_DIR)/main.elf > $(BUILD_DIR)/main.asm

.PHONY: install
install: $(BUILD_DIR)/main.hex
	avrdude -c usbasp -p m644p -U flash:w:$(BUILD_DIR)/main.hex

.PHONY: disasm
disasm: $(BUILD_DIR)/main.asm
