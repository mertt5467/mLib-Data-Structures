#pragma once

#include <iostream>
#include <stdexcept>
#include <string>
#include <initializer_list>
#include "list.hpp"

namespace mLib {
	template<typename T>
	class Stack {
	private:
		List<T> stack;
		long long cap;

		void checkOverflow() const{
			if (stack.getSize() == cap) { throw std::overflow_error("Stack overflow: maximum capacity reached. Capacity = " + std::to_string(cap)); }
		}
		void checkUnderflow() const{
			if (stack.getSize() == 0 or cap == 0) { throw std::underflow_error("Stack underflow: operation not allowed on an empty stack. Capacity = " + std::to_string(cap)); }
		}
	public:
		Stack() : cap(-1){}
		explicit Stack(size_t cap_param) : stack(cap_param), cap(static_cast<long long>(cap_param)){}
		~Stack() {
			cap = 0;
		}
		Stack(const Stack& other) : stack(other.stack), cap(other.cap){}
		Stack(Stack&& other) noexcept: stack(static_cast<List<T>&&>(other.stack)), cap(other.cap) { // BigO(1)
			other.cap = 0;
		}
		Stack(std::initializer_list<T> initList) : stack(initList), cap(-1) {}
		Stack& operator=(const Stack& other) {
			if (this != &other) {
				stack = other.stack;
				cap = other.cap;
			}
			return *this;
		}
		Stack& operator=(Stack&& other) noexcept{ // BigO(1)
			if (this != &other) {
				stack = static_cast<List<T>&&>(other.stack);
				cap = other.cap;

				other.cap = 0;
			}
			return *this;
		}
		void push(const T& value) { // BigO(1), but if capacity has not been determined -> (Amortized (1))
			checkOverflow();
			stack.add(value);
		}
		void push(T&& value) { // BigO(1), but if capacity has not been determined -> (Amortized (1))
			checkOverflow();
			stack.add(static_cast<T&&>(value));
		}
		T pop() { // BigO(1)
			checkUnderflow();
			return stack.remove();
		}
		const T& peek() const { // BigO(1)
			checkUnderflow();
			return stack.back();
		}
		T& peek() { // BigO(1)
			checkUnderflow();
			return stack.back();
		}
		void clear() { // BigO(n)
			stack.clear();
		}
		void setCap(size_t cap) { // BigO(n)
			if (stack.getSize() >= cap) { throw std::invalid_argument("Invalid capacity : new capacity(" + std::to_string(cap) + ") cannot be less than or equal to the current stack size(" + std::to_string(stack.getSize()) + ")."); }
			stack.reserve(cap);
			this->cap = cap;
		}
		friend std::ostream& operator<<(std::ostream& os, const Stack<T>& other) { // BigO(n)
			if (other.getSize() == 0) { os << "EMPTY"; }
			else {
				os << std::endl;
				os << "\t|OVER|" << std::endl;
				for (long long i = other.stack.getSize() - 1; i >= 0; --i) {
					os << other.stack[i] << std::endl;
				}
				os << "\t|UNDER|" << std::endl;
			}
			return os;
		}
		inline bool isEmpty() const noexcept { return stack.getSize() == 0; }
		inline size_t getSize() const noexcept { return stack.getSize(); }
		inline long long getCap() const noexcept { return cap; }
		inline bool isFull() const noexcept { return static_cast<long long>(stack.getSize()) == cap; }
	};
}