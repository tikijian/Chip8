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
    printf("0x%04x\n", opcode);
    
    // Decode & execute Opcode
    switch (opcode & 0xF000)
    {
        case 0x0:
            if (get_X(opcode) > 0) {
                // 0NNN Calls RCA 1802 program at address NNN. Not necessary for most ROMs. Skip
                PC += 2;
            } else if ((opcode & 0x00FF) == 0xE0) {
                // 00E0 Clears the screen.
                memset(display, 0, 2048);
                PC += 2;
            } else if ((opcode & 0x00FF) == 0xEE) {
                // 00EE Returns from a subroutine.
                // The interpreter sets the program counter to the address at the top of the stack, then subtracts 1 from the stack pointer.
                PC = stack[SP];
                SP -= 1;
            } else {
                // should not happen
                printf ("Unknown opcode: 0x%04x\n", opcode);
                PC += 2;
            }
            break;
        case 0x1000:
            // 1NNN Jumps to address NNN.
            PC = opcode & 0x0FFF;
            break;
        case 0x2000:
            // 2NNN Calls subroutine at NNN.
            // The interpreter increments the stack pointer, then puts the current PC on the top of the stack.
            // The PC is then set to nnn.
            stack[SP] = PC;
            SP += 1;
            PC = opcode & 0x0FFF;
            break;
        case 0x3000:
            // 3XNN Skips the next instruction if VX equals NN.
            if ( V[ get_X(opcode) ] == (opcode & 0X00FF) )
                PC += 4;
            else
                PC += 2;
            break;
        case 0x4000:
            // 4XNN Skips the next instruction if VX doesn't equal NN.
            if ( V[ get_X(opcode) ] != (opcode & 0X00FF) )
                PC += 4;
            else
                PC += 2;
            break;
        case 0x5000:
            // 5XY0 Skips the next instruction if VX equals VY.
            if ( V[ get_X(opcode) ] ==  V[ get_Y(opcode) ] )
                PC += 4;
            else
                PC += 2;
            break;
        case 0x6000:
            // 6XNN Sets VX to NN.
            V[ get_X(opcode) ] = (opcode & 0x00FF);
            PC += 2;
            break;
        case 0X7000:
            // 7XNN Adds NN to VX.
            V[ get_X(opcode) ] += (opcode & 0x00FF);
            PC += 2;
            break;
        case 0x8000:
            switch (opcode & 0x000F) {
                case 0x0000:
                    // 8XY0 Set Vx = Vy.
                    V[ get_X(opcode) ] = V[ get_Y(opcode)];
                    PC += 2;
                    break;
                case 0x0001:
                    // 8XY1 Set Vx = Vx OR Vy.
                    // Performs a bitwise OR on the values of Vx and Vy, then stores the result in Vx.
                    V[ get_X(opcode) ] |= V[ get_Y(opcode) ];
                    PC += 2;
                    break;
                case 0x0002:
                    // 8XY2 Set Vx = Vx AND Vy.
                    V[ get_X(opcode) ] &= V[ get_Y(opcode) ];
                    PC += 2;
                    break;
                case 0x0003:
                    // 8XY3 Set Vx = Vx XOR Vy.
                    V[ get_X(opcode) ] ^= V[ get_Y(opcode) ];
                    PC += 2;
                    break;
                case 0x0004:
                    // 8XY4 Set Vx = Vx + Vy, set VF = carry.
                    // The values of Vx and Vy are added together.
                    // If the result is greater than 8 bits (i.e., > 255,) VF is set to 1, otherwise 0.
                    if ((V[ get_X(opcode) ] += V[ get_Y(opcode) ]) > 0xFF)
                        V[0xF] = 1;
                    else
                        V[0xF] = 0;
                    
                    PC += 2;
                    break;
                case 0x0005:
                    // 8XY5 Set Vx = Vx - Vy, set VF = NOT borrow.
                    // If Vx > Vy, then VF is set to 1, otherwise 0.
                    // Then Vy is subtracted from Vx, and the results stored in Vx.
                    if (V[ get_X(opcode) ] > V[ get_Y(opcode) ])
                        V[0xF] = 1;
                    else
                        V[0xF] = 0;
                    
                    V[ get_X(opcode) ] -= V[ get_Y(opcode) ];
                    PC += 2;
                    break;
                case 0x0006:
                    // 8X06 Set Vx = Vx SHR 1.
                    // If the least-significant bit of Vx is 1, then VF is set to 1, otherwise 0. Then Vx is divided by 2.
                    V[0xF] = (V[ get_X(opcode)] & 1) ? 1 : 0;
                    V[ get_X(opcode)] >>= 1;
                    PC += 2;
                    break;
                case 0x0007:
                    // 8XY7 Sets VX to VY minus VX. VF is set to 0 when there's a borrow, and 1 when there isn't.
                    V[0xF] = V[ get_Y(opcode) ] > V[ get_X(opcode) ];
                    V[ get_X(opcode) ] = V[ get_Y(opcode) ] - V[ get_X(opcode) ];
                    
                    PC += 2;
                    break;
                case 0x000E:
                    // 8X0E Set Vx = Vx SHL 1.
                    // If the most-significant bit of Vx is 1, then VF is set to 1, otherwise to 0. Then Vx is multiplied by 2.
                    V[0xF] = ((V[ get_X(opcode)] & 0x80) != 0) ;
                    V[ get_X(opcode) ] <<= 1;
                    PC += 2;
                    break;
                default:
                    printf ("Unknown opcode: 0x%04x\n", opcode);
                    PC += 2;
                    break;
            }
            break;
        case 0x9000:
            // 9XY0 Skips the next instruction if VX doesn't equal VY.
            if ( V[ get_X(opcode) ] != V[ get_Y(opcode) ] )
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
            V[ get_X(opcode) ] = (rand() % 255) & (opcode & 0x00FF);
            PC += 2;
            break;
        case 0xD000:
            /// DXYN TODO
            DXYN(opcode);
            PC += 2;
            break;
        case 0xE000:
            switch (opcode & 0x00FF) {
                case 0x009E:
                    // EX9E Skips the next instruction if the key stored in VX is pressed.
                    if ( keyboard[ V[get_X(opcode)] ] )
                        PC += 4;
                    else
                        PC += 2;
                    break;
                case 0x00A1:
                    // EXA1 Skips the next instruction if the key stored in VX is NOT pressed.
                    if (! keyboard[ V[get_X(opcode)] ] )
                        PC += 4;
                    else
                        PC += 2;
                    break;
                default:
                    printf ("Unknown opcode: 0x%04x\n", opcode);
                    PC += 2;
                    break;
            }
            
            break;
        case 0xF000:
            switch (opcode & 0x00FF) {
                case 0x0007:
                    // FX07 Sets VX to the value of the delay timer.
                    V[ get_X(opcode) ] = delay_t;
                    PC += 2;
                    break;
                case 0x000A:
                    // FX0A Wait for a key press, store the value of the key in Vx.
                    //All execution stops until a key is pressed, then the value of that key is stored in Vx.
                    // TODO:
                    waiting_key = true;
                    PC += 2;
                    break;
                case 0x0015:
                    // FX15 Sets the delay timer to VX.
                    delay_t = V[ get_X(opcode) ];
                    PC += 2;
                    break;
                case 0x0018:
                    // FX18 Sets the sound timer to VX.
                    sound_t = V[ get_X(opcode) ];
                    PC += 2;
                    break;
                case 0x001E:
                    // FX1E Adds VX to I.
                    
                    // care about addition overflow
                    if (Ireg + V[ get_X(opcode) ] > 0xFFF)
                        V[0xF] = 1;
                    else
                        V[0xF] = 0;
                    
                    Ireg += V[ get_X(opcode) ];
                    PC += 2;
                    break;
                case 0x0029:
                    // FX29 Sets I to the location of the sprite for the character in VX.
                    Ireg = V[ get_X(opcode) ] * 5; // each character sprite are 5 bytes long
                    PC += 2;
                    break;
                case 0x0033:
                    FX33(opcode);
                    PC += 2;
                    break;
                case 0x0055:
                    // FX55 Stores V0 to VX in memory starting at address I
                    for (unsigned short i = 0; i <= get_X(opcode); i++)
                    {
                        memory[Ireg + i] = V[i];
                    }
                    PC += 2;
                    break;
                case 0x0065:
                    // FX65 Read registers V0 through Vx from memory starting at location I.
                    // The interpreter reads values from memory starting at location I into registers V0 through Vx.
                    for (unsigned short i = 0; i <= get_X(opcode); i++)
                    {
                        V[i] = memory[Ireg + i];
                    }
                    PC += 2;
                    break;
                default:
                    printf ("Unknown opcode: 0x%04x\n", opcode);
                    PC += 2;
                    break;
            }
            break;
        default:
            printf ("Unknown opcode: 0x%04x\n", opcode);
            PC += 2;
            break;
    }
    
    // Update timers
    return;
}

