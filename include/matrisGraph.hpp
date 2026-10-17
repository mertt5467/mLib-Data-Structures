#pragma once

#include <iostream>
#include <stdexcept>
#include <string>
#include <initializer_list>
#include "list.hpp"
#include "chainingHash.hpp"
#include "debug.hpp"

namespace mLib {
	template <typename T, typename N = size_t>
	class MatrisGraph {
	private:
		mLib::List<mLib::List<N>> matris;
		mLib::List<T> values;

		void checkValue(const T& value) const {
			size_t count = 0;
			for (const auto& item : values) {
				if (item == value) {
					throw std::invalid_argument("The entered value already exists at index " + std::to_string(count) + ".");
				}
				count++;
			}
		}
		void checkValue(T&& value) const {
			size_t count = 0;
			for (const auto& item : values) {
				if (item == value) {
					throw std::invalid_argument("The entered value already exists at index " + std::to_string(count) + ".");
				}
				count++;
			}
		}
		// Completed
		void extend() {
			matris.add(mLib::List<N>(matris.getCap(), matris.infoAutoShrink()));
			size_t row_index = values.getSize() - 1;
			for (auto& row : matris) {
				if (&row == &matris[row_index]) {
					while (row.getSize() < values.getSize()) {
						row.add(0);
					}
					return;
				} else {
					row.add(0);
				}
			}
		}
	public:
		MatrisGraph() : matris(), values(){}

		explicit MatrisGraph(size_t cap) : matris(cap), values(cap) {} // @TODO: Values bos, ekleme sirasinda arttirilacak

		explicit MatrisGraph(bool autoShrink_param) : matris(autoShrink_param), values(autoShrink_param){}

		MatrisGraph(size_t cap, bool autoShrink_param) : matris(cap, autoShrink_param), values(cap, autoShrink_param) {}

		MatrisGraph(const MatrisGraph& other) : matris(other.matris), values(other.values) {}

		MatrisGraph(MatrisGraph&& other) noexcept : matris(static_cast<mLib::List<mLib::List<N>>&&>(other.matris)), values(static_cast<mLib::List<T>&&>(other.values)) {}

		MatrisGraph& operator=(const MatrisGraph& other) {
			if (this != &other) {
				matris = other.matris;
				values = other.values;
			}
			return *this;
		}
		MatrisGraph& operator=(MatrisGraph&& other) noexcept {
			if (this != &other) {
				matris = static_cast<mLib::List<mLib::List<N>>&&>(other.matris);
				values = static_cast<mLib::List<T>&&>(other.values);
			}
			return *this;
		}

		void add(const T& value) {
			checkValue(value);
			values.add(value);
			try {
				extend();
			}catch(...) {
				values.remove();
				throw;
			}
		}

		void add(T&& value) {
			checkValue(static_cast<T&&>(value));
			values.add(static_cast<T&&>(value));
			try {
				extend();
			}catch(...) {
				values.remove();
				throw;
			}
		}
		void addEdge(const T& value1, const T& value2) {
			// @TODO: WRITE ME!!! (agirlikli/agirliksiz matris'e uyumlu yazilacak)
		}
		// @TODO: ONLY TEST, DELETE ME!!!
		void printMatris() const {
			std::cout << matris << std::endl;
			std::cout << "FINISH" << std::endl;
		}

		void reserve(size_t cap) {
			if (cap > matris.getCap()) {
				matris.reserve(cap);
				values.reserve(cap);
				for (auto& row : matris) {
					row.reserve(cap);
				}
			}
		}
		inline size_t getSize() const noexcept { return values.getSize(); }
		inline size_t getCap() const noexcept { return values.getCap(); }
		inline bool infoAutoShrink() const noexcept { return values.infoAutoShrink(); }
		inline void setAutoShrink(bool choose) noexcept { values.setAutoShrink(choose); matris.setAutoShrink(choose); }
	};
}