#pragma once

#include "debug.hpp"

namespace mLib {
	template<typename T>
	void bubbleSort(T& list) {
		if (list.begin() == list.end()) {
			return;
		}
		auto it = list.begin();
		int n = list.getSize();
		for (int i = 0; i < n; ++i) {
			for (int j = 0; j < (n - i - 1); ++j) {
				auto temp = it;
				++it;
				if (*temp > *it) {
					auto move = static_cast<typename std::remove_reference<decltype(*temp)>::type&&>(*temp);
					*temp = static_cast<typename std::remove_reference<decltype(*it)>::type&&>(*it);
					*it = static_cast<typename std::remove_reference<decltype(move)>::type&&>(move);
				}
			}
			it = list.begin();
		}
	}
	template<typename T, typename Compare>
	void bubbleSort(T& list, Compare comp) {
		if (list.begin() == list.end()) {
			return;
		}
		auto it = list.begin();
		int n = list.getSize();
		for (int i = 0; i < n; ++i) {
			for (int j = 0; j < (n - i - 1); ++j) {
				auto temp = it;
				++it;
				if (comp(*temp, *it)) {
					auto move = static_cast<typename std::remove_reference<decltype(*temp)>::type&&>(*temp);
					*temp = static_cast<typename std::remove_reference<decltype(*it)>::type&&>(*it);
					*it = static_cast<typename std::remove_reference<decltype(move)>::type&&>(move);
				}
			}
			it = list.begin();
		}
	}
}