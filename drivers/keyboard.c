#include "keyboard.h"
#include "../cpu/ports.h"
#include "../cpu/isr.h"
#include "screen.h"
#include "../libc/string.h"
#include "../libc/function.h"
#include "../libc/mem.h"
#include "../kernel/kernel.h"


#define BACKSPACE 0x0e
#define ENTER 0x1c
#define LShift 0x2a
#define RShift 0x36

static char key_buffer[256];
int bShiftPressed = 0;

#define SC_MAX 0x39

const char* sc_name[] = {"ERROR" , "Esc" , "1" , "2" , "3" , "4" , "5" , "6" , "7" , "8" , "9" , "0" ,
"-" , "=" , "Backspace" , "Tab" , "Q" , "W" , "E" , "R" , "T" , "Y" , "U" , "I" , "O" , "P" , "[" ,
"]" , "Enter" , "Lctrl" , "A" , "S" , "D" , "F" , "G" , "H" , "J" , "K" , "L" , ";" , "'" , "`" , 
"LShift" , "\\" , "Z" , "X" , "C" , "V" , "B" , "N" , "M" , "," , "." , "/" , "RShift" , "Keypad *" , 
"LAlt" , "Spacebar"};

const char sc_ascii[] = {0xff , 0xff , '1' , '2' , '3' , '4' , '5' , '6' , '7' , '8' , '9' , '0' , 
'-' , '=' , 0xff , 0xff , 'q' , 'w' , 'e' , 'r' , 't' , 'y' , 'u' , 'i' , 'o' , 'p' , '[' , ']' , 
0xff , 0xff , 'a' , 's' , 'd' , 'f' , 'g' , 'h' , 'j' , 'k' , 'l' , ';' , '\'' , '`' , 0xff , '\\' ,
'z' , 'x' , 'c' , 'v' , 'b' , 'n' , 'm' , ',' , '.' , '/' , 0xff , 0xff , 0xff , ' '};

const char sc_shift_ascii[] ={0xff , 0xff , '!' , '@' , '#' , '$' , '%' , '^' , '&' , '*' , '(' , ')' ,
'_' , '+' , 0xff , 0xff , 'Q' , 'W' , 'E' , 'R' , 'T' , 'Y' , 'U' , 'I' , 'O' , 'P' , '{' , '}' , 0xff , 
0xff , 'A' , 'S' , 'D' , 'F' , 'G' , 'H' , 'J' , 'K' , 'L' , ':' , '"' , '~' , 0xff , '|' , 'Z' , 'X' ,
'C' , 'V' , 'B' , 'N' , 'M' , '<' , '>' , '?' , 0xff , 0xff , 0xff , ' '};

static void keyboard_callback(registers_t regs){
    u8 scancode = port_byte_in(0x60);
    if ((scancode == LShift + 0x80) || (scancode == RShift + 0x80)) bShiftPressed = 0;
    if ((scancode == LShift) || (scancode == RShift)) bShiftPressed = 1;
    if (scancode > SC_MAX) return;

    if (scancode == BACKSPACE && key_buffer[0] != 0){
        backspace(key_buffer);
        kprint_backspace();
    } 
    else if (scancode == ENTER){
        kprint("\n");
        user_input(key_buffer);
        key_buffer[0] = '\0';
    }
    else{
        unsigned char letter;
        if (bShiftPressed) letter = sc_shift_ascii[scancode];
        else letter = sc_ascii[scancode];
        if (letter == 0xff) return;
        char str[2] = {(char)letter , '\0'};
        append(key_buffer , letter);
        kprint(str);
    }

    UNUSED(regs);
}

void init_keyboard(){
    memory_set(key_buffer ,  0 , 256);
    register_interrupt_handler(IRQ1 , keyboard_callback);
}