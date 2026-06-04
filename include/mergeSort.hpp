#pragma once

#include <iterator>
#include "debug.hpp"
#include "utils.hpp"

namespace mLib {
	template<typename Iterator, typename Compare>
	void merge(Iterator begin, Iterator mid, Iterator end, Compare comp) { // BigO(n)

		using T = typename std::iterator_traits<Iterator>::value_type;

		auto left = begin;
		auto right = mid;
		size_t size = mLib::getDistance(begin, end);
		struct OBJ_BUFFER {
			T* ptr;
			size_t constructed_size = 0;

			OBJ_BUFFER(size_t n) {
				ptr = static_cast<T*>(::operator new(n * sizeof(T)));
			}

			~OBJ_BUFFER() {
				for (size_t i = 0; i < constructed_size; ++i) {
					ptr[i].~T();
				}
				::operator delete(ptr);
			}
		};
		OBJ_BUFFER buffer(size);
		while (left != mid and right != end) {
			if (comp(*left, *right)) {
				new (&buffer.ptr[buffer.constructed_size]) T(static_cast<T&&>(*left++));
			}
			else {
				new (&buffer.ptr[buffer.constructed_size])  T(static_cast<T&&>(*right++));
			}
			++buffer.constructed_size;
		}
		while (left != mid) {
			new (&buffer.ptr[buffer.constructed_size])  T(static_cast<T&&>(*left++));
			++buffer.constructed_size;
		}
		while (right != end) {
			new (&buffer.ptr[buffer.constructed_size])  T(static_cast<T&&>(*right++));
			++buffer.constructed_size;
		}
		auto target = begin;
		for (size_t j = 0; j < size; ++j) {
			*target = static_cast<T&&>(buffer.ptr[j]);
			++target;
		}
	}
	template<typename Iterator, typename Compare>
	void mergeSort(Iterator begin, Iterator end, Compare comp) { // BigO(n * log n)
		if (begin == end || itNext(begin, 1) == end) {
			return;
		}
		auto mid = getMid(begin, end);
		mergeSort(begin, mid, comp);
		mergeSort(mid, end, comp);

		merge(begin, mid, end, comp);
	}
	template<typename Iterator>
	void mergeSort(Iterator begin, Iterator end) { // BigO(n * log n)

		using V = typename std::iterator_traits<Iterator>::value_type;

		mergeSort(begin, end, mLib::less<V>{});
	}
	template<typename T>
	void mergeSort(T& list) { // BigO(n * log n)
		auto begin = list.begin();
		auto end = list.end();
		mergeSort(begin, end);
	}
	template<typename T, typename Compare>
	void mergeSort(T& list, Compare comp) { // BigO(n * log n)
		auto begin = list.begin();
		auto end = list.end();
		mergeSort(begin, end, comp);
	}
}