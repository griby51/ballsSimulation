#include "Simulation2.hpp"
#include "SDL2_gfxPrimitives.h"
#include <iostream>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 800;
const int CIRCLE_RADIUS = 100;
const int BALL_RADIUS = 10;
const float GRAVITY = 0.2f;
const int CIRCLE_WIDTH = 5;
const int CENTER_X = WINDOW_WIDTH / 2;
const int CENTER_Y = WINDOW_HEIGHT / 2;
const SDL_Color WHITE = { 255, 255, 255, 255 };
const int NUM_ARCS = 10;
double ANGLE_SIZE = 50.0;
double ANGLE_START = 0.0;
const int GAP_BETWEEN_ARCS = 10;
const int PARTICLE_PER_ARC = 500;

Arc* activeArc;

Simulation2::Simulation2() :
	ball(Ball(CENTER_X, CENTER_Y, RandomUtils::randomFloat(-3, 3), RandomUtils::randomFloat(-3, 3), BALL_RADIUS, RandomUtils::randomColor())),
	running(true),
	gen(rd()) {}

void Simulation2::initialize() {
	arcs.clear();

	for (int i = 0; i < NUM_ARCS; ++i) {
		int radius = i * GAP_BETWEEN_ARCS + CIRCLE_RADIUS;
		float speed = 0.1f + (NUM_ARCS - 1 - i) * 0.1f;
		arcs.emplace_back(CENTER_X, CENTER_Y, radius, CIRCLE_WIDTH, ANGLE_START, ANGLE_SIZE, speed);
	}

	if (!arcs.empty()) activeArc = &arcs[0];
}

void Simulation2::handleEvent(const SDL_Event& event) {
	if (event.type == SDL_QUIT) running = false;
}

void Simulation2::update() {
	
	for (auto& arc : arcs) {
		arc.update();
	}

	for (auto& particle : particles) {
		particle.update();
	}

	ball.update(GRAVITY);
	

	float dx = ball.x - CENTER_X;
	float dy = ball.y - CENTER_Y;

	float dist = std::sqrt(dx * dx + dy * dy);

	if (activeArc && dist + ball.radius >= activeArc->radius) {

		if (!activeArc->contains((int)ball.x, (int)ball.y)) {
			float nx = dx / dist;
			float ny = dy / dist;

			ball.x = activeArc->centerX + nx * (activeArc->radius - ball.radius);
			ball.y = activeArc->centerY + ny * (activeArc->radius - ball.radius);

			float dot = ball.vx * nx + ball.vy * ny;

			ball.vx -= 2 * dot * nx;
			ball.vy -= 2 * dot * ny;
		}
		else {
			ball.hasExited = true;
		}

		if (ball.hasExited) {
			if (!arcs.empty()) {

				float full_circle = 2 * M_PI;
				float start = fmod(activeArc->angleEnd, full_circle);
				float end = fmod(activeArc->angleStart, full_circle);

				if (start > end) {
					end += full_circle;
				}
				float visibleAngleStart = activeArc->angleStart + activeArc->angleSize;
				for (int i = 0; i < PARTICLE_PER_ARC; ++i) {
					float visibleAngle = visibleAngleStart + RandomUtils::randomFloat(0, 360.0f - activeArc->angleSize);
					visibleAngle = fmod(visibleAngle, 360.0f);

					float angleRad = visibleAngle * M_PI / 180.0f;

					float px = activeArc->centerX + activeArc->radius * cos(angleRad);
					float py = activeArc->centerY + activeArc->radius * sin(angleRad);

					float speed = RandomUtils::randomFloat(0.5f, 1.5f);
					float vx = cos(angleRad) * speed + RandomUtils::randomFloat(-0.5f, 0.5f);
					float vy = sin(angleRad) * speed + RandomUtils::randomFloat(-0.5f, 0.5f);

					SDL_Color color = WHITE;
					int lifeTime = RandomUtils::randomFloat(50, 100);
					float size = RandomUtils::randomFloat(1, 5);

					particles.emplace_back(px, py, vx, vy, size,color, lifeTime);
				}

				arcs.erase(arcs.begin());

				if (!arcs.empty()) {
					activeArc = &arcs[0];
					ball.hasExited = false;
				}
				else activeArc = nullptr;
			}
		}


		if (ball.isOutOfScreen(WINDOW_WIDTH, WINDOW_HEIGHT)) {
			running = false;
		}

	}
}

void Simulation2::render(SDL_Renderer* renderer) {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	for (auto& arc : arcs) {
		arc.draw(renderer, WHITE);
	}

	for (auto it = particles.begin(); it != particles.end();) {
		it->draw(renderer);
		if (!it->isAlive()) {
			it = particles.erase(it);
		}
		else {
			++it;
		}
	}

	ball.draw(renderer);
}

bool Simulation2::isRunning() const {
	return running;
}