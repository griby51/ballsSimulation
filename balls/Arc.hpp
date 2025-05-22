#pragma once
#include <SDL2/SDL.h>
#include "SDL2_gfxPrimitives.h"

class Arc {
public:
	Arc() = default;

	Arc(int cx, int cy, int r, int w, double angle_start, double angle_size, float speed);

	void update();
	void draw(SDL_Renderer* renderer, SDL_Color color);
	bool contains(int px, int py);

	void setSpeed(float speed);
	void setAngles(double angle_start, double angle_size);

	int centerX, centerY;
	int radius, width;
	double angleStart, angleSize, angleEnd;

private:
	float rotationSpeed;

	bool sameSide(double x1, double y1, double x2, double y2, double px, double py, double qx, double qy) const;
};


