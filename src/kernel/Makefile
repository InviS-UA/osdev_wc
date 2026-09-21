ASMFLAGS += -f obj
CFLAGS16 += -I$(SRC_DIR)/lib -I. -3 -d3 -s -wx -ms -zl -zq -za99 -fr

SRC_C=$(wildcard *.c */*.c)
SRC_ASM=$(wildcard *.asm */*.asm)
OBJ_C=$(patsubst %.c, $(BUILD_DIR)/kernel/c/%.obj, $(SRC_C))
OBJ_ASM=$(patsubst %.asm, $(BUILD_DIR)/kernel/asm/%.obj, $(SRC_ASM))

.PHONY: all kernel

all: kernel

kernel: $(BUILD_DIR)/kernel.sys

$(BUILD_DIR)/kernel.sys: $(OBJ_ASM) $(OBJ_C)
	$(LD16) NAME $@ FILE \{ $^ \} OPTION MAP=$(BUILD_DIR)/kernel.map @linker.lnk LIB $(BUILD_DIR)/lib/lib.lib

$(BUILD_DIR)/kernel/c/%.obj: %.c
	@mkdir -p $(@D)
	$(CC16) $(CFLAGS16) -fo=$@ $<

$(BUILD_DIR)/kernel/asm/%.obj: %.asm
	@mkdir -p $(@D)
	$(ASM) $(ASMFLAGS) -o $@ $<