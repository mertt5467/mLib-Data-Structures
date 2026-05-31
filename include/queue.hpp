#pragma once

#include <iostream>
#include <stdexcept>
#include <string>
#include <initializer_list>
#include "sll.hpp"

namespace mLib {
	template<typename T>
	class Queue {
	private:
		Sll<T> queue;
		long long cap;

		void checkOverflow() const{
			if (queue.getSize() == cap) { throw std::overflow_error("Queue overflow: maximum capacity reached. Capacity = " + std::to_string(cap)); }
		}

		void checkUnderflow() const{
			if (queue.getSize() == 0 or cap == 0) { throw std::underflow_error("Queue underflow: operation not allowed on an empty queue. Capacity = " + std::to_string(cap)); }
		}
	public:
		Queue() : cap(-1){}

		explicit Queue(size_t cap_param) : cap(static_cast<long long>(cap_param)){}

		Queue(const Queue& other) : queue(other.queue), cap(other.cap) {}

		Queue(Queue&& other) noexcept : queue(static_cast<Sll<T>&&>(other.queue)), cap(other.cap){
			other.cap = 0;
		}

		Queue(std::initializer_list<T> initList) : queue(initList), cap(-1) {}

		Queue& operator=(const Queue& other) { // BigO(n)
			if (this != &other) {
				Queue<T> temp(other);
				this->queue = static_cast<Sll<T>&&>(temp.queue);
				this->cap = temp.cap;
				temp.cap = 0;
			}
			return *this;
		}
		Queue& operator=(Queue&& other) noexcept{ // BigO(1)
			if (this != &other) {
				this->queue = static_cast<Sll<T>&&>(other.queue);
				this->cap = other.cap;
				other.cap = 0;
			}
			return *this;
		}

		~Queue() {
			cap = 0;
		}

		void enqueue(const T& value) { // BigO(1)
			checkOverflow();
			queue.addLast(value);
		}
		void enqueue(T&& value) { // BigO(1)
			checkOverflow();
			queue.addLast(static_cast<T&&>(value));
		}
		T dequeue() { // BigO(1)
			checkUnderflow();
			return queue.removeFirst();
		}
		T& front() { // BigO(1)
			checkUnderflow();
			return queue.front();
		}
		const T& front() const{ // BigO(1)
			checkUnderflow();
			return queue.front();
		}
		T& back() { // BigO(1)
			checkUnderflow();
			return queue.back();
		}
		const T& back() const{ // BigO(1)
			checkUnderflow();
			return queue.back();
		}
		void setCap(size_t cap) { // BigO(n)
			if (queue.getSize() >= cap) { throw std::invalid_argument("Invalid capacity : new capacity(" + std::to_string(cap) + ") cannot be less than or equal to the current queue size(" + std::to_string(queue.getSize()) + ")."); }
			this->cap = cap;
		}
		void clear() { // BigO(n)
			queue.clear();
		}
		friend std::ostream& operator<<(std::ostream& os, const Queue& list) {
			if (list.isEmpty()) { os << "NULL"; }
			else {
				size_t i = list.getSize();
				os << "FRONT | ";
				for (auto it = list.queue.begin(); it != list.queue.end(); ++it) {
					os << *it;
					i--;
					if (i > 0) {
						os << " <- ";
					}
				}
				os << " | BACK";
			}
			return os;
		}
		inline bool isEmpty() const noexcept { return queue.getSize() == 0; }
		inline size_t getSize() const noexcept { return queue.getSize(); }
		inline long long getCap() const noexcept { return cap; }
	};
}