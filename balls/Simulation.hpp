#pragma once
#include <SDL2/SDL.h>

class Simulation {
public:
	virtual ~Simulation() = default;
	virtual void initialize() = 0;
	virtual void handleEvent(const SDL_Event& event) = 0;
	virtual void update() = 0;
	virtual void render(SDL_Renderer* renderer) = 0;
	virtual bool isRunning() const = 0;
};