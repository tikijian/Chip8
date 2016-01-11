//
//  opcodes.c
//  Chip8
//
//  Created by Тимур on 11.01.16.
//  Copyright © 2016 Tikijian. All rights reserved.
//


#include "opcodes.h"


void emulate_cycle()
{
    // Fetch Opcode
    opcode = memory[PC] << 8 | memory[PC + 1];
    
    // Decode Opcode
    
    // Execute Opcode
    
    // Update timers
    return;
}