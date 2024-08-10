#include "window.h"
#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2_image/SDL_image.h>

#define WINDOW_WIDTH (1200)
#define GRID_WIDTH (WINDOW_WIDTH/50)
#define WINDOW_HEIGHT (750)
#define GRID_HEIGHT (WINDOW_HEIGHT/50)
#define SPEED (50)
#define TILE_SIZE (50)

typedef struct points {
    float x,y;
} points;

void open_window(void) {
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
    SDL_Surface* snake_surface = IMG_Load("../res/images/block.png");
    SDL_Surface* fruit_surface = IMG_Load("../res/images/fruit.png");
    if (!snake_surface || !fruit_surface) {
        printf("Image could not be read! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return;
    }

    // Creates a texture (image data in VRAM) from the surface
    SDL_Texture* snake_texture = SDL_CreateTextureFromSurface(renderer,snake_surface);
    SDL_Texture* fruit_texture = SDL_CreateTextureFromSurface(renderer,fruit_surface);
    if (!snake_texture || !fruit_texture) {
        printf("Texture could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_FreeSurface(snake_surface);
        SDL_FreeSurface(fruit_surface);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return;
    }

    // Free up memory because image is now stored as texture
    SDL_FreeSurface(snake_surface);
    SDL_FreeSurface(fruit_surface);

    // Strucut to sprite image size and pos: w, h, x, y
    SDL_Rect snake_dests[10];
    SDL_Rect fruit_dest;

    // Get info about texture
    SDL_QueryTexture(snake_texture, NULL, NULL, &snake_dests[0].w, &snake_dests[0].h);
    SDL_QueryTexture(snake_texture, NULL, NULL, &fruit_dest.w, &fruit_dest.h);

    snake_dests[0].h = 50;
    snake_dests[0].w = 50;
    fruit_dest.h = 50;
    fruit_dest.w = 50;

    // Centers image on y axis
    float x_pos = 300;
    float y_pos = 100;
    float x_vel = 0;
    float y_vel = 0;


    // Generate fruit points
    float fruit_x = (rand()%GRID_WIDTH)*TILE_SIZE;
    float fruit_y = (rand()%GRID_HEIGHT)*TILE_SIZE;

    // inputs
    int up = 0;
    int down = 0;
    int left = 0;
    int right = 0;

    SDL_Event e;
    int running = 1;
    int tail = 0;

    struct points past_postitions[10];


    // Set the draw color
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    // float x_pos = (float) -snake_dests.w;
    while (running) {

        // Make sure events aren't pedning
        while (SDL_PollEvent(&e)) {
            switch (e.type){
                case SDL_QUIT: 
                    running = 0;
                    break;
                case SDL_KEYDOWN:
                    switch (e.key.keysym.scancode) {
                        case SDL_SCANCODE_W:
                        case SDL_SCANCODE_UP:
                            up = 1;
                            break;
                        case SDL_SCANCODE_A:
                        case SDL_SCANCODE_LEFT:
                            left = 1;
                            break;
                        case SDL_SCANCODE_S:
                        case SDL_SCANCODE_DOWN:
                            down = 1;
                            break;
                        case SDL_SCANCODE_D:
                        case SDL_SCANCODE_RIGHT:
                            right = 1;
                            break;
                    }
                    break;
                case SDL_KEYUP:
                    switch (e.key.keysym.scancode) {
                        case SDL_SCANCODE_W:
                        case SDL_SCANCODE_UP:
                            up = 0;
                            break;
                        case SDL_SCANCODE_A:
                        case SDL_SCANCODE_LEFT:
                            left = 0;
                            break;
                        case SDL_SCANCODE_S:
                        case SDL_SCANCODE_DOWN:
                            down = 0;
                            break;
                        case SDL_SCANCODE_D:
                        case SDL_SCANCODE_RIGHT:
                            right = 0;
                            break;
                    }
                    break;
            }
        }
        if (up || down || left || right){
            x_vel = y_vel = 0;
            if (up && !down) y_vel = -SPEED;
            if (!up && down) y_vel = SPEED;
            if (!right && left) x_vel = -SPEED;
            if (right && !left) x_vel = SPEED;
        } 

        x_pos += (x_vel);
        y_pos += (y_vel);


        if (x_pos < 0) {
            x_pos = WINDOW_WIDTH - snake_dests[0].w;
        } else if (x_pos > WINDOW_WIDTH - snake_dests[0].w) {
            x_pos = 0;
        }
        if (y_pos < 0) y_pos = WINDOW_HEIGHT - snake_dests[0].h;
        else if (y_pos > WINDOW_HEIGHT - snake_dests[0].h) y_pos = 0;

        snake_dests[0].y = (int) y_pos;
        snake_dests[0].x = (int) x_pos;

        if (snake_dests[0].y != fruit_dest.y || snake_dests[0].x != fruit_dest.x){
            fruit_dest.x = (int) fruit_x;
            fruit_dest.y = (int) fruit_y;
        } else {
            fruit_x = (rand()%GRID_WIDTH)*TILE_SIZE;
            fruit_y = (rand()%GRID_HEIGHT)*TILE_SIZE;
            fruit_dest.x = (int) fruit_x;
            fruit_dest.y = (int) fruit_y;
            tail += 1;
            snake_dests[tail].h = 50;
            snake_dests[tail].w = 50;
        }

        // if (x_pos <= 0) x_pos = 0;
        // if (y_pos <= 0) y_pos = 0;
        // if (x_pos >= WINDOW_WIDTH - snake_dests[0].w) x_pos = WINDOW_WIDTH - snake_dests[0].w;
        // if (y_pos >= WINDOW_HEIGHT - snake_dests[0].h) y_pos = WINDOW_HEIGHT - snake_dests[0].h;

        for (int i = tail; i >= 1; i--) {
            snake_dests[i].x = past_postitions[i-1].x;
            snake_dests[i].y= past_postitions[i-1].y ;
            past_postitions[i] = past_postitions[i - 1];
        }

        past_postitions[0].x = x_pos;
        past_postitions[0].y = y_pos;
        

        SDL_RenderClear(renderer);

        for (int i = 0; i <= tail; i++) {
            // printf("%d, x: %d\n", i, snake_dests[i].x);
            // printf("%d, y: %d\n", i, snake_dests[i].y);
            SDL_RenderCopy(renderer, snake_texture, NULL, &snake_dests[i]);
        }

        SDL_RenderCopy(renderer, fruit_texture, NULL, &fruit_dest);
        // Put image on 
        // Show the window
        SDL_RenderPresent(renderer);

        // Wait 1/60th of a second
        SDL_Delay(1000/15);
    }
    
    // Safe 
    SDL_DestroyTexture(snake_texture);
    SDL_DestroyTexture(fruit_texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}