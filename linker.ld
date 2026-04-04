ENTRY(_start)

SECTIONS
{
    . = 0x1000;

    .text : ALIGN(4) {
        *(.text.entry)    
        *(.text)          
        *(.text.*)        
    }

    
    .rodata : ALIGN(4) {
        *(.rodata)
        *(.rodata.*)
    }

    
    .data : ALIGN(4) {
        *(.data)
        *(.data.*)
    }

    
    .bss : ALIGN(4) {
        __bss_start = .;
        *(.bss)
        *(.bss.*)
        *(COMMON)
        __bss_end = .;
    }

    
    __bss_size = __bss_end - __bss_start;

    
    __kernel_end = .;
    
    
    /DISCARD/ : {
        *(.comment)
        *(.eh_frame)
        *(.note.*)
    }
}
