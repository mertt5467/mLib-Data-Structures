#pragma once

#include <iostream>
#include <stdexcept>
#include <string>
#include <initializer_list>
#include "array.hpp"
#include "sll.hpp"
#include "utils.hpp"

namespace mLib {
	template <typename T, typename U = size_t>
	class ChainingHash {
	private:
		struct Entry {
			U key;
			T value;

			Entry(const T& value_param, const U& key_param) : value(value_param), key(key_param){}

			Entry(T&& value_param, U&& key_param) noexcept : value(static_cast<T&&>(value_param)), key(static_cast<U&&>(key_param)){}

			Entry(const T& value_param, U&& key_param) noexcept : value(value_param), key(static_cast<U&&>(key_param)){}

			Entry(T&& value_param, const U& key_param) noexcept : value(static_cast<T&&>(value_param)), key(key_param){}

			Entry(const Entry& other) : value(other.value), key(other.key){}

			Entry(Entry&& other) noexcept : value(static_cast<T&&>(other.value)), key(static_cast<U&&>(other.key)) {}

			Entry& operator=(const Entry& other) {
				if (this != &other) {
					key = other.key;
					value = other.value;
				}
				return *this;
			}

			Entry& operator=(Entry&& other) noexcept {
				if (this != &other) {
					value = static_cast<T&&>(other.value);
					key = static_cast<U&&>(other.key);
				}
				return *this;
			}
			bool operator==(const Entry& other) const noexcept {
				return this->key == other.key;
			}
			friend std::ostream& operator<<(std::ostream & os, const Entry& entry) {
				return os << "Value: " << entry.value << " - Key: " << entry.key << " ";
			}
		};
		mLib::Array<mLib::Sll<Entry>> table;
		size_t size;
		bool autoRehash;
		size_t getHashIndex(const U& key) const {
			size_t converted_key = mLib::Hash<U>{}(key);
			return converted_key % mLib::max(table.getSize(), static_cast<size_t>(1));
		}

		void add(Entry&& myEntry) {
			if (autoRehash && size >= table.getSize()) { rehash(table.getSize() * 2); }
			size_t index = getHashIndex(myEntry.key);
			long long result = table[index].indexOf(myEntry);
			if (result == -1) { table[index].addFirst(static_cast<Entry&&>(myEntry)); size++; }
			else { table[index].set(result, static_cast<Entry&&>(myEntry)); }
		}

		void move(Entry&& myEntry) {
			size_t index = getHashIndex(myEntry.key);
			table[index].addLast(static_cast<Entry&&>(myEntry));
		}
		void rehash(size_t target) {
			if (size > 0 && target < 1) { throw std::invalid_argument("Invalid target capacity: Target capacity (" + std::to_string(target) + ") cannot be less than 1 when there is data (" + std::to_string(size) + " items) within it."); }
			ChainingHash temp(target);
			for (size_t i = 0; i < getCap(); ++i) {
				while (!table[i].isEmpty()) {
					temp.move(table[i].removeFirst());
				}
			}
			temp.size = size;
			*this = static_cast<ChainingHash&&>(temp);
		}
	public:

		explicit ChainingHash(size_t capacity) : table(capacity), size(0), autoRehash(true){}

		ChainingHash(size_t capacity, bool autoRehash_param) : table(capacity), size(0), autoRehash(autoRehash_param) {}

		ChainingHash(std::initializer_list<mLib::pair<T, U>> initList) : table(initList.size() * 2), size(0), autoRehash(true) {
			for (auto& item : initList) {
				add(item.first, item.second);
			}
		}
		ChainingHash(const ChainingHash& other) : table(other.table), size(other.size), autoRehash(other.autoRehash) {}

		ChainingHash(ChainingHash&& other) noexcept : table(static_cast<mLib::Array<mLib::Sll<Entry>>&&>(other.table)), size(other.size), autoRehash(other.autoRehash) {
			other.size = 0;
		}

