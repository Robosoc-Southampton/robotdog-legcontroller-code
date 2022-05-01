BUILD_DIR = ./build
SOURCE_DIR = ./src

TARGET_NAME = motor-controller

$(BUILD_DIR)/$(TARGET_NAME).elf: $(SOURCE_DIR)/*
	mkdir -p $(BUILD_DIR)
	avr-gcc -mmcu=atmega644p -DF_CPU=12000000 -Wall -O3 -o $(BUILD_DIR)/$(TARGET_NAME).elf $(SOURCE_DIR)/*.c

$(BUILD_DIR)/$(TARGET_NAME).hex: $(BUILD_DIR)/$(TARGET_NAME).elf
	avr-objcopy -O ihex $(BUILD_DIR)/$(TARGET_NAME).elf $(BUILD_DIR)/$(TARGET_NAME).hex

$(BUILD_DIR)/$(TARGET_NAME).asm: $(BUILD_DIR)/$(TARGET_NAME).elf
	avr-objdump -m avr5 -D $(BUILD_DIR)/$(TARGET_NAME).elf > $(BUILD_DIR)/$(TARGET_NAME).asm

.PHONY: install
install: $(BUILD_DIR)/$(TARGET_NAME).hex
	avrdude -c c232hm -p m644p -U flash:w:$(BUILD_DIR)/$(TARGET_NAME).hex

.PHONY: asm
asm: $(BUILD_DIR)/$(TARGET_NAME).asm
