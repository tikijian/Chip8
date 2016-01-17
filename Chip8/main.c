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
bool draw_flag   = false;
bool cls_flag    = false;

// =============== General purpose functions ==========

// Initialize computer
void initialize();

// Reset computer
void reset();

// Font-to-memory routine
void load_fontset();

// Loads rom into memory
bool load_program(const char *filename);

bool process_events(SDL_Window *window);

// ====================================================


// =============== CONSTANTS ==========================

#define PIXEL_SIZE 10.0
#define W_WIDTH 640
#define W_HEIGHT 320
#define D_WIDTH 64
#define D_HEIGHT 32
#define DISPLAY_LENGTH 2048

// ====================================================

int main(int argc, const char * argv[]) {
    
    initialize();
    bool is_running = true;
    
    SDL_Init(SDL_INIT_EVENTS | SDL_INIT_VIDEO | SDL_INIT_TIMER);
    
    SDL_Window *window = SDL_CreateWindow("CHIP-8 Eulator",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          W_WIDTH,
                                          W_HEIGHT,
                                          0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderSetScale(renderer, PIXEL_SIZE, PIXEL_SIZE);
    
    // Emulation loop
    while (is_running) {
        is_running = process_events(window);
        
        // Emulate one cycle
        emulate_cycle();
        
        if (cls_flag)
        {
            SDL_RenderClear(renderer);
        }
        
        // If the draw flag is set, update the screen
        if (draw_flag) {
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);
            
            
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            int y = 0;
            int x = 0;
            
            for (int i = 0; i < DISPLAY_LENGTH; i++) {
                // if pixel at display[i] is set, then draw color is White
                if(x != 0 && (x % 63 == 0)){
                    x = 0;
                    y += 1;
                }
                else {
                    x++;
                }
                
                if (display[i])
                    SDL_RenderDrawPoint(renderer, x, y);
            }
            SDL_RenderPresent(renderer);
            
            
            draw_flag = false;
        }
        
        // Store key press state (Press and Release)

        SDL_Delay(25);
    }
    
    printf("Bye!\n");
    
    SDL_DestroyRenderer(renderer);
    if(window)
        SDL_DestroyWindow(window);
    
    SDL_Quit();
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
    memset(display, 0, DISPLAY_LENGTH); // clear display data
    
    if (! load_program("roms/TETRIS"))
        exit(1);
}

bool process_events(SDL_Window *window)
{
    SDL_Event event;
    bool is_running = true;
    
    // Check for events
    while(SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_WINDOWEVENT_CLOSE:
                SDL_DestroyWindow(window);
                window = NULL;
                is_running = false;
            break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        is_running = false;
                    break;
                }
            break;
            case SDL_QUIT:
                is_running = false;
            break;
        }
    }
    return is_running;
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


