#pragma once

#include <random>
#include <iterator>

namespace mLib {
	inline std::mt19937& getGlobalRNG() noexcept {
		thread_local static std::mt19937 gen(std::random_device{}());
		return gen;
	}
	template <typename Iterator>
	size_t getDistance(Iterator begin, Iterator end, std::random_access_iterator_tag) noexcept {
		return end - begin;
	}
	template <typename Iterator, typename tag>
	size_t getDistance(Iterator begin, Iterator end, tag) {
		size_t size = 0;
		for (auto i = begin; i != end; ++i) {
			size++;
		}
		return static_cast<size_t>(size);
	}
}