#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include "Simulation1.hpp"

int main() {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Bouncing Balls", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 800, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    Simulation1 simulation;
    simulation.initialize();

    SDL_Event event;
    while (simulation.isRunning()) {
        while (SDL_PollEvent(&event)) {
            simulation.handleEvent(event);
        }
        simulation.update();
        simulation.render(renderer);
        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}