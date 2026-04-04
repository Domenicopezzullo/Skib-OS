BUILD_DIR = build
$(shell mkdir -p $(BUILD_DIR))

$(BUILD_DIR)/os.bin: $(BUILD_DIR)/bootloader.bin $(BUILD_DIR)/kernel.bin
	dd if=/dev/zero bs=512 count=2880 of=$(BUILD_DIR)/os.bin
	dd if=$(BUILD_DIR)/bootloader.bin of=$(BUILD_DIR)/os.bin conv=notrunc
	dd if=$(BUILD_DIR)/kernel.bin of=$(BUILD_DIR)/os.bin bs=512 seek=1 conv=notrunc

$(BUILD_DIR)/interrupts.o: drivers/interrupts.asm
	fasm drivers/interrupts.asm $(BUILD_DIR)/interrupts.o

$(BUILD_DIR)/bootloader.bin: boot.asm
	fasm boot.asm $(BUILD_DIR)/bootloader.bin

$(BUILD_DIR)/kernel.bin: $(BUILD_DIR)/kernel.o $(BUILD_DIR)/interrupts.o
	i686-elf-ld -T linker.ld -m elf_i386 $(BUILD_DIR)/interrupts.o $(BUILD_DIR)/kernel.o --oformat binary -o $(BUILD_DIR)/kernel.bin

$(BUILD_DIR)/kernel.o: kernel.c
	i686-elf-gcc -Wall -Wextra -m32 -ffreestanding -O0 -fno-stack-protector -c kernel.c -o $(BUILD_DIR)/kernel.o

clean:
	rm -rf $(BUILD_DIR)