		ChainingHash& operator=(const ChainingHash& other) {
			if (this != &other) {
				table = other.table;
				size = other.size;
				autoRehash = other.autoRehash;
			}
			return *this;
		}
		ChainingHash& operator=(ChainingHash&& other) noexcept{
			if (this != &other) {
				table = static_cast<mLib::Array<mLib::Sll<Entry>>&&>(other.table);
				size = other.size;
				autoRehash = other.autoRehash;
			}
			return *this;
		}

		~ChainingHash() {
			size = 0;
		}

		void add(const T& value, const U& key) {
			add(Entry(value, key));
		}
		void add(T&& value, U&& key) {
			add(Entry(static_cast<T&&>(value), static_cast<U&&>(key)));
		}
		void add(const T& value, U&& key) {
			add(Entry(value, static_cast<U&&>(key)));
		}
		void add(T&& value, const U& key) {
			add(Entry(static_cast<T&&>(value), key));
		}

		T& get(const U& key) {
			if (isEmpty()) { throw std::underflow_error("Hash underflow: Operation not allowed on an empty hash table."); }
			size_t index = getHashIndex(key);
			for (auto& entry : table[index]) {
				if (entry.key == key) { return entry.value; }
			}
			throw std::invalid_argument("Key not found in the hash table.");
		}

		const T& get(const U& key) const {
			if(isEmpty()){ throw std::underflow_error("Hash underflow: Operation not allowed on an empty hash table."); }
			size_t index = getHashIndex(key);
			for (const auto& entry : table[index]) {
				if (entry.key == key) { return entry.value; }
			}
			throw std::invalid_argument("Key not found in the hash table.");
		}

		void set(const U& key, const T& value) {
			if (isEmpty()) { throw std::underflow_error("Hash underflow: Operation not allowed on an empty hash table."); }
			size_t index = getHashIndex(key);
			for (auto& entry : table[index]) {
				if (entry.key == key) { entry.value = value; }
			}
			throw std::invalid_argument("Key not found in the hash table.");
		}

		void set(const U& key, T&& value) {
			if (isEmpty()) { throw std::underflow_error("Hash underflow: Operation not allowed on an empty hash table."); }
			size_t index = getHashIndex(key);
			for (auto& entry : table[index]) {
				if (entry.key == key) { entry.value = static_cast<T&&>(value); }
			}
			throw std::invalid_argument("Key not found in the hash table.");
		}

		T remove(const U& key) {
			if (isEmpty()) { throw std::underflow_error("Hash underflow: Operation not allowed on an empty hash table."); }
			size_t index = getHashIndex(key);
			size_t result = 0;
			for (auto& entry : table[index]) {
				if (entry.key == key) { size--; return table[index].remove(result).value; }
				result++;
			}
			throw std::invalid_argument("Key not found in the hash table.");
		}
		class Iterator {
		private:
			size_t current;
			mLib::Array<mLib::Sll<Entry>>* table_ptr;
			mLib::Sll<Entry>::Iterator it;
		public:
			struct HashProxy {
				T& value;
				const U& key;
				friend std::ostream& operator<<(std::ostream& os, const HashProxy& proxy) {
					return os << "Value: " << proxy.value << " - Key: " << proxy.key << " ";
				}
			};
			using iterator_category = std::forward_iterator_tag;
            using value_type = HashProxy;
            using difference_type = std::ptrdiff_t;
            using reference = HashProxy;
            using pointer = void;

			Iterator(mLib::Array<mLib::Sll<Entry>>* table_ptr_param, size_t index) noexcept : table_ptr(table_ptr_param), current(index), it(nullptr) {
				if (table_ptr->getSize() > current) { 
					it = (*table_ptr)[current].begin();
				}
			}

			reference operator*() const noexcept{
				return HashProxy{ it->value, it->key };
			}

