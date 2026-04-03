# Skib-OS
32-bit operating system built from scratch 

# Dependencies
1. **i686-elf-gcc** and **i686-elf-ld**
2. **fasm**

# Building instructions
```sh
make
```
and then you can just run with ```sh qemu-system-i386 -drive format=raw,file=build/os.bin ```
