#include "Simulation1.hpp"
#include "SDL2_gfxPrimitives.h""

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 800;
const int CIRCLE_RADIUS = 200;
const int BALL_RADIUS = 10;
const float GRAVITY = 0.2f;
const int CIRCLE_WIDTH = 5;
const int CENTER_X = WINDOW_WIDTH / 2;
const int CENTER_Y = WINDOW_HEIGHT / 2;
const SDL_Color WHITE = { 255, 255, 255, 255 };

Simulation1::Simulation1() : gameArc(CENTER_X, CENTER_Y, CIRCLE_RADIUS, 5, 0.0, 30.0, 1.0f), running(true), gen(rd()), BOUNCE_SFX(nullptr) {}

void Simulation1::initialize() {
	balls.clear();
	newBalls.clear();
	balls.emplace_back(CENTER_X, CENTER_Y, RandomUtils::randomFloat(-3, 3), RandomUtils::randomFloat(-3, 3), BALL_RADIUS, RandomUtils::randomColor());

	char* basePath = SDL_GetBasePath();
	if (basePath) {
		executablePath = basePath;
		SDL_free(basePath);
	}
	else {
		std::cerr << "Failed to get base path: " << SDL_GetError() << std::endl;
		return;
	}
	std::string bouncePath = executablePath + "bounce.wav";
	BOUNCE_SFX = Mix_LoadWAV(bouncePath.c_str());

	Mix_AllocateChannels(32);
}

void Simulation1::handleEvent(const SDL_Event& event) {
	if (event.type == SDL_QUIT) running = false;
}

void Simulation1::update() {
	gameArc.update();

	for (auto& ball : balls) {
		ball.update(GRAVITY);

		float dx = ball.x - CENTER_X;
		float dy = ball.y - CENTER_Y;

		float dist = std::sqrt(dx * dx + dy * dy);

		if (dist + ball.radius >= CIRCLE_RADIUS) {
			if (!gameArc.contains((int)ball.x, (int)ball.y) && !ball.hasExited) {
				float nx = dx / dist;
				float ny = dy / dist;

				ball.x = CENTER_X + nx * (CIRCLE_RADIUS - ball.radius);
				ball.y = CENTER_Y + ny * (CIRCLE_RADIUS - ball.radius);

				float dot = ball.vx * nx + ball.vy * ny;

				ball.vx -= 2 * dot * nx;
				ball.vy -= 2 * dot * ny;
				Mix_PlayChannel(-1, BOUNCE_SFX, 0);
			}
			else {
				ball.hasExited = true;
			}

			if (ball.isOutOfScreen(WINDOW_WIDTH, WINDOW_HEIGHT)) {
				newBalls.emplace_back(CENTER_X, CENTER_Y, RandomUtils::randomFloat(-3, 3), RandomUtils::randomFloat(-3, 3), BALL_RADIUS, RandomUtils::randomColor());
				newBalls.emplace_back(CENTER_X, CENTER_Y, RandomUtils::randomFloat(-3, 3), RandomUtils::randomFloat(-3, 3), BALL_RADIUS, RandomUtils::randomColor());
			}
		}

	}
	balls.insert(balls.end(), newBalls.begin(), newBalls.end());
	newBalls.clear();

	balls.erase(
		std::remove_if(balls.begin(), balls.end(),
			[](const Ball& b) { return b.isOutOfScreen(WINDOW_WIDTH, WINDOW_HEIGHT); }
		),
		balls.end()
	);
}

void Simulation1::render(SDL_Renderer* renderer) {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	gameArc.draw(renderer, WHITE);

	for (const auto& ball : balls) {
		ball.draw(renderer);
	}
}

bool Simulation1::isRunning() const {
	return running;
}

Simulation1::~Simulation1() {
	Mix_FreeChunk(BOUNCE_SFX);
	Mix_CloseAudio();
}