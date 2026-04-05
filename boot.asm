org 0x7C00
use16

start:
    cli
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00
    mov [boot_drive], dl

    
    mov ax, 0x4F01
    mov cx, 0x4143
    mov di, 0x0600
    int 0x10
    cmp ax, 0x004F
    jne vbe_error

    
    mov eax, [0x0600 + 0x28]
    mov [0x0500], eax
    movzx eax, word [0x0600 + 0x10]
    mov [0x0504], eax


    
    mov ax, 0x0003
    int 0x10
    mov bx, 0x7000
    mov cx, 4
.print_loop:
    mov al, [bx]
    push cx
    push bx
    mov ah, 0x0E
    int 0x10
    pop bx
    pop cx
    inc bx
    loop .print_loop

    
    mov ax, 0x4F02
    mov bx, 0x4143
    or bx, 0x4000
    int 0x10
    cmp ax, 0x004F
    jne vbe_error

    
    mov bx, 0x1000      
    mov ah, 0x02
    mov al, 17          
    mov ch, 0           
    mov dh, 0           
    mov cl, 2           
    int 0x13
    jc disk_error

    
    
    mov bx, 0x3200      
    mov ah, 0x02
    mov al, 18          
    mov ch, 0           
    mov dh, 1           
    mov cl, 1           
    int 0x13
    jc disk_error

    
    
    mov bx, 0x5600      
    mov ah, 0x02
    mov al, 18          
    mov ch, 1           
    mov dh, 0           
    mov cl, 1           
    int 0x13
    jc disk_error

    
    in al, 0x92
    or al, 2
    out 0x92, al

    
    lgdt [gdt_descriptor]
    mov eax, cr0
    or eax, 1
    mov cr0, eax
    jmp 0x08:pmode

vbe_error:
    mov ax, 0x0003
    int 0x10
    mov ah, 0x0E
    mov al, 'E'
    int 0x10
    hlt
    jmp $

disk_error:
    mov ax, 0x0003
    int 0x10
    mov ah, 0x0E
    mov al, 'D'
    int 0x10
    hlt
    jmp $

use32
pmode:
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov esp, 0xC00000
    mov esi, [0x7000]
    jmp 0x08:0x1000

align 16
gdt:
    dq 0
    dw 0xFFFF, 0x0000
    db 0, 10011010b, 11001111b, 0
    dw 0xFFFF, 0x0000
    db 0, 10010010b, 11001111b, 0
gdt_end:

gdt_descriptor:
    dw gdt_end - gdt - 1
    dd gdt

boot_drive: db 0

times 510-($-$$) db 0
dw 0xAA55
