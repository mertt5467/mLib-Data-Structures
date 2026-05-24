#pragma once

#include <random>

namespace mLib {
	inline std::mt19937& getGlobalRNG() noexcept {
		thread_local static std::mt19937 gen(std::random_device{}());
		return gen;
	}
}