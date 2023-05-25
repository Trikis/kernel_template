#include "screen.h"
#include "../cpu/ports.h"
#include "../libc/mem.h"

/* Private Kernel Api Functions*/

int get_offset(int row , int col){
    return 2 * (row * MAX_COLS + col);
}

int  get_offset_row(int offset){
    return offset / (2 * MAX_COLS);
}

int get_offset_col(int offset){
    return (offset % (2 * MAX_COLS)) / 2;
}

int get_cursor_offset(){
    port_byte_out(REG_SCREEN_CTRL , 14);
    int offset = port_byte_in(REG_SCREEN_DATA) << 8;
    port_byte_out(REG_SCREEN_CTRL , 15);
    offset += port_byte_in(REG_SCREEN_DATA);
    return offset * 2;
}

void set_cursor_offset(int offset){
    offset /= 2;
    port_byte_out(REG_SCREEN_CTRL , 14);
    port_byte_out(REG_SCREEN_DATA , (u8)(offset >> 8));
    port_byte_out(REG_SCREEN_CTRL , 15);
    port_byte_out(REG_SCREEN_DATA , (u8)(offset & 0xff));
}

int print_char(char  ch , int row , int col , char attr){
    u8* vidmem = (u8*)VIDEO_ADDRESS;
    if (!attr) attr = WHITE_ON_BLACK;

    if (col >= MAX_COLS || row >= MAX_ROWS){
        vidmem[2 * MAX_COLS * MAX_ROWS - 2] = 'E';
        vidmem[2 * MAX_COLS * MAX_ROWS - 1] = RED_ON_WHITE;
        return get_offset(row , col);
    }

    int offset;
    if (col >= 0 && row >= 0) offset = get_offset(row , col);
    else offset = get_cursor_offset();

    if(ch == '\n'){
        row = get_offset_row(offset);
        offset = get_offset(row + 1 , 0);
    }
    else if (ch == 0x08){ /*Backspace*/
        vidmem[offset] = ' ';
        vidmem[offset + 1] = attr;
    }
    else{
        vidmem[offset] = ch;
        vidmem[offset + 1] = attr;
        offset += 2;
    }

    if (offset >= MAX_ROWS * MAX_COLS * 2){
        for (int i = 1; i < MAX_ROWS ; ++i){
            memory_copy(
                (u8*)(get_offset(i , 0) + VIDEO_ADDRESS) , 
                (u8*)(get_offset(i - 1 , 0) + VIDEO_ADDRESS) , 
                MAX_COLS * 2
            );
        }
        char* last_line = (char*)(get_offset(MAX_ROWS - 1 , 0) + (u8*)VIDEO_ADDRESS);
        for (int i = 0 ; i < MAX_COLS * 2; ++i) last_line[i] = 0;

        offset -= 2 * MAX_COLS;
    }
    set_cursor_offset(offset);
    return offset;
}

/* Public Kernel Api Functions*/

void clear_screen(){
    int screen_size = MAX_COLS * MAX_ROWS;
    u8* screen = (u8*)VIDEO_ADDRESS;

    for (int i = 0 ; i < screen_size ; ++i){
        screen[i * 2] = ' ';
        screen[i * 2 + 1] = WHITE_ON_BLACK;
    }
    set_cursor_offset(0);
}

void kprint_at(const char* message , int row , int col){
    int offset;
    if (col >= 0 && row >= 0) offset = get_offset(row , col);
    else{
        offset = get_cursor_offset();
        row = get_offset_row(offset);
        col = get_offset_col(offset);
    }

    int i = 0;
    while (message[i] != 0){
        offset = print_char(message[i++] , row , col , WHITE_ON_BLACK);
        row = get_offset_row(offset);
        col = get_offset_col(offset);
    }
}

void kprint(const char* message){
    kprint_at(message , -1 , -1);
}

void kprint_backspace(){
    int offset = get_cursor_offset() - 2;
    int row = get_offset_row(offset);
    int col = get_offset_col(offset);
    print_char(0x08 , row , col , WHITE_ON_BLACK);
}