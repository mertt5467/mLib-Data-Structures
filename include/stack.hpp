#pragma once

#include <iostream>
#include <stdexcept>
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
			if (stack.getSize() == 0) { throw std::underflow_error("Stack underflow: operation not allowed on an empty stack."); }
		}
	public:
		Stack() : cap(-1){}
		explicit Stack(size_t cap_param) : stack(cap_param), cap(static_cast<long long>(cap_param)){}
		~Stack() {
			cap = 0;
		}
		Stack(const Stack& other) = delete;
		Stack(Stack&& other) noexcept: stack(static_cast<List<T>&&>(other.stack)), cap(other.cap) {
			other.cap = 0;
		}
		Stack& operator=(const Stack& other) = delete;
		Stack& operator=(Stack&& other) noexcept{
			if (this != &other) {
				this->stack = static_cast<List<T>&&>(other.stack);
				this->cap = other.cap;
				other.cap = 0;
			}
			return *this;
		}
		void push(const T& value) {
			checkOverflow();
			stack.add(value);
		}
		void push(T&& value) {
			checkOverflow();
			stack.add(static_cast<T&&>(value));
		}
		T pop() {
			checkUnderflow();
			return stack.remove();
		}
		const T& peek() const {
			checkUnderflow();
			return stack.back();
		}
		T& peek() {
			checkUnderflow();
			return stack.back();
		}
		void clear() {
			stack.clear();
		}
		friend std::ostream& operator<<(std::ostream& os, const Stack<T>& other) {
			if (other.getSize() == 0) { os << "EMPTY"; }
			else {
				os << std::endl;
				os << "\t|OVER|" << std::endl;
				for (auto it = other.stack.end() - 1; it >= other.stack.begin(); --it) {
					os << *it << std::endl;
				}
				os << "\t|UNDER|" << std::endl;
			}
			return os;
		}
		inline bool isEmpty() const noexcept { return stack.getSize() == 0; }
		inline size_t getSize() const noexcept { return stack.getSize(); }
		inline long long getCap() const noexcept { return cap; }
	};
}