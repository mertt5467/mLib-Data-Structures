#pragma once

#include <iostream>
#include <stdexcept>
#include <initializer_list> // not used, write once
#include "debug.hpp" // not used, write once

namespace mLib {
	template<typename T>
	class Cll {
	private:
		struct Node {
			T value;
			Node* next;
			Node* prev;

			Node(const T& value) : value(value), next(nullptr), prev(nullptr){}

			Node(T&& value) noexcept : value(static_cast<T&&>(value)), next(nullptr), prev(nullptr){}

			Node(const Node& other) = delete;

			Node(Node&& other) = delete;

			Node& operator=(const Node& other) = delete;

			Node& operator=(Node&& other) = delete;
		};
		Node* head;
		Node* tail;
		size_t size;

		void checkEmpty() const{
			if (isEmpty()) { throw std::out_of_range("List is empty."); }
		}
		void checkAvailableBounds(size_t index) const {
			if (index >= size) { throw std::out_of_range("Index out of bounds. Size = " + std::to_string(size)); }
		}
		void checkAddBounds(size_t index) const {
			if (index > size) { throw std::out_of_range("Index out of bounds. Size = " + std::to_string(size)); }
		}

		Node* getNodeAddress(size_t index) noexcept{ // BigO(n)
			Node* temp;
			if (size / 2 > index) {
				temp = head;
				for (size_t i = 0; i < index; ++i) {
					temp = temp->next;
				}
			}
			else {
				temp = tail;
				for (size_t i = size - 1; i > index; --i) {
					temp = temp->prev;
				}
			}
			return temp;
		}
		const Node* getNodeAddress(size_t index) const noexcept { // BigO(n)
			return const_cast<Cll<T>*>(this)->getNodeAddress(index);
		}

	public:

		Cll() : head(nullptr), tail(nullptr), size(0){}

