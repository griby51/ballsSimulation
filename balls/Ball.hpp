#pragma once
#include <SDL2/SDL.h>

class Ball {
public:
	float x, y;
	float vx, vy;

	bool hasExited = false;
	bool isFilled;

	int radius;
	SDL_Color color;

	Ball(float x, float y, float vx, float vy, int radius, SDL_Color color, bool isFilled);
	void update(float gravity);
	bool isOutOfScreen(int screenWidth, int screenHeight) const;
	void draw(SDL_Renderer* renderer) const;
};