			Iterator& operator++() noexcept {
				if (++it == (*table_ptr)[current].end()) {
					for (size_t i = current + 1; i < table_ptr->getSize(); ++i) {
						if ((*table_ptr)[i].isEmpty()) { continue; }
						else {
							it = (*table_ptr)[i].begin();
							current = i;
							return *this;
						}
					}
					current = table_ptr->getSize();
					return *this;
				}
				else {
					return *this;
				}
			}
			Iterator operator++(int) noexcept {
				Iterator temp = (*this);
				++(*this);
				return temp;
			}
			bool operator==(const Iterator& other) const noexcept{
				return (this->current == other.current) && (this->table_ptr == other.table_ptr) && (this->it == other.it);
			}
			bool operator!=(const Iterator& other) const noexcept {
				return !(operator==(other));
			}
		};
		class Const_Iterator {
		private:
			size_t current;
			const mLib::Array<mLib::Sll<Entry>>* table_ptr;
			const mLib::Sll<Entry>::Iterator it;
		public:
			struct HashProxy {
				const T& value;
				const U& key;
				friend std::ostream& operator<<(std::ostream& os, const HashProxy& proxy) {
					return os << "Value: " << proxy.value << " - Key: " << proxy.key << " ";
				}
			};
			using iterator_category = std::forward_iterator_tag;
			using value_type = HashProxy;
			using difference_type = std::ptrdiff_t;
			using reference = const HashProxy;
			using pointer = void;

			Const_Iterator(const mLib::Array<mLib::Sll<Entry>>* table_ptr_param, size_t index) noexcept : table_ptr(table_ptr_param), current(index), it(nullptr) {
				if (table_ptr->getSize() > current) {
					it = (*table_ptr)[current].begin();
				}
			}

			reference operator*() const noexcept {
				return HashProxy{ it->value, it->key };
			}

			Const_Iterator& operator++() noexcept {
				if (++it == (*table_ptr)[current].end()) {
					for (size_t i = current + 1; i < table_ptr->getSize(); ++i) {
						if ((*table_ptr)[i].isEmpty()) { continue; }
						else {
							it = (*table_ptr)[i].begin();
							current = i;
							return *this;
						}
					}
					current = table_ptr->getSize();
				}
				else {
					return *this;
				}
			}
			Const_Iterator operator++(int) noexcept {
				Const_Iterator temp = (*this);
				++(*this);
				return temp;
			}
			bool operator==(const Const_Iterator& other) const noexcept {
				return (this->current == other.current) && (this->table_ptr == other.table_ptr) && (this->it == other.it);
			}
			bool operator!=(const Const_Iterator& other) const noexcept {
				return !(operator==(other));
			}
		};

		Iterator begin() noexcept {
			for (size_t i = 0; i < getCap(); ++i) {
				if (table[i].isEmpty()) { continue; }
				else {
					return Iterator(&table, i);
				}
			}
			return end();
		}
		Iterator end() noexcept {
			return Iterator(&table, getCap());
		}

		Const_Iterator begin() const noexcept {
			for (size_t i = 0; i < getCap(); ++i) {
				if (table[i].isEmpty()) { continue; }
				else {
					return Const_Iterator(&table, i);
				}
			}
			return end();
		}
		Const_Iterator end() const noexcept {
			return Const_Iterator(&table, getCap());
		}

		Const_Iterator cbegin() const noexcept { return begin(); }
		Const_Iterator cend() const noexcept { return end(); }

		friend std::ostream& operator<<(std::ostream& os, const ChainingHash& hash) {
			for (size_t i = 0; i < hash.getCap(); ++i) {
				os << hash.table[i] << std::endl;
			}
			return os;
		}
		inline bool isEmpty() const noexcept { return size == 0; }
		inline size_t getSize() const noexcept { return size; }
		inline size_t getCap() const noexcept { return table.getSize(); }
		inline bool infoAutoRehash() const noexcept { return autoRehash; }
		inline void setAutoRehash(bool choose) noexcept { autoRehash = choose; }
		inline void setCap(size_t target) noexcept { rehash(target); }
	};
}