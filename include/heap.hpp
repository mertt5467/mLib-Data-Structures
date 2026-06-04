#pragma once

#include <iostream>
#include <stdexcept>
#include <string>
#include <initializer_list>
#include "list.hpp"
#include "utils.hpp"

namespace mLib {
	template <typename T, typename Compare = mLib::less<T>>
	class Heap{
	private:
		List<T> heap;
		Compare comp;
		long long cap;

		size_t getParentIndex(size_t index) const noexcept{
			return (index - 1) / 2;
		}
		size_t getLeftChildIndex(size_t index) const noexcept{
			return index * 2 + 1;
		}
		size_t getRightChildIndex(size_t index) const noexcept{
			return index * 2 + 2;
		}
		void checkOverflow() const{ 
			if (heap.getSize() == cap) {
				throw std::overflow_error("Heap overflow: maximum capacity reached. Capacity = " + std::to_string(cap));
			}
		}
		void checkUnderflow() const{ 
			if (heap.isEmpty() or cap == 0) {
				throw std::underflow_error("Heap underflow: operation not allowed on an empty heap. Capacity = " + std::to_string(cap));
			}
		}
		void heapifyUp(size_t index) noexcept{ // BigO(log(n))
			if (index == 0) { return; }
			size_t parentIndex = getParentIndex(index);
				if (comp(heap.get(index), heap.get(parentIndex))) {
					heap.swap(parentIndex, index);
					heapifyUp(parentIndex);
				}
		}
		void heapifyDown(size_t index) noexcept{ // BigO(log(n))
			size_t left = getLeftChildIndex(index);
			size_t right = getRightChildIndex(index);
			size_t pin = index;

			if (checkIndex(left) and comp(heap.get(left), heap.get(pin))) {
				pin = left;
			}
			if (checkIndex(right) and comp(heap.get(right), heap.get(pin))) {
				pin = right;
			}
			if (pin != index) {
				heap.swap(pin, index);
				heapifyDown(pin);
			}
		}
		bool checkIndex(size_t index) const noexcept{
			return index < heap.getSize();
		}
		const T* begin() const{
			return heap.begin();
		}
		const T* end() const {
			return heap.end();
		}
	public:
		Heap() : cap(-1){}

		explicit Heap(size_t cap_param) : heap(cap_param), cap(static_cast<long long>(cap_param)){}

		Heap(std::initializer_list<T> initList) : heap(initList), cap(-1) { // BigO(n) Floyd's Algorithm
			if (!heap.isEmpty()) {
				for (long long i = (heap.getSize() / 2) - 1; i >= 0; --i) {
					heapifyDown(static_cast<size_t>(i));
				}
			}
		}

		Heap(const Heap& other) : heap(other.heap), cap(other.cap){} // BigO(n)

		Heap(Heap&& other) noexcept : heap(static_cast<List<T>&&>(other.heap)), cap(other.cap) { // BigO(1)
			other.cap = 0;
		}

		Heap& operator=(const Heap& other) { // BigO(n)
			if (this != &other) {
				heap = other.heap;
				cap = other.cap;
			}
			return *this;
		}
		Heap& operator=(Heap&& other) noexcept{ // BigO(1)
			if (this != &other) {
				heap = static_cast<List<T>&&>(other.heap);
				cap = other.cap;

				other.cap = 0;
			}
			return *this;
		}
		void add(const T& value) { // BigO(log(n))
			checkOverflow();
			heap.add(value);
			heapifyUp(heap.getSize() - 1);
		}
		void add(T&& value) { // BigO(log(n))
			checkOverflow();
			heap.add(static_cast<T&&>(value));
			heapifyUp(heap.getSize() - 1);
		}
		T remove() { // BigO(log(n))
			checkUnderflow();
			heap.swap(0, heap.getSize() - 1);
			T temp = heap.remove();

			if(!isEmpty()) {
				heapifyDown(0);
			}

			return temp;
		}

		const T& get() const{ // BigO(1)
			checkUnderflow();
			return heap[0];
		}

		T& get() { // BigO(1)
			checkUnderflow();
			return heap[0];
		}

		void setCap(size_t cap) { // BigO(n)
			if (heap.getSize() >= cap) { throw std::invalid_argument("Invalid capacity : new capacity(" + std::to_string(cap) + ") cannot be less than or equal to the current heap size(" + std::to_string(heap.getSize()) + ")."); }
			heap.reserve(cap);
			this->cap = cap;
		}
		void clear() { // BigO(n)
			heap.clear();
		}
		friend std::ostream& operator<<(std::ostream& os, const Heap& heap) { // BigO(n)
			if (heap.isEmpty()) { os << "EMPTY"; }
			else {
				size_t i = heap.getSize();
				os << "OUT <- ";
				for (auto it = heap.begin(); it != heap.end(); ++it) {
					os << *it;
					i--;
					if (i > 0) {
						os << " <- ";
					}
				}
				os << " <- IN";
			}
			return os;
		}
		~Heap() {
			cap = 0;
		}
		void printConsole() const { // BigO(n * log(n))
			if (isEmpty()) {
				std::cout << "EMPTY" << std::endl;
			}
			else {
				Heap<T, Compare> temp(*this);
				while (!temp.isEmpty()) {
					std::cout << temp.remove() << std::endl;
				}
			}
		}
		void printConsoleBottomUp() const { // BigO(n * log(n))
			if (isEmpty()) {
				std::cout << "EMPTY" << std::endl;
			}
			else {
				Heap<T, Compare> temp(*this);
				Stack<T> reverse(temp.getSize() + 1);
				while (!temp.isEmpty()) {
					reverse.push(temp.remove());
				}
				while (!reverse.isEmpty()) {
					std::cout << reverse.pop() << std::endl;
				}
			}
		}
		T& front() { // BigO(1)
			checkUnderflow();
			return heap.front();
		}
		const T& front() const { // BigO(1)
			checkUnderflow();
			return heap.front();
		}
		T& back() { // BigO(1)
			checkUnderflow();
			return heap.back();
		}
		const T& back() const { // BigO(1)
			checkUnderflow();
			return heap.back();
		}
		inline bool isEmpty() const noexcept { return heap.isEmpty(); }
		inline size_t getSize() const noexcept { return heap.getSize(); }
		inline long long getCap() const noexcept { return cap; }
		inline bool isFull() const noexcept { return static_cast<long long>(heap.getSize()) == cap; }
	};
}