ASMFLAGS += -f obj
CFLAGS16 += -I. -4 -d3 -s -wx -ms -zl -zq -fr

SRC_C=$(wildcard *.c */*.c)
SRC_ASM=$(wildcard *.asm */*.asm)
OBJ_C=$(patsubst %.c, $(BUILD_DIR)/lib/c/%.obj, $(SRC_C))
OBJ_ASM=$(patsubst %.asm, $(BUILD_DIR)/lib/asm/%.obj, $(SRC_ASM))

TARGET=$(BUILD_DIR)/lib/lib.lib

.PHONY: all

all: $(TARGET)

$(TARGET): $(OBJ_C) $(OBJ_ASM)
	@mkdir -p $(@D)
	$(AR16) -q -b -n $@ $^

$(BUILD_DIR)/lib/c/%.obj: %.c
	@mkdir -p $(@D)
	$(CC16) $(CFLAGS16) -fo=$@ $<

$(BUILD_DIR)/lib/asm/%.obj: %.asm
	@mkdir -p $(@D)
	$(ASM) $(ASMFLAGS) -o $@ $<