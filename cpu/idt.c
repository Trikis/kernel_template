#include "idt.h"


void set_idt_gate(int n , u32 handler){
    idt[n].low_offset = low_16(handler);
    idt[n].kernelcode_sel = KERNEL_CS;
    idt[n].always0 = 0;
    idt[n].flags = 0x8e;
    idt[n].high_offset = high_16(handler);
}

void set_idt(){
    idt_reg.base = (u32)&idt;
    idt_reg.limit = IDT_ENTRIES * sizeof(idt_gate_t) - 1;

    __asm__(
        "lidt %0"
        :
        : "m"(idt_reg)
    );
}