		Cll(const Cll& other) : head(nullptr), tail(nullptr), size(0) {
			try {
				for (auto& item : other) {
					addLast(item);
				}
			}
			catch (...) {
				clear();
				throw;
			}
		}
		Cll(Cll&& other) noexcept : head(other.head), tail(other.tail), size(other.size) {
			other.head = nullptr;
			other.tail = nullptr;
			other.size = 0;
		}
		Cll(std::initializer_list<T> initList) : head(nullptr), tail(nullptr), size(0){
			try {
				for (const auto& item : initList) {
					addLast(item);
				}
			}catch(...){
				clear();
				throw;
			}
		}
		Cll& operator=(const Cll& other) {
			if (this != &other) {
				Cll temp(other);
				*this = static_cast<Cll<T>&&>(temp);
			}
			return *this;
		}
		Cll& operator=(Cll&& other) noexcept{
			if (this != &other) {
				Node* moveHead = other.head;
				Node* moveTail = other.tail;
				size_t moveSize = other.size;

				other.head = head;
				other.tail = tail;
				other.size = size;

				head = moveHead;
				tail = moveTail;
				size = moveSize;
			}
			return *this;
		}
		~Cll() {
			clear();
		}
		void addFirst(const T& value) { // BigO(1)
			Node* myNode = new Node(value);
			if (isEmpty()) {
				myNode->next = myNode;
				myNode->prev = myNode;
				head = myNode;
				tail = myNode;
			}
			else {
				tail->next = myNode;
				myNode->prev = tail;
				myNode->next = head;
				head->prev = myNode;
				head = myNode;
			}
			size++;
		}
		void addFirst(T&& value) { // BigO(1)
			Node* myNode = new Node(static_cast<T&&>(value));
			if (isEmpty()) {
				myNode->next = myNode;
				myNode->prev = myNode;
				head = myNode;
				tail = myNode;
			}
			else {
				tail->next = myNode;
				myNode->prev = tail;
				myNode->next = head;
				head->prev = myNode;
				head = myNode;
			}
			size++;
		}
		void addLast(const T& value) { // BigO(1)
			Node* myNode = new Node(value);
			if (isEmpty()) {
				myNode->next = myNode;
				myNode->prev = myNode;
				head = myNode;
				tail = myNode;
			}
			else {
				tail->next = myNode;
				head->prev = myNode;
				myNode->prev = tail;
				myNode->next = head;
				tail = myNode;
			}
			size++;
		}
		void addLast(T&& value) { // BigO(1)
			Node* myNode = new Node(static_cast<T&&>(value));
			if (isEmpty()) {
				myNode->next = myNode;
				myNode->prev = myNode;
				head = myNode;
				tail = myNode;
			}
			else {
				tail->next = myNode;
				head->prev = myNode;
				myNode->prev = tail;
				myNode->next = head;
				tail = myNode;
			}
			size++;
		}
		void insert(const T& value, size_t index) { // BigO(n)
			if (index == 0) {
				addFirst(value);
			}
			else if (index == size) {
				addLast(value);
			}
			else {
				checkEmpty();
				checkAddBounds(index);
				Node* myNode = new Node(value);
				Node* target = getNodeAddress(index);
				target->prev->next = myNode;
				myNode->prev = target->prev;
				myNode->next = target;
				target->prev = myNode;
				size++;
			}
		}
		void insert(T&& value, size_t index) { // BigO(n)
			if (index == 0) {
				addFirst(static_cast<T&&>(value));
			}
			else if (index == size) {
				addLast(static_cast<T&&>(value));
			}
			else {
				checkEmpty();
				checkAddBounds(index);
				Node* myNode = new Node(static_cast<T&&>(value));
				Node* target = getNodeAddress(index);
				target->prev->next = myNode;
				myNode->prev = target->prev;
				myNode->next = target;
				target->prev = myNode;
				size++;
			}
		}
		const T& get(size_t index) const { //BigO(n)
			checkEmpty();
			checkAvailableBounds(index);
			return getNodeAddress(index)->value;
		}
		void set(const T& value, size_t index) { // BigO(n)
			checkEmpty();
			checkAvailableBounds(index);
			Node* temp = getNodeAddress(index);
			temp->value = value;
		}
		T removeFirst() { // BigO(1)
			checkEmpty();
			Node* trash = head;
			T returnValue = static_cast<T&&>(trash->value);
			if (size == 1) {
				head = nullptr;
				tail = nullptr;
			}
			else {
				head = head->next;
				tail->next = head;
				head->prev = tail;
			}
			delete trash;
			size--;
			return returnValue;
		}
		T removeLast() { // BigO(1)
			checkEmpty();
			Node* trash = tail;
			T returnValue = static_cast<T&&>(trash->value);
			if (size == 1) {
				head = nullptr;
				tail = nullptr;
			}
			else {
				tail = tail->prev;
				tail->next = head;
				head->prev = tail;
			}
			delete trash;
			size--;
			return returnValue;
		}
		T remove(size_t index) { // BigO(n)
			if (index == 0) {
				return removeFirst();
			}
			else if (index == size - 1) {
				return removeLast();
			}
			else {
				checkEmpty();
				checkAvailableBounds(index);
				Node* trash = getNodeAddress(index);
				trash->prev->next = trash->next;
				trash->next->prev = trash->prev;
				T returnValue = static_cast<T&&>(trash->value);
				delete trash;
				size--;
				return returnValue;
			}
		}
		void shuffle() { // BigO(n)
			if (isEmpty() || size == 1) { return; }
			std::mt19937& gen = mLib::getGlobalRNG();
			T** array = new T * [size];
			T** pin = array;
			try {
				for (auto& item : *this) {
					*array = &item;
					array++;
				}
				array -= size;
				for (size_t i = size - 1; i > 0; --i) {
					std::uniform_int_distribution<size_t> distr(0, i);
					size_t random = distr(gen);
					T move = static_cast<T&&>(*array[random]);
					*array[random] = static_cast<T&&>(*array[i]);
					*array[i] = static_cast<T&&>(move);
				}
			}
			catch (...) {
				delete[] pin;
				throw;
			}
			delete[] array;
		}
		void sort() { // @REFACTOR
			mLib::bubbleSort(*this);
		}
		template <typename Compare>
		void sort(Compare comp) {
			mLib::bubbleSort(*this, comp);
		}
		void swap(size_t index1, size_t index2) { // BigO(n)
			checkEmpty();
			checkAvailableBounds(index1);
			checkAvailableBounds(index2);
			if (index1 == index2) {
				return;
			}
			else {
				Node* temp1 = getNodeAddress(index1);
				Node* temp2 = getNodeAddress(index2);
				T move = static_cast<T&&>(temp1->value);
				temp1->value = static_cast<T&&>(temp2->value);
				temp2->value = static_cast<T&&>(move);
			}
		}
		const T& getRandom(size_t index1, size_t index2) const { // BigO(n)
			checkEmpty();
			checkAvailableBounds(index1);
			checkAvailableBounds(index2);
			if (index1 > index2) {
				throw std::invalid_argument("Invalid range: index1 (" + std::to_string(index1) + ") cannot be greater than index2 (" + std::to_string(index2) + ").");
			}
			std::mt19937& gen = mLib::getGlobalRNG();
			std::uniform_int_distribution<size_t> distr(index1, index2);
			return getNodeAddress(distr(gen))->value;
		}
		const T& getRandom() const { // BigO(n)
			checkEmpty();
			std::mt19937& gen = mLib::getGlobalRNG();
			std::uniform_int_distribution<size_t> distr(0, size - 1);
			return getNodeAddress(distr(gen))->value;
		}
		void clear() { // BigO(n)
			Node* temp = head;
			head = nullptr;
			tail = nullptr;
			for (size_t i = 0; i < size; i++) {
				Node* next = temp->next;
				delete temp;
				temp = next;
			}
			size = 0;
		}
		bool isEmpty() const noexcept{ return size == 0; }
		size_t getSize() const noexcept { return size; }
		class Iterator {
		public:
			Node* current;
			Node* finish;

