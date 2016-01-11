// =============== Chip-8 Internals ===================

#ifndef cpu_h
#define cpu_h


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
unsigned char display[2048] = {0}; // 2048 pixels

// Keyboard with 16 keys
unsigned char keyboard[16] = {0};

// ====================================================

#endif /* cpu_h */
