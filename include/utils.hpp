#pragma once

#include <random>
#include <iterator>

namespace mLib {
	inline std::mt19937& getGlobalRNG() noexcept {
		thread_local static std::mt19937 gen(std::random_device{}());
		return gen;
	}
	namespace helper {
		template <typename Iterator>
		Iterator itNextHelper(Iterator it, size_t step, std::random_access_iterator_tag) noexcept {
			return it + step;
		}
		template <typename Iterator, typename tag>
		Iterator itNextHelper(Iterator it, size_t step, tag) {
			for (size_t i = 0; i < step; ++i) {
				++it;
			}
			return it;
		}
		template <typename Iterator>
		Iterator getPointsHelper(Iterator begin, Iterator end, std::random_access_iterator_tag) {
			size_t distance = end - begin;
			return begin + (distance / 2);
		}
		template <typename Iterator, typename tag>
		Iterator getPointsHelper(Iterator begin, Iterator end, tag) {
			auto fast = begin;
			auto slow = begin;
			while (fast != end) {
				++fast;
				if (fast != end) { ++fast; ++slow; }
			}
			return slow;
		}
		template <typename Iterator>
		size_t getDistanceHelper(Iterator begin, Iterator end, std::random_access_iterator_tag) noexcept {
			return end - begin;
		}
		template <typename Iterator, typename tag>
		size_t getDistanceHelper(Iterator begin, Iterator end, tag) {
			size_t size = 0;
			for (auto i = begin; i != end; ++i) {
				++size;
			}
			return static_cast<size_t>(size);
		}
	}
	template <typename Iterator>
	size_t getDistance(Iterator begin, Iterator end) noexcept {
		return helper::getDistanceHelper(begin, end, typename std::iterator_traits<Iterator>::iterator_category{});
	}
	template <typename Iterator>
	Iterator getPoints(Iterator begin, Iterator end) noexcept{
		return helper::getPointsHelper(begin, end, typename std::iterator_traits<Iterator>::iterator_category{});
	}
	template <typename Iterator>
	Iterator itNext(Iterator it, size_t step) noexcept{
		return helper::itNextHelper(it, step, typename std::iterator_traits<Iterator>::iterator_category{});
	}
	template <typename Iterator>
	Iterator itNext(Iterator it) noexcept{
		return itNext(it, 1);
	}
}