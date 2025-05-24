#pragma once
#include "Simulation.hpp"
#include "Ball.hpp"
#include <vector>
#include "Arc.hpp"
#include "Particle.hpp"
#include "RandomUtils.hpp"
#include <SDL2/SDL_mixer.h>
#include <string>

class Simulation2 : public Simulation {
public:
    Simulation2();
    void initialize() override;
    void handleEvent(const SDL_Event& event) override;
    void update() override;
    void render(SDL_Renderer* renderer) override;
    bool isRunning() const override;
	~Simulation2() override;

private:

    std::string executablePath;

	std::vector<Arc> arcs;
	std::vector<Particle> particles;
	std::vector<Ball> balls;


    const double arc_speed = 1.0;
    bool running;

    Mix_Chunk* BOUNCE_SFX;
	Mix_Chunk* DESTROY_SFX;

    std::random_device rd;
    std::mt19937 gen;
};