#pragma once
#include "Simulation.hpp"
#include "Ball.hpp"
#include <vector>
#include "Arc.hpp"
#include "RandomUtils.hpp"

class Simulation2 : public Simulation {
public:
    Simulation2();
    void initialize() override;
    void handleEvent(const SDL_Event& event) override;
    void update() override;
    void render(SDL_Renderer* renderer) override;
    bool isRunning() const override;

private:

    Ball ball;

	std::vector<Arc> arcs;

    const double arc_speed = 1.0;
    bool running;

    std::random_device rd;
    std::mt19937 gen;
};