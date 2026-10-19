#pragma once

#include <iostream>
#include <stdexcept>
#include <string>
#include <initializer_list>
#include "list.hpp"
#include "chainingHash.hpp"
#include "debug.hpp"
#include "utils.hpp"

namespace mLib {
	template <typename T, typename N = bool>
	class MatrisGraph {
	private:
		mLib::List<mLib::List<N>> matris;
		// mLib::ChainingHash<T, std::string> hash; // @TODO: listede obje arama performansini bigO(n)'den bigO(1)'e getirmek icin hash kullanilacak
		mLib::List<T> values;

		// @REFACTOR: performans icin listede obje sorgu/arama kismi values(list) uzerinden degil hash uzerinden yapilacak.
		void checkValue(const T& value) const {
			size_t count = 0;
			for (const auto& item : values) {
				if (item == value) {
					throw std::invalid_argument("The entered value already exists at index " + std::to_string(count) + ".");
				}
				count++;
			}
		}
		size_t search(const T& value) const{
			// @TODO: hash'de arama ve listedeki indeksini geri donduren fonksiyon yazilacak
			return 0;
		}
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
		// @TODO: Constructorlara hash eklenecek
		MatrisGraph() : matris(), values(){}
		// @TODO: Constructorlara hash eklenecek
		explicit MatrisGraph(size_t cap) : matris(cap), values(cap) {} 
		// @TODO: Constructorlara hash eklenecek
		explicit MatrisGraph(bool autoShrink_param) : matris(autoShrink_param), values(autoShrink_param){}
		// @TODO: Constructorlara hash eklenecek
		MatrisGraph(size_t cap, bool autoShrink_param) : matris(cap, autoShrink_param), values(cap, autoShrink_param) {}
		// @TODO: Constructorlara hash eklenecek
		MatrisGraph(const MatrisGraph& other) : matris(other.matris), values(other.values) {}
		// @TODO: Constructorlara hash eklenecek
		MatrisGraph(MatrisGraph&& other) noexcept : matris(static_cast<mLib::List<mLib::List<N>>&&>(other.matris)), values(static_cast<mLib::List<T>&&>(other.values)) {}
		// @TODO: Constructorlara hash eklenecek
		MatrisGraph& operator=(const MatrisGraph& other) {
			if (this != &other) {
				matris = other.matris;
				values = other.values;
			}
			return *this;
		}
		// @TODO: Constructorlara hash eklenecek
		MatrisGraph& operator=(MatrisGraph&& other) noexcept {
			if (this != &other) {
				matris = static_cast<mLib::List<mLib::List<N>>&&>(other.matris);
				values = static_cast<mLib::List<T>&&>(other.values);
			}
			return *this;
		}
		// Add fonksiyonlarina hash methodu eklenecek
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
		// Add fonksiyonlarina hash methodu eklenecek
		void add(T&& value) {
			checkValue(value);
			values.add(static_cast<T&&>(value));
			try {
				extend();
			}catch(...) {
				values.remove();
				throw;
			}
		}
		void singleEdge(const T& value1, const T& value2) requires mLib::req::is_same<N, bool> { // Unweighted
			checkValue(value1);
			checkValue(value2);
			// @TODO: WRITE ME!!!
		}
		void doubleEdge(const T& value1, const T& value2) requires mLib::req::is_same<N, bool> { // Unweighted
			checkValue(value1);
			checkValue(value2);
			// @TODO: WRITE ME!!!
		}
		void singleEdge(const T& value1, const T& value2, const N& weight) requires (!mLib::req::is_same<N, bool>) { // Weighted
			// @TODO: WRITE ME!!!
		}
		void doubleEdge(const T& value1, const T& value2, const N& weight) requires (!mLib::req::is_same<N, bool>) { // Weighted
			// @TODO: WRITE ME!!!
		}
		// @TODO: ONLY TEST, DELETE ME!!!
		void printMatris() const {
			std::cout << matris << std::endl;
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