			using iterator_category = std::bidirectional_iterator_tag;
			using value_type = T;
			using difference_type = std::ptrdiff_t;
			using reference = T&;
			using pointer = T*;

			Iterator(Node* ptr) : current(ptr), finish(nullptr){
				if (current != nullptr) {
					finish = ptr->prev;
				}
			}

			reference operator*() noexcept {
				return current->value;
			}
			pointer operator->() noexcept {
				return &(current->value);
			}
			Iterator& operator++() noexcept {
				if (current == finish) {
					current = nullptr;
				}
				else {
					current = current->next;
				}
				return *this;
			}
			Iterator operator++(int) noexcept {
				Iterator temp = *this;
				++(*this);
				return temp;
			}
			Iterator& operator--() noexcept {
				if (current == finish->next) {
					current = nullptr;
				}
				else {
					current = current->prev;
				}
				return *this;
			}
			Iterator operator--(int) noexcept {
				Iterator temp = *this;
				--(*this);
				return temp;
			}
			bool operator==(const Iterator& other) const noexcept{
				return this->current == other.current;
			}
			bool operator!=(const Iterator& other) const noexcept {
				return this->current != other.current;
			}
		};
		class Const_Iterator {
		public:
			const Node* current;
			const Node* finish;

			using iterator_category = std::bidirectional_iterator_tag;
			using value_type = T;
			using difference_type = std::ptrdiff_t;
			using reference = const T&;
			using pointer = const T*;

			Const_Iterator(const Node* ptr) : current(ptr), finish(nullptr) {
				if (current != nullptr) {
					finish = ptr->prev;
				}
			}

			reference operator*() const noexcept{
				return current->value;
			}
			pointer operator->() const noexcept{
				return &(current->value);
			}
			Const_Iterator& operator++() noexcept{
				if (current == finish) {
					current = nullptr;
				}
				else {
					current = current->next;
				}
				return *this;
			}
			Const_Iterator operator++(int) noexcept{
				Const_Iterator temp = *this;
				++(*this);
				return temp;
			}
			Const_Iterator& operator--() noexcept{
				if (current == finish->next) { 
					current = nullptr;         
				}
				else {
					current = current->prev;   
				}
				return *this;
			}
			Const_Iterator operator--(int) noexcept {
				Const_Iterator temp = *this;
				--(*this);
				return temp;
			}
			bool operator==(const Const_Iterator& other) const noexcept {
				return this->current == other.current;
			}
			bool operator!=(const Const_Iterator& other) const noexcept {
				return this->current != other.current;
			}
		};
		Iterator begin() noexcept {
			return Iterator(head);
		}
		Iterator end() noexcept {
			return Iterator(nullptr);
		}
		Const_Iterator begin() const noexcept {
			return Const_Iterator(head);
		}
		Const_Iterator end() const noexcept {
			return Const_Iterator(nullptr);
		}
		friend std::ostream& operator<<(std::ostream& os, const Cll<T>& list) { // BigO(n)
			if (list.isEmpty()) { os << "EMPTY"; }
			else {
				os << "TAIL <-> ";
				for (const auto& item : list) {
					os << item << " <-> ";
				}
				os << "HEAD";
			}
			return os;
		}
	};
}