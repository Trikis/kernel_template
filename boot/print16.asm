;bx - address of string
print16:
    pusha
    mov ah , 0x0e
print16_loop:
    mov al , [bx]
    cmp al , 0
    je print16_done
    int 0x10

    add bx , 1
    jmp print16_loop

print16_done:
    mov al , 0x0a
    int 0x10
    mov al , 0x0d
    int 0x10
    popa
    ret
