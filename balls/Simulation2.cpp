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

Arc activeArc;

Simulation2::Simulation2() :
	ball(Ball(CENTER_X, CENTER_Y, RandomUtils::randomFloat(-3, 3), RandomUtils::randomFloat(-3, 3), BALL_RADIUS, RandomUtils::randomColor())),
	running(true),
	gen(rd()) {}

void Simulation2::initialize() {
	arcs.clear();

	// Create arcs in encreasing order of radius, and same angle size
	for (int i = 0; i < NUM_ARCS; ++i) {
		int radius = i * GAP_BETWEEN_ARCS + CIRCLE_RADIUS;
		float speed = 0.1f + (NUM_ARCS - 1 - i) * 0.1f;
		arcs.emplace_back(CENTER_X, CENTER_Y, radius, CIRCLE_WIDTH, ANGLE_START, ANGLE_SIZE, speed);
	}

	if (!arcs.empty()) activeArc = arcs[0];
}

void Simulation2::handleEvent(const SDL_Event& event) {
	if (event.type == SDL_QUIT) running = false;
}

void Simulation2::update() {
	
	for (auto& arc : arcs) {
		arc.update();
	}

	ball.update(GRAVITY);

	float dx = ball.x - CENTER_X;
	float dy = ball.y - CENTER_Y;

	float dist = std::sqrt(dx * dx + dy * dy);

	if (dist + ball.radius >= activeArc.radius) {

		if (activeArc.contains(ball.x, ball.y)) std::cout << "Ball is inside the arc" << std::endl;

		if (!activeArc.contains((int)ball.x, (int)ball.y)) {
			float nx = dx / dist;
			float ny = dy / dist;

			ball.x = activeArc.centerX + nx * (activeArc.radius - ball.radius);
			ball.y = activeArc.centerY + ny * (activeArc.radius - ball.radius);

			float dot = ball.vx * nx + ball.vy * ny;

			ball.vx -= 2 * dot * nx;
			ball.vy -= 2 * dot * ny;
		}
		else {
			ball.hasExited = true;
		}

		if (ball.isOutOfScreen(WINDOW_WIDTH, WINDOW_HEIGHT)) {
			return;
		}

	}
}

void Simulation2::render(SDL_Renderer* renderer) {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	for (auto& arc : arcs) {
		arc.draw(renderer, WHITE);
	}


	ball.draw(renderer);
}

bool Simulation2::isRunning() const {
	return running;
}