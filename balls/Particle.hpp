#pragma once
#include <SDL2/SDL.h>

class Particle
{
public:
	Particle(float x, float y, float vx, float vy, float size, SDL_Color color, float lifeTime);
	void update();
	void draw(SDL_Renderer* renderer);
	bool isAlive() const { return lifeTime > 0; }

private:
	float x, y, vx, vy, size;
	SDL_Color color;
	float lifeTime, maxLifeTime;
};

