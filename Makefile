BUILD_DIR = build
$(shell mkdir -p $(BUILD_DIR))

.PHONY: all clean

all: $(BUILD_DIR)/os.bin

$(BUILD_DIR)/os.bin: $(BUILD_DIR)/bootloader.bin $(BUILD_DIR)/kernel.bin
	cat $(BUILD_DIR)/bootloader.bin $(BUILD_DIR)/kernel.bin > $(BUILD_DIR)/os.bin
	dd if=/dev/zero bs=512 count=2880 >> $(BUILD_DIR)/os.bin

$(BUILD_DIR)/bootloader.bin: boot.asm
	fasm boot.asm $(BUILD_DIR)/bootloader.bin

$(BUILD_DIR)/kernel.bin: $(BUILD_DIR)/kernel.o
	i686-elf-ld -T linker.ld -m elf_i386 $(BUILD_DIR)/kernel.o --oformat binary -o $(BUILD_DIR)/kernel.bin

$(BUILD_DIR)/kernel.o: kernel.c
	i686-elf-gcc -Wall -Wextra -m32 -ffreestanding -O0 -fno-stack-protector -c kernel.c -o $(BUILD_DIR)/kernel.o

clean:
	rm -rf $(BUILD_DIR)
