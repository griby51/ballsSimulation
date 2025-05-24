#include "Ball.hpp"
#include "SDL2_gfxPrimitives.h"
#include <iostream>
#include <string>

Ball::Ball(float x, float y, float vx, float vy, int radius, SDL_Color color, bool isFilled) : x(x), y(y), vx(vx), vy(vy), radius(radius), color(color), isFilled(isFilled), hasExited(false) {}

void Ball::update(float gravity = 0.2) {
	x += vx;
	y += vy;
	vy += gravity;
}

bool Ball::isOutOfScreen(int screenWidth, int screenHeight) const {
	return (x + radius < 0 || x - radius > screenWidth || y + radius < 0 || y - radius > screenHeight);
}

void Ball::draw(SDL_Renderer* renderer) const {
	filledCircleRGBA(renderer, (Sint16)x, (Sint16)y, radius, color.r, color.g, color.b, color.a);
	if (!isFilled) {
		circleRGBA(renderer, (Sint16)x, (Sint16)y, radius, 0, 0, 0, 255);
	}
}