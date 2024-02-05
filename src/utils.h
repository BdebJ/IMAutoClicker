#pragma once
#include <random>

namespace utils
{
	enum MouseButton { LEFT, MIDDLE, RIGHT };
	std::mt19937_64 RandomGeneratorEngine();
	inline std::mt19937_64 mt{ RandomGeneratorEngine() };
	int GetRandomNum(int max);
}