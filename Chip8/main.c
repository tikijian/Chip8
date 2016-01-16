//  Chip8 emulator project
//
//  Created by Tim Tikijian.
//  Copyright © 2016 Tikijian. No rights reserved.
//



#include "cpu.h"
// =============== Chip-8 Internals ===================

unsigned char memory[4096] = {0};
unsigned char V[16] = {0};
unsigned char display[2048] = {0}; // 2048 pixels
unsigned char keyboard[16] = {0};
unsigned char sound_t = 0;
unsigned char delay_t = 0;
unsigned short Ireg = 0;
unsigned short PC = 0;
unsigned short stack[16] = {0};
unsigned short SP = 0;
unsigned short opcode;
bool waiting_key = false;

// =============== General purpose functions ==========

// Initialize computer
void initialize();

// Reset computer
void reset();

// Font-to-memory routine
void load_fontset();

// Loads rom into memory
bool load_program(const char *filename);

// ====================================================


int main(int argc, const char * argv[]) {
    
    initialize();
    
    // Emulation loop
//    for(;;)
//    {
        // Emulate one cycle
        
        
        // If the draw flag is set, update the screen
       
            //drawGraphics();
        
        // Store key press state (Press and Release)
        
//    }
    
    getchar();
    
    printf("Bye!\n");
    return 0;
}

void initialize()
{
    printf("Initializing...\n");
    reset();
    load_fontset();
    return;
}

void reset()
{
    printf("Starting CPU reset sequence...\n");
    srand((unsigned int) time(NULL));
    PC      = 0x200; // start program at address 0x200
    opcode  = 0;
    Ireg    = 0;
    SP      = 0;
    waiting_key = false;
    
    memset(memory, 0, 4096);
    memset(V, 0, 16); //reset registers
    memset(keyboard, 0, 16); // reset keyboard state
    memset(display, 0, 2048); // clear display data
    
    if (! load_program("roms/IBM.ch8"))
        exit(1);
}

bool load_program(const char *filename)
{
    FILE *pFile;
    long lSize;

    pFile = fopen(filename, "rb");
    if (pFile == NULL) {
        printf("Unable to open file \"%s\"\n", filename);
        return false;
    }
    
    // obtain file size:
    fseek (pFile , 0 , SEEK_END);
    lSize = ftell (pFile);
    rewind (pFile);
    
    // copy file data into the memory, from address 0x200:
    fread (&memory[0x200], 1, lSize, pFile);
    
    fclose (pFile);

    return true;
}

void load_fontset()
{
    unsigned char fontset[80] =
    {
        0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
        0x20, 0x60, 0x20, 0x20, 0x70, // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
        0xF0, 0x10, 0x20, 0x40, 0x40, // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90, // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
        0xF0, 0x80, 0x80, 0x80, 0xF0, // C
        0xE0, 0x90, 0x90, 0x90, 0xE0, // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
        0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    };
    
    memcpy(memory, fontset, 80);
    
    return;
}


