#pragma once

#include <iostream>
#include <stdexcept>
#include <string>
#include <initializer_list>
#include "list.hpp"

namespace mLib {
	template <typename T>
	class MinHeap{
	private:
		List<T> heap;
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
				if (heap.get(parentIndex) > heap.get(index)) {
					heap.swap(parentIndex, index);
					heapifyUp(parentIndex);
				}
		}
		void heapifyDown(size_t index) noexcept{ // BigO(log(n))
			size_t left = getLeftChildIndex(index);
			size_t right = getRightChildIndex(index);
			size_t smallest = index;

			if (checkIndex(left) and heap.get(smallest) > heap.get(left)) {
				smallest = left;
			}
			if (checkIndex(right) and heap.get(smallest) > heap.get(right)) {
				smallest = right;
			}

			if (smallest != index) {
				heap.swap(smallest, index);
				heapifyDown(smallest);
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
		MinHeap() : cap(-1){}

		explicit MinHeap(size_t cap_param) : heap(cap_param), cap(static_cast<long long>(cap_param)){}

		MinHeap(std::initializer_list<T> initList) : heap(initList), cap(-1) { // BigO(n) Floyd's Algorithm
			if (!heap.isEmpty()) {
				for (long long i = (heap.getSize() / 2) - 1; i >= 0; --i) {
					heapifyDown(static_cast<size_t>(i));
				}
			}
		}

		MinHeap(const MinHeap& other) : heap(other.heap), cap(other.cap){} // BigO(n)

		MinHeap(MinHeap&& other) noexcept : heap(static_cast<List<T>&&>(other.heap)), cap(other.cap) { // BigO(1)
			other.cap = 0;
		}

		MinHeap& operator=(const MinHeap& other) { // BigO(n)
			if (this != &other) {
				heap = other.heap;
				cap = other.cap;
			}
			return *this;
		}
		MinHeap& operator=(MinHeap&& other) noexcept{ // BigO(1)
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
		friend std::ostream& operator<<(std::ostream& os, const MinHeap& heap) { // BigO(n)
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
		~MinHeap() {
			cap = 0;
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
		bool isEmpty() const noexcept { return heap.isEmpty(); }
		size_t getSize() const noexcept { return heap.getSize(); }
		long long getCap() const noexcept { return cap; }
	};
}