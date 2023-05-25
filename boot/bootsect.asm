[org 0x7c00]
KERNEL_OFFSET equ 0x1000
    mov [BOOT_DRIVE] , dl 
    mov bp , 0x9000
    mov sp , bp 

    mov bx , MSG_REAL_MODE
    call print16

    call load_kernel
    call switch_to_pm
    jmp $


load_kernel:
    mov bx , KERNEL_OFFSET
    mov dh , 31 ; 31 сектор считать с диска в память
    mov dl , [BOOT_DRIVE]
    call disk_load

    mov bx , MSG_LOAD_KERNEL
    call print16
    ret


%include "boot/print16.asm"
%include "boot/disk_load.asm"
%include "boot/gdt.asm"
%include "boot/switch_to_pm.asm"
%include "boot/print32.asm"

BEGIN_PM:
    mov ebx , MSG_PROT_MODE 
    call print32
    call KERNEL_OFFSET
    jmp $


BOOT_DRIVE db 0
MSG_REAL_MODE db "Started in 16bit Real Mode" , 0
MSG_PROT_MODE db "Landed in 32bit Protected Mode" , 0
MSG_LOAD_KERNEL db "Loaded kernel into memory" , 0
MSG_RETURNED_KERNEL db "Returned from kernel. Error?" , 0

times 510 - ($ - $$) db 0
dw 0xaa55