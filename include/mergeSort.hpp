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
		size_t size = mLib::getDistance(begin, end, typename std::iterator_traits<Iterator>::iterator_category{});
		struct RAII_BUFFER {
			T* ptr;
			size_t allocated_size;
			size_t constructed_size = 0;

			RAII_BUFFER(size_t n) : allocated_size(n) {
				ptr = static_cast<T*>(::operator new(n * sizeof(T)));
			}

			~RAII_BUFFER() {
				for (size_t i = 0; i < constructed_size; ++i) {
					ptr[i].~T();
				}
				::operator delete(ptr);
			}
		};
		RAII_BUFFER buffer(size);
		while (left != mid and right != end) {
			if (comp(*left, *right)) {
				new (&buffer.ptr[buffer.constructed_size]) T(static_cast<T&&>(*left));
				left++;
			}
			else {
				new (&buffer.ptr[buffer.constructed_size]) T(static_cast<T&&>(*right));
				right++;
			}
			buffer.constructed_size++;
		}
		while (left != mid) {
			new (&buffer.ptr[buffer.constructed_size]) T(static_cast<T&&>(*left));
			buffer.constructed_size++;
			left++;
		}
		while (right != end) {
			new (&buffer.ptr[buffer.constructed_size]) T(static_cast<T&&>(*right));
			buffer.constructed_size++;
			right++;
		}
		auto target = begin;
		for (size_t j = 0; j < size; ++j) {
			*target = static_cast<T&&>(buffer.ptr[j]);
			target++;
		}
	}
	template<typename Iterator>
	void mergeSort(Iterator begin, Iterator end) { // BigO(n * log n)
		mergeSort(begin, end, [](const auto& a, const auto& b){
			return a < b;
		});
	}
	template<typename Iterator, typename Compare>
	void mergeSort(Iterator begin, Iterator end, Compare comp) { // BigO(n * log n)
		auto fast = begin;
		auto slow = begin;
		while (fast != end) {
			fast++;
			if (fast != end) { fast++; slow++; }
		}
		if (begin == slow) {
			return;
		}
		mergeSort(begin, slow, comp);
		mergeSort(slow, end, comp);

		merge(begin, slow, end, comp);
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