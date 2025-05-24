#pragma once
#include "Simulation.hpp"
#include "Ball.hpp"
#include <vector>
#include "Arc.hpp"
#include "RandomUtils.hpp"
#include <SDL2/SDL_mixer.h>
#include <string>
#include <iostream>

class Simulation1 : public Simulation {
public:
	Simulation1();
	void initialize() override;
	void handleEvent(const SDL_Event& event) override;
	void update() override;
	void render(SDL_Renderer* renderer) override;
	bool isRunning() const override;
	~Simulation1() override;

private:
    std::vector<Ball> balls;
    std::vector<Ball> newBalls;

    Arc gameArc;
    const double arc_speed = 1.0;
    bool running;

	std::string executablePath;

	Mix_Chunk* BOUNCE_SFX;

    std::random_device rd;
    std::mt19937 gen;
};