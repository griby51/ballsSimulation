#pragma once
#include <SDL2/SDL.h>
#include <random>

namespace RandomUtils {
	static thread_local std::mt19937 gen(std::random_device{}());

	inline float randomFloat(float min, float max) {
		std::uniform_real_distribution<float> dist(min, max);
		return dist(gen);
	}

	inline int randomInt(int min, int max) {
		std::uniform_int_distribution<int> dist(min, max);
		return dist(gen);
	}

	inline SDL_Color randomColor() {
		return { 
			static_cast<Uint8>(randomInt(0, 255)),
			static_cast<Uint8>(randomInt(0, 255)),
			static_cast<Uint8>(randomInt(0, 255)),
			255};
	}
}