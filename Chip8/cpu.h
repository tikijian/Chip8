//
//  cpu.h
//  Chip8
//
//  Created by Тимур on 12.01.16.
//  Copyright © 2016 Tikijian. All rights reserved.
//
#pragma once

#ifndef cpu_h
#define cpu_h

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>
// =============== Internals ==========================

// 4KB RAM
extern unsigned char memory[4096];

// 16 General purpose registers
extern unsigned char V[16];

// Address register
extern unsigned short Ireg;

// Sound register
extern unsigned char sound_t;

// Delay timer
extern unsigned char delay_t;

// Program counter register
extern unsigned short PC;

// Stack
extern unsigned short stack[16];

// Stack pointer
extern unsigned short SP;

// Current opcode
extern unsigned short opcode;

// Display array
extern unsigned char display[2048]; // 2048 pixels

// Keyboard with 16 keys
extern unsigned char keyboard[16];

// ====================================================



// =============== Emulation ==========================

void emulate_cycle();

// ====================================================

#endif /* cpu_h */
