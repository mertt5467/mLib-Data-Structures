#pragma once

#include <iostream>
#include <stdexcept>
#include <string>
#include <initializer_list>
#include "heap.hpp"
#include "utils.hpp"
#include "stack.hpp"

namespace mLib {
	template<typename T, typename P = double, typename Compare = mLib::less<P>>
	class PriorityQueue {
	private:
		struct Node {
			T value;
			P priority;

			Node(const T& value_param, P priority_param) : value(value_param), priority(priority_param){}

			Node(T&& value_param, P priority_param) noexcept: value(static_cast<T&&>(value_param)), priority(priority_param){}

			friend std::ostream& operator<<(std::ostream& os, const Node& node) {
				os << "{ " << node.value << " - Priority : " << node.priority << " }";
				return os;
			}
		};
		struct NodeCompare {
			Compare comp;
			constexpr bool operator()(const Node& l, const Node& r) const{
				return comp(l.priority, r.priority);
			}
		};
		void checkOverflow() const {
			if (queue.getSize() == queue.getCap()) {
				throw std::overflow_error("Queue overflow: maximum capacity reached. Capacity = " + std::to_string(queue.getCap()));
			}
		}
		void checkUnderflow() const {
			if (queue.isEmpty() or queue.getCap() == 0) {
				throw std::underflow_error("Queue underflow: operation not allowed on an empty heap. Capacity = " + std::to_string(queue.getCap()));
			}
		}
		Heap<Node, NodeCompare> queue;
	public:
		PriorityQueue() : queue(){}

		explicit PriorityQueue(size_t cap_param) : queue(static_cast<long long>(cap_param)){}

		PriorityQueue(std::initializer_list<mLib::pair<T, P>> initList) : queue(-1){ // BigO(n)
			for (const auto& item : initList) {
				enqueue(item.first, item.second);
			}
		}

		void enqueue(const T& value, P priority) { // BigO(log(n))
			checkOverflow();
			queue.add(Node(value, priority));
		}

		void enqueue(T&& value, P priority) { // BigO(log(n))
			checkOverflow();
			queue.add(Node(static_cast<T&&>(value), priority));
		}
		T dequeue() { // BigO(log(n))
			checkUnderflow();
			return queue.remove().value;
		}
		T& front() { // BigO(1)
			checkUnderflow();
			return queue.front().value;
		}
		const T& front() const { // BigO(1)
			checkUnderflow();
			return queue.front().value;
		}
		T& back() { // BigO(1)
			checkUnderflow();
			return queue.back().value;
		}
		const T& back() const { // BigO(1)
			checkUnderflow();
			return queue.back().value;
		}
		void setCap(size_t cap) { // BigO(n)
			checkUnderflow();
			queue.setCap(cap);
		}
		void printConsole() const { // BigO(n * log(n))
			if (isEmpty()) {
				std::cout << "EMPTY" << std::endl;
			}
			else {
				PriorityQueue<T, P, Compare> temp(*this);
				while (!temp.isEmpty()) {
					std::cout << temp.dequeue() << std::endl;
				}
			}
		}
		void printConsoleBottomUp() const { // BigO(n * log(n))
			if (isEmpty()) {
				std::cout << "EMPTY" << std::endl;
			}
			else {
				PriorityQueue<T, P, Compare> temp(*this);
				Stack<T> reverse(temp.getSize() + 1);
				while (!temp.isEmpty()) {
					reverse.push(temp.dequeue());
				}
				while (!reverse.isEmpty()) {
					std::cout << reverse.pop() << std::endl;
				}
			}
		}
		friend std::ostream& operator<<(std::ostream& os, const PriorityQueue& pqueue) { // BigO(n)
			return os << pqueue.queue;
		}
		inline long long getCap() const noexcept { return queue.getCap(); }
		inline size_t getSize() const noexcept { return queue.getSize(); }
		inline bool isEmpty() const noexcept { return queue.isEmpty(); }
		inline bool isFull() const noexcept { return static_cast<long long>(queue.getSize()) == queue.getCap(); }
	};
}