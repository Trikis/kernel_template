#include "mem.h"

void memory_copy(const u8* source , u8* dest , int nbytes){
    for (int i =0 ; i < nbytes; ++i){
        dest[i] = source[i];
    }
}

void memory_set(u8* dest , u8 val , u32 len){
    int i = 0;
    for (; len !=0 ; --len){
        dest[i] = val;
        ++i;
    }
}