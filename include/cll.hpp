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

		Node* getNodeAddress(size_t index) noexcept{
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
		const Node* getNodeAddress(size_t index) const noexcept {
			return const_cast<Cll<T>*>(this)->getNodeAddress(index);
		}

	public:

		Cll() : head(nullptr), tail(nullptr), size(0){}

		Cll(const Cll& other) : head(nullptr), tail(nullptr), size(0) {
			// @TODO: WRITE ME !!!
		}
		Cll(Cll&& other) noexcept : head(other.head), tail(other.tail), size(other.size) {
			other.head = nullptr;
			other.tail = nullptr;
			other.size = 0;
		}

		Cll& operator=(const Cll& other) {
			// @TODO: WRITE ME !!!
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
		void addFirst(const T& value) {
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
		void addFirst(T&& value) {
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
		void addLast(const T& value) {
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
		void addLast(T&& value) {
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
		void insert(const T& value, size_t index) {
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
		void insert(T&& value, size_t index) {
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
		const T& get(size_t index) const {
			checkEmpty();
			checkAvailableBounds(index);
			return getNodeAddress(index)->value;
		}
		void clear() {
			Node* temp = head;
			head = nullptr;
			tail = nullptr;
			for (int i = 0; i < size; i++) {
				Node* next = temp->next;
				delete temp;
				temp = next;
			}
			size = 0;
		}
		bool isEmpty() const noexcept{ return size == 0; }
		size_t getSize() const noexcept { return size; }
	};
}