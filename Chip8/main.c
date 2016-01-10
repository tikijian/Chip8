//  Chip8 emulator project
//
//  Created by Tim Tikidjian.
//  Copyright © 2016 Tikijian. No rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <SDL2/SDL.h>

// =============== Chip-8 Internals ===================

// 4KB RAM
unsigned char memory[4096] = {0};

// 16 General purpose registers
unsigned char V[16] = {0};

// Address register
unsigned short I = 0;

// Sound register
unsigned char sound_t = 0;

// Delay timer
unsigned char delay_t = 0;

// Program counter register
unsigned short PC = 0;

// Stack
unsigned short stack[16] = {0};

// Stack pointer
unsigned short SP = 0;

// Current opcode
unsigned short opcode;

// Display array
unsigned char display[2048]; // 2048 pixels

// Keyboard with 16 keys
unsigned char keyboard[16] = {0};

// ====================================================


// =============== General purpose functions ==========

// Initialize computer
void initialize();

// Reset computer
void reset();

// Font-to-memory routine
void load_fontset();

// ====================================================



int main(int argc, const char * argv[]) {
    
    initialize();
    
    for (int i = 0; i < 80; i++) {
        printf("%0#2x \n", memory[i]);
    }
    
    return 0;
}

void initialize()
{
    reset();
    load_fontset();
    return;
}

void reset()
{
    PC      = 0x200; // start program at address 0x200
    opcode  = 0;
    I       = 0;
    SP      = 0;
    
    // reset display
    // reset keyboard
    // reset everything
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


