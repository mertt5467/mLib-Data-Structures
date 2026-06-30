#pragma once

#include <random>
#include <iterator>

namespace mLib {
	inline std::mt19937& getGlobalRNG() noexcept {
		thread_local static std::mt19937 gen(std::random_device{}());
		return gen;
	}
	template <typename T1, typename T2>
	struct pair {
		T1 first;
		T2 second;

		pair(const T1& first_param, const T2& second_param) : first(first_param), second(second_param){}

		template<typename U1, typename U2>
		pair(U1&& a, U2&& b) : first(static_cast<U1&&>(a)), second(static_cast<U2&&>(b)){}
	};
	template <typename T>
	struct less {
		constexpr bool operator()(const T& l, const T& r) const {
			return l < r;
		}
	};
	template <typename T>
	struct less<T*> {
		constexpr bool operator()(const T* l, const T* r) const {
			return *l < *r;
		}
	};
	template <typename T>
	struct greater {
		constexpr bool operator()(const T& l, const T& r) const {
			return l > r;
		}
	};
	template <typename T>
	struct greater<T*> {
		constexpr bool operator()(const T* l, const T* r) const {
			return *l > *r;
		}
	};

	template <typename T>
	constexpr const T& max(const T& a, const T& b) {
		return a > b ? a : b;
	}

	template <typename T>
	constexpr const T& min(const T& a, const T& b) {
		return a > b ? b : a;
	}

	template <typename T>
	constexpr T absDiff(const T& a, const T& b) noexcept {
		return (a > b) ? (a - b) : (b - a);
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
		Iterator getMidHelper(Iterator begin, Iterator end, std::random_access_iterator_tag) {
			size_t distance = end - begin;
			return begin + (distance / 2);
		}
		template <typename Iterator, typename tag>
		Iterator getMidHelper(Iterator begin, Iterator end, tag) {
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
	Iterator getMid(Iterator begin, Iterator end) noexcept{
		return helper::getMidHelper(begin, end, typename std::iterator_traits<Iterator>::iterator_category{});
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