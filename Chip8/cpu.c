//
//  cpu.c
//  Chip8
//
//  Created by Тимур on 12.01.16.
//  Copyright © 2016 Tikijian. All rights reserved.
//

#include "cpu.h"

void emulate_cycle()
{
    // Fetch Opcode
    opcode = memory[PC] << 8 | memory[PC + 1];
    
    // Decode Opcode
    switch (opcode & 0xF000)
    {
        case 0x0:
            if ((opcode & 0x0F00) > 0) {
                // 0NNN Calls RCA 1802 program at address NNN. Not necessary for most ROMs. Skip
                PC = PC + 2;
                break;
            }
            if ((opcode & 0x00FF) == 0xE0) {
                // 00E0 Clears the screen.
                memset(display, 0, 2048);
                PC = PC + 2;
                break;
            } else if ((opcode & 0x00FF) == 0xEE) {
                // 00EE TODO: Returns from a subroutine.
                break;
            }
        case 0x1000:
            // 1NNN Jumps to address NNN.
            PC = opcode & 0x0FFF;
            break;
        case 0x2000:
            // 2NNN Calls subroutine at NNN.
            break;
        case 0x3000:
            // 3XNN Skips the next instruction if VX equals NN.
            if ( V[ opcode & 0x0F00 ] == (opcode & 0X00FF) ) {
                PC = PC + 4;
            }
            break;
        case 0x4000:
            // 4XNN Skips the next instruction if VX doesn't equal NN.
            if ( V[ opcode & 0x0F00 ] != (opcode & 0X00FF) ) {
                PC = PC + 4;
            }
            break;
        case 0x5000:
            // 5XY0 Skips the next instruction if VX equals VY.
            if ( V[ opcode & 0x0F00 ] ==  V[ opcode & 0x00F0 ] ) {
                PC = PC + 4;
            }
            break;
        case 0xA000: // ANNN Sets I to the address NNN.
            I = opcode & 0x0FFF;
            PC = PC + 2;
            break;
            
    }
    // Decode Opcode
    // Execute Opcode
    
    // Update timers
    return;
}