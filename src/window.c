#include "window.h"
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <SDL2/SDL.h>

void open_window(void){
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    }
    SDL_Window* window = SDL_CreateWindow(
        "SDL Window",                  // Window title
        SDL_WINDOWPOS_CENTERED,        // Initial x position
        SDL_WINDOWPOS_CENTERED,        // Initial y position
        800,                           // Width, in pixels
        600,                           // Height, in pixels
        SDL_WINDOW_SHOWN               // Window flags
    );
    
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_Quit();
    }

    SDL_Event e;
    int running = 1;
    while (running) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                running = 0;
            }
        }

        // Clear the window with a white background
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);
    }
    
    // SDL_DestroyWindow(window);
    // SDL_Quit();
    }