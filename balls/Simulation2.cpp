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
const SDL_Color GREEN = { 0, 255, 0, 255 };
const SDL_Color RED = { 255, 0, 0, 255 };

const float ARC_SPEED = 0.15f;
const int NUM_ARCS = 10;
double ANGLE_SIZE = 50.0;
double ANGLE_START = 0.0;
const int GAP_BETWEEN_ARCS = 10;
const int PARTICLE_PER_ARC = 500;

Arc* activeArc;

Simulation2::Simulation2() :
	balls(),
	running(true),
	BOUNCE_SFX(nullptr), DESTROY_SFX(nullptr),
	gen(rd()) {}

void Simulation2::initialize() {
	arcs.clear();

	balls.emplace_back(CENTER_X, CENTER_Y, RandomUtils::randomFloat(-3.0f, 3.0f), RandomUtils::randomFloat(-3.0f, 3.0f), BALL_RADIUS, GREEN, true);
	balls.emplace_back(CENTER_X, CENTER_Y, RandomUtils::randomFloat(-3.0f, 3.0f), RandomUtils::randomFloat(-3.0f, 3.0f), BALL_RADIUS, RED, true);

	char* basePath = SDL_GetBasePath();

	if (basePath) {
		executablePath = basePath;
		SDL_free(basePath);
	}
	else {
		std::cerr << "Failed to get base path: " << SDL_GetError() << std::endl;
		return;
	}

	for (int i = 0; i < NUM_ARCS; ++i) {
		int radius = i * GAP_BETWEEN_ARCS + CIRCLE_RADIUS;
		float speed = ARC_SPEED + (NUM_ARCS - 1 - i) * ARC_SPEED;
		arcs.emplace_back(CENTER_X, CENTER_Y, radius, CIRCLE_WIDTH, ANGLE_START, ANGLE_SIZE, speed);
	}

	std::string bouncePath = executablePath + "bounce.wav";
	std::string destroyPath = executablePath + "destroy.wav";



	BOUNCE_SFX = Mix_LoadWAV(bouncePath.c_str());
	DESTROY_SFX = Mix_LoadWAV(destroyPath.c_str());

	if (!BOUNCE_SFX) {
		std::cerr << "Failed to load sound effect: " << Mix_GetError() << std::endl;
		return;
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

	for (auto& ball : balls) {
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

				Mix_PlayChannel(-1, BOUNCE_SFX, 0);
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

						particles.emplace_back(px, py, vx, vy, size, color, lifeTime);
					}

					Mix_PlayChannel(-1, DESTROY_SFX, 0);

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

	for (auto& ball : balls) {
		ball.draw(renderer);
	}
}

bool Simulation2::isRunning() const {
	return running;
}

Simulation2::~Simulation2() {
	Mix_FreeChunk(BOUNCE_SFX);
	Mix_FreeChunk(DESTROY_SFX);
	Mix_CloseAudio();
}