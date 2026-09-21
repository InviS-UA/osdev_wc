.PHONY: all boot

all: boot

boot: $(BUILD_DIR)/boot.bin

$(BUILD_DIR)/boot.bin:
	$(ASM) boot.asm -f bin -o $(BUILD_DIR)/boot.bin