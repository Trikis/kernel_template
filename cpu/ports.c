#include "ports.h"

u8 port_byte_in(u16 port){
    u8 result;
    __asm__(
        "in al , dx"
        : "=a"(result)
        : "d"(port)
    );
    return result;
}

u16 port_word_in(u16 port){
    u16 result;
    __asm__(
        "in ax , dx"
        : "=a"(result)
        : "d"(port)
    );
    return result;
}

void port_byte_out(u16 port , u8 data){
    __asm__(
        "out dx , al"
        :
        : "d"(port) , "a"(data)
    );
}

void port_word_out(u16 port , u16 data){
    __asm__(
        "out dx , ax"
        :
        :"d"(port) , "a"(data)
    );
}