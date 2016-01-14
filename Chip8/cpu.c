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
    
    // Decode & execute Opcode
    switch (opcode & 0xF000)
    {
        case 0x0:
            if ((opcode & 0x0F00) > 0) {
                // 0NNN Calls RCA 1802 program at address NNN. Not necessary for most ROMs. Skip
                PC += 2;
                break;
            } else if ((opcode & 0x00FF) == 0xE0) {
                // 00E0 Clears the screen.
                memset(display, 0, 2048);
                PC += 2;
                break;
            } else if ((opcode & 0x00FF) == 0xEE) {
                // 00EE Returns from a subroutine.
                // The interpreter sets the program counter to the address at the top of the stack, then subtracts 1 from the stack pointer.
                PC = stack[SP];
                SP -= 1;
                break;
            } else {
                // should not happen
                printf ("Unknown opcode: 0x%X\n", opcode);
                PC += 2;
                break;
            }
        case 0x1000:
            // 1NNN Jumps to address NNN.
            PC = opcode & 0x0FFF;
            break;
        case 0x2000:
            // 2NNN Calls subroutine at NNN.
            // The interpreter increments the stack pointer, then puts the current PC on the top of the stack.
            // The PC is then set to nnn.
            SP += 1;
            stack[SP] = PC;
            PC = opcode & 0x0FFF;
            break;
        case 0x3000:
            // 3XNN Skips the next instruction if VX equals NN.
            if ( V[ opcode & 0x0F00 ] == (opcode & 0X00FF) )
                PC += 4;
            else
                PC += 2;
            break;
        case 0x4000:
            // 4XNN Skips the next instruction if VX doesn't equal NN.
            if ( V[ opcode & 0x0F00 ] != (opcode & 0X00FF) )
                PC += 4;
            else
                PC += 2;
            break;
        case 0x5000:
            // 5XY0 Skips the next instruction if VX equals VY.
            if ( V[ opcode & 0x0F00 ] ==  V[ opcode & 0x00F0 ] )
                PC += 4;
            else
                PC += 2;
            break;
        case 0x6000:
            // 6XNN Sets VX to NN.
            V[ opcode & 0x0F00 ] = (opcode & 0x00FF);
            PC += 2;
            break;
        case 0X7000:
            // 7XNN Adds NN to VX.
            V[ opcode & 0x0F00 ] += (opcode & 0x00FF);
            PC += 2;
            break;
        case 0x8000:
            // TODO
            PC += 2;
            break;
        case 0x9000:
            // 9XY0 Skips the next instruction if VX doesn't equal VY.
            if ( V[ opcode & 0x0F00 ] != V[ opcode & 0x00F0 ] )
                PC += 4;
            else
                PC += 2;
            break;
        case 0xA000:
            // ANNN Sets I to the address NNN.
            Ireg = opcode & 0x0FFF;
            PC += 2;
            break;
        case 0xB000:
            // BNNN Jump to location nnn + V0.
            // The program counter is set to nnn plus the value of V0.
            PC = (opcode & 0x0FFF) + V[0];
            break;
        case 0xC000:
            // CXNN Set VX = random byte AND NN
            // The interpreter generates a random number from 0 to 255, which is then ANDed with the value kk.
            // The results are stored in Vx.
            srand((unsigned int) time(NULL));
            V[ opcode & 0x0F00 ] = (rand() % 255) & (opcode & 0x00FF);
            PC += 2;
            break;
        case 0xD000:
            /// DXYN TODO
            PC += 2;
            break;
        case 0xE000:
            switch (opcode & 0x00FF) {
                case 0x009E:
                    // EX9E Skips the next instruction if the key stored in VX is pressed.
                    if ( keyboard[ V[opcode & 0x0F00] ] )
                        PC += 4;
                    else
                        PC += 2;
                    break;
                case 0x00A1:
                    // EXA1 Skips the next instruction if the key stored in VX is NOT pressed.
                    if (! keyboard[ V[opcode & 0x0F00] ] )
                        PC += 4;
                    else
                        PC += 2;
                    break;
                default:
                    printf ("Unknown opcode: 0x%X\n", opcode);
                    PC += 2;
                    break;
            }
        case 0xF000:
            switch (opcode & 0x00FF) {
                case 0x0007:
                    // FX07 Sets VX to the value of the delay timer.
                    V[ opcode & 0x0F00 ] = delay_t;
                    PC += 2;
                    break;
                case 0x000A:
                    // FX0A Wait for a key press, store the value of the key in Vx.
                    //All execution stops until a key is pressed, then the value of that key is stored in Vx.
                    // TODO:
                    PC += 2;
                    break;
                case 0x0015:
                    // FX15 Sets the delay timer to VX.
                    delay_t = V[ opcode & 0x0F00 ];
                    PC += 2;
                    break;
                case 0x0018:
                    // FX18 Sets the sound timer to VX.
                    sound_t = V[ opcode & 0x0F00 ];
                    PC += 2;
                    break;
                case 0x001E:
                    // FX1E Adds VX to I.
                    
                    // care about addition overflow
                    if (Ireg + V[ opcode & 0x0F00 ] > 0xFFF)
                        V[0xF] = 1;
                    else
                        V[0xF] = 0;
                    
                    Ireg += V[ opcode & 0x0F00 ];
                    PC += 2;
                    break;
                case 0x0029:
                    // FX29 Sets I to the location of the sprite for the character in VX.
                    Ireg = V[ opcode & 0x0F00 ] * 5; // each character sprite are 5 bytes long
                    PC += 2;
                    break;
                case 0x0033:
                    // Stores the Binary-coded decimal representation of VX, with the most significant of three digits at the address in I,
                    //the middle digit at I plus 1, and the least significant digit at I plus 2.
                    // (In other words, take the decimal representation of VX, place the hundreds digit in memory at location in I, the tens digit at location I+1, and the ones digit at location I+2.)
                    // TODO:
                    PC += 2;
                    break;
                case 0x0055:
                    // FX55 Stores V0 to VX in memory starting at address I
                    for (unsigned short i = 0; i <= (opcode & 0x0F00); i++)
                    {
                        memory[Ireg + i] = V[i];
                    }
                    PC += 2;
                    break;
                case 0x0065:
                    // FX65 Read registers V0 through Vx from memory starting at location I.
                    // The interpreter reads values from memory starting at location I into registers V0 through Vx.
                    for (unsigned short i = 0; i <= (opcode & 0x0F00); i++)
                    {
                        V[i] = memory[Ireg + i];
                    }
                    PC += 2;
                    break;
                default:
                    printf ("Unknown opcode: 0x%X\n", opcode);
                    PC += 2;
                    break;
            }

    }
    
    // Update timers
    return;
}