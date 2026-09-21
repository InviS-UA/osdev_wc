include config.mk

.PHONY: all floppy_image kernel boot lib clean always

all: floppy_image

#
# Floppy image
#
floppy_image: $(BUILD_DIR)/main_floppy.img

$(BUILD_DIR)/main_floppy.img: boot kernel lib
	dd if=/dev/zero of=$(BUILD_DIR)/main_floppy.img bs=512 count=2880
	mkfs.fat -F 12 -n "OSDEV" $(BUILD_DIR)/main_floppy.img
	dd if=$(BUILD_DIR)/boot.bin of=$(BUILD_DIR)/main_floppy.img conv=notrunc
	mcopy -i $(BUILD_DIR)/main_floppy.img $(BUILD_DIR)/kernel.sys "::kernel.sys"

#
# Bootloader
#
boot: $(BUILD_DIR)/boot.bin

$(BUILD_DIR)/boot.bin: always
	$(MAKE) -C $(SRC_DIR)/boot

#
# Kernel
#
kernel: lib $(BUILD_DIR)/kernel.sys

$(BUILD_DIR)/kernel.sys: always
	$(MAKE) -C $(SRC_DIR)/kernel

#
# Lib
#
lib: $(BUILD_DIR)/lib.lib

$(BUILD_DIR)/lib.lib: always
	$(MAKE) -C $(SRC_DIR)/lib

#
# Always
#
always:
	mkdir -p $(BUILD_DIR)

#
# Clean
#
clean:
	rm -rf $(BUILD_DIR)/*