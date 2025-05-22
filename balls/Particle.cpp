#include "Particle.hpp"
#include <SDL2_gfxPrimitives.h>
#include <algorithm>

Particle::Particle(float x, float y, float vx, float vy, float size, SDL_Color color, float lifeTime)
	: x(x), y(y), vx(vx), vy(vy), size(size), color(color), lifeTime(lifeTime), maxLifeTime(lifeTime) {
}

void Particle::update() {
	x += vx;
	y += vy;
	lifeTime--;
}

void Particle::draw(SDL_Renderer* renderer) {
	if (lifeTime > 0) {
		float radius = 1 / (maxLifeTime / lifeTime) * size;

		filledCircleRGBA(renderer, static_cast<Sint16>(x), static_cast<Sint16>(y), radius,
			color.r, color.g, color.b, color.a);
	}
}