// X - is usualy an register index in opcode, like 0x0100
unsigned short get_X(const unsigned short op_code)
{
    return (op_code & 0x0F00) >> 8;
}

// Y - is usualy an register index in opcode, like 0x0010
unsigned short get_Y(const unsigned short op_code)
{
    return (op_code & 0x00F0) >> 4;
}

// FX33 Opcode
/* 
 Stores the Binary-coded decimal representation of VX, with the most significant of three digits at the address 
 in I,the middle digit at I plus 1, and the least significant digit at I plus 2.
 (In other words, take the decimal representation of VX, place the hundreds digit in memory at location in I, 
 the tens digit at location I+1, and the ones digit at location I+2.)
*/
void FX33(const unsigned short op_code)
{
    // register index
    short X = op_code & 0x0F00;
    memory[Ireg]     = V[(X) >> 8] / 100;
    memory[Ireg + 1] = (V[(X) >> 8] / 10) % 10;
    memory[Ireg + 2] = (V[(X) >> 8] % 100) % 10;
    return;
}

// DXYN Opcode
/*
 Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision.
 
 The interpreter reads n bytes from memory, starting at the address stored in I. 
 These bytes are then displayed as sprites on screen at coordinates (Vx, Vy). 
 Sprites are XORed onto the existing screen. If this causes any pixels to be erased, 
 VF is set to 1, otherwise it is set to 0.
 If the sprite is positioned so part of it is outside the coordinates of the display, 
 it wraps around to the opposite side of the screen.
*/
void DXYN(const unsigned short op_code)
{
    unsigned short x = V[ get_X(op_code) ];
    unsigned short y = V[ get_Y(op_code) ];
    unsigned short height = opcode & 0x000F;
    unsigned short pixel;
    
    V[0xF] = 0;
    for (int yline = 0; yline < height; yline++)
    {
        pixel = memory[Ireg + yline];
        for(int xline = 0; xline < 8; xline++)
        {
            if((pixel & (0x80 >> xline)) != 0)
            {
                if(display[(x + xline + ((y + yline) * 64))] == 1)
                    V[0xF] = 1;
                display[x + xline + ((y + yline) * 64)] ^= 1;
            }
        }
    }
    
    draw_flag = true;

    return;
}