#include "../cpu/isr.h"
#include "../drivers/screen.h"
#include "kernel.h"
#include "../libc/string.h"

void main(){
    clear_screen();
    isr_install();
    irq_install();
    kprint("Type END to halt the CPU\n> ");
}

void user_input(char* input){
    if (strcmp(input , "END") == 0){
        kprint("Stopping the CPU. Bye!\n");
        __asm__(
            "hlt"
        );
    }
    kprint("You said: ");
    kprint(input);
    kprint("\n> ");
}