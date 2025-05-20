#pragma once
#include "Simulation.hpp"
#include "Ball.hpp"
#include <vector>
#include "Arc.hpp"
#include "RandomUtils.hpp"

class Simulation1 : public Simulation {
public:
	Simulation1();
	void initialize() override;
	void handleEvent(const SDL_Event& event) override;
	void update() override;
	void render(SDL_Renderer* renderer) override;
	bool isRunning() const override;

private:
    std::vector<Ball> balls;
    std::vector<Ball> newBalls;

    Arc gameArc;
    const double arc_speed = 1.0;
    bool running;

    std::random_device rd;
    std::mt19937 gen;
};