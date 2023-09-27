#include "snake.h"

#include <SDL2/SDL.h>

#include <iostream>
#include <vector>
#include <cstdint>
#include <cstddef>
#include <unistd.h>

static constexpr std::size_t WIN_WIDTH = 800u;
static constexpr std::size_t WIN_HEIGHT = 600u;
static constexpr int TICK_RATE_MS = 60;

static bool TickSync();

int main(int argc, char *argv[])
{
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        std::cerr << "SDL_Init failed：" << SDL_GetError() << std::endl; 
        return -1;
    }
    
    SDL_Window *pWindow = SDL_CreateWindow("SDL Game",
    	SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
    	WIN_WIDTH, WIN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!pWindow) {
        std::cerr << "[ERR] SDL_CreateWindow failed:" << SDL_GetError() << std::endl;
        return -1;
    }
    
    SDL_Renderer *pRenderer = SDL_CreateRenderer(pWindow, -1,
        SDL_RENDERER_SOFTWARE | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE);
    if (!pRenderer) {
        std::cerr << "[ERR] SDL_CreateRenderer failed:" << SDL_GetError() << std::endl;
        SDL_DestroyWindow(pWindow);
        return -1;
    }
    
    SDL_SetRenderDrawColor(pRenderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(pRenderer);
    SDL_RenderPresent(pRenderer);
    SDL_SetWindowInputFocus(pWindow);
    SDL_StopTextInput();

    Snake snake { WIN_WIDTH, WIN_HEIGHT };
    for (;;) {
        SDL_Event event;
        if (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                std::cout << "[INFO] Quit" << std::endl;
                break;
            } else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_w
                    || event.key.keysym.sym == SDLK_UP) {
                    std::cout << "[INFO] Up" << std::endl;
                    snake.SetStepDirection(Snake::Direction::UP);
                } else if (event.key.keysym.sym == SDLK_x
                    || event.key.keysym.sym == SDLK_DOWN) {
                    std::cout << "[INFO] Down" << std::endl;
                    snake.SetStepDirection(Snake::Direction::DOWN);
                } else if (event.key.keysym.sym == SDLK_a
                    || event.key.keysym.sym == SDLK_LEFT) {
                    std::cout << "[INFO] Left" << std::endl;
                    snake.SetStepDirection(Snake::Direction::LEFT);
                } else if (event.key.keysym.sym == SDLK_d
                    || event.key.keysym.sym == SDLK_RIGHT) {
                    std::cout << "[INFO] Right" << std::endl;
                    snake.SetStepDirection(Snake::Direction::RIGHT);
                }
            }
        }
        
        if (TickSync()) {
            snake.MoveStep();
            SDL_SetRenderDrawColor(pRenderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
            SDL_RenderClear(pRenderer);
            snake.Draw(pRenderer);
            SDL_RenderPresent(pRenderer);
        }
    }
    
    SDL_DestroyRenderer(pRenderer);
    SDL_DestroyWindow(pWindow);
    SDL_Quit();
    return 0;
}

static bool TickSync()
{
    static int tsprev = SDL_GetTicks();
    int tsnow = SDL_GetTicks();
    int delta = tsnow - tsprev;
    if (delta >= TICK_RATE_MS) {
        tsprev = tsnow;
        return true;
    } else {
        return false;
    }
}
