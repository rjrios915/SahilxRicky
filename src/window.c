#include "window.h"
#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2_image/SDL_image.h>

#define WINDOW_WIDTH (1200)
#define WINDOW_HEIGHT (750)
#define SPEED (600)

void open_window(void){
    // Check the SDL library is loaded correctly
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    } 

    // Create window instance
    SDL_Window* window = SDL_CreateWindow("SahilxRicky", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    
    // Create renderer instance
    Uint32 flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, flags);

    // Check renderer was created
    if (!renderer) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return;
    }

    // Reads in image 
    SDL_Surface* surface = IMG_Load("../res/images/sahil.png");
    if (!surface) {
        printf("Image could not be read! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return;
    }

    // Creates a texture (image data in VRAM) from the surface
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!surface) {
        printf("Texture could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return;
    }

    // Free up memory because image is now stored as texture
    SDL_FreeSurface(surface);

    // Strucut to sprite image size and pos: w, h, x, y
    SDL_Rect dest;

    // Get info about texture
    SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);

    // Centers image on y axis
    dest.y = (WINDOW_HEIGHT - dest.h) / 2;

    SDL_Event e;
    int running = 1;
    // Set the draw color
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    float x_pos = (float) -dest.w;
    while (running) {

        // Make sure events aren't pedning
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                running = 0;
            }
        }

        if (dest.x <= WINDOW_WIDTH) {
            // Clear the screen with the set color
            SDL_RenderClear(renderer);

            // Update image position
            dest.x = (int) x_pos;

            // Put image on 
            SDL_RenderCopy(renderer, texture, NULL, &dest);
            // Show the window
            SDL_RenderPresent(renderer);
            
            // Increment the new position
            x_pos += (float) SPEED / 60;

            // Wait 1/60th of a second
            SDL_Delay(1000/60);
        }
    }
    
    // Safe 
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    }