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

// =============== Internals ==========================

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



// =============== Emulation ==========================

void emulate_cycle();

// ====================================================

#endif /* cpu_h */
