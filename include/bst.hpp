#pragma once

#include <iostream>
#include <stdexcept>
#include <string>
#include <initializer_list> 
#include "debug.hpp" // not used
#include "utils.hpp"
#include "queue.hpp"
#include "stack.hpp"

namespace mLib {
	template <typename T>
	class Bst {
	private:
		struct Node {
			T value;
			Node* left;
			Node* right;

			Node(const T& value_param) : value(value_param), left(nullptr), right(nullptr){}

			Node(T&& value_param) noexcept : value(static_cast<T&&>(value_param)), left(nullptr), right(nullptr){}
			
		};
		Node* root;
		size_t size;
		void add(Node*& root, const T& value) { // BigO(n)
			if (!root) {
				root = new Node(value);
				size++;
				return;
			}
			else if (value >= root->value) {
				add(root->right, value);
			}
			else {
				add(root->left, value);
			}
		}
		void add(Node*& root, T&& value) { // BigO(n)
			if (!root) {
				root = new Node(static_cast<T&&>(value));
				size++;
				return;
			}
			else if (value >= root->value) {
				add(root->right, static_cast<T&&>(value));
			}
			else {
				add(root->left, static_cast<T&&>(value));
			}
		}

		T removeMin(Node*& root) { // BigO(n)
			if (!root) { throw std::invalid_argument("The minimum of the null pointer cannot be obtained."); }
			else if (!root->left) { 
				T min = static_cast<T&&>(root->value);
				if (root->right) {
					Node* right = root->right;
					delete root;
					root = right;
				}
				else {
					delete root;
					root = nullptr;
				}
				return min; 
			}
			else {
				return removeMin(root->left);
			}
		}

		void remove(Node*& root, const T& value) { // BigO(n)
			if (!root) { throw std::invalid_argument("Value is not found."); }
			else if (root->value < value) {
				remove(root->right, value);
			}
			else if (root->value > value) {
				remove(root->left, value);
			}
			else {
				if (!root->left and !root->right) {
					delete root;
					root = nullptr;
				}
				else if (!root->right) {
					Node* next = root->left;
					delete root;
					root = next;
				}
				else if (!root->left) {
					Node* next = root->right;
					delete root;
					root = next;
				}
				else {
					T temp = removeMin(root->right);
					root->value = static_cast<T&&>(temp);
				}
				size--;
			}
		}
		bool search(Node* root, const T& value) const noexcept { // BigO(n)
			if (!root) { return false; }
			else if(root->value < value){
				return search(root->right, value);
			}
			else if (root->value > value) {
				return search(root->left, value);
			}
			else {
				return true;
			}
		}
		void printInOrder(const Node* root) const noexcept { // BigO(n)
			if (!root) { return; }
			printInOrder(root->left);
			std::cout << root->value << " ";
			printInOrder(root->right);
		}
		void printPreOrder(const Node* root) const noexcept { // BigO(n)
			if (!root) { return; }
			std::cout << root->value << " ";
			printPreOrder(root->left);
			printPreOrder(root->right);
		}
		void printPostOrder(const Node* root) const noexcept { // BigO(n)
			if (!root) { return; }
			printPostOrder(root->left);
			printPostOrder(root->right);
			std::cout << root->value << " ";
		}
		void printLevelOrder(const Node* root) const noexcept {
			if (!root) { return; }
			mLib::Queue<const Node*> queue = { root };

			while (!queue.isEmpty()) {
				const Node* current = queue.dequeue();

				std::cout << current->value << " ";

				if (current->left) { queue.enqueue(current->left); }
				if (current->right) { queue.enqueue(current->right); }
			}
		}
		void clear(Node*& root) { // BigO(n)
			if (!root) { return; }
			clear(root->left);
			clear(root->right);
			delete root;
			size--;
			root = nullptr;
		}
		void copyTree(const Node* root) {
			if (!root) { return; }
			add(root->value);
			copyTree(root->left);
			copyTree(root->right);
		}
		long long getHeight(const Node* root) const noexcept {
			if (!root) { return -1; }
			return mLib::max(getHeight(root->left), getHeight(root->right)) + 1;
		}
		void addBalanced(mLib::Array<T>& arr, long long start, long long end) noexcept{
			if (start > end) { return; }

			long long mid = start + (end - start) / 2;

			add(static_cast<T&&>(arr[mid]));

			addBalanced(arr, start, mid - 1);
			addBalanced(arr, mid + 1, end);
		}
	public:

		Bst() : root(nullptr), size(0){}

		Bst(std::initializer_list<T> initList) : root(nullptr), size(0) {
			if (initList.size() == 0) { return; }
			mLib::Array<T> arr(initList);
			mLib::mergeSort(arr, mLib::less<T>{});
			addBalanced(arr, 0, static_cast<long long>(arr.getSize() - 1));
		}
		Bst(const Bst& other) : root(nullptr), size(0) { // BigO(n)
			copyTree(other.root);
		}

		Bst(Bst&& other) noexcept : root(other.root), size(other.size) { // BigO(1)
			other.root = nullptr;
			other.size = 0;
		}

		Bst& operator=(const Bst& other) { // BigO(n)
			if (this != &other) {
				Bst<T> temp(other);
				*this = static_cast<Bst&&>(temp);
			}
			return *this;
		}
		Bst& operator=(Bst&& other) noexcept {
			if (this != &other) {
				Bst<T> temp(static_cast<Bst&&>(other));

				Node* moveRoot = this->root;
				size_t moveSize = this->size;

				this->root = temp.root;
				this->size = temp.size;

				temp.root = moveRoot;
				temp.size = moveSize;
			}
			return *this;
		}

		void add(const T& value) {
			add(root, value);
		}
		void add(T&& value) {
			add(root, static_cast<T&&>(value));
		}
		void remove(const T& value) {
			if (size == 0) { throw std::underflow_error("Tree is Empty"); }
			remove(root, value);
		}
		bool search(const T& value) const noexcept {
			return search(root, value);
		}
		void printInOrder() const noexcept{
			if (!root) { std::cout << "EMPTY"; }
			else {
				printInOrder(root);
			}
			std::cout << std::endl;
		}
		void printPreOrder() const noexcept {
			if (!root) { std::cout << "EMPTY"; }
			else {
				printPreOrder(root);
			}
			std::cout << std::endl;
		}
		void printPostOrder() const noexcept {
			if (!root) { std::cout << "EMPTY"; }
			else {
				printPostOrder(root);
			}
			std::cout << std::endl;
		}
		void printLevelOrder() const noexcept {
			if (!root) { std::cout << "EMPTY"; }
			else {
				printLevelOrder(root);
			}
			std::cout << std::endl;
		}
		friend std::ostream& operator<<(std::ostream& os, const Bst& tree) {
			if (!tree.root) {
				os << "EMPTY";
			}
			else {
				tree.printInOrder();
			}
			return os << std::endl;
		}
		class const_iterator {
		private:
			const Node* current;
			mLib::Stack<const Node*> st;

			void step() {
				if (current->right) {
					const Node* temp = current->right;
					while (temp) {
						st.push(temp);
						temp = temp->left;
					}
				}
				if (!st.isEmpty()) {
					current = st.peek();
				}
				else {
					current = nullptr;
				}
			}
		public:
			using iterator_category = std::forward_iterator_tag;
			using value_type = T;
			using difference_type = std::ptrdiff_t;
			using reference = const T&;
			using pointer = const T*;


			const_iterator(const Node* root) : current(nullptr), st() {
				const Node* temp = root;
				while (temp) {
					st.push(temp);
					temp = temp->left;
				}
				if (!st.isEmpty()) {
					current = st.peek();
				}
			}

			reference operator*() const noexcept {
				return current->value;
			}
			pointer operator->() const noexcept {
				return &(current->value);
			}
			const_iterator& operator++() {
				if (!st.isEmpty()) {
					st.pop();
					step();
				}
				else {
					current = nullptr;
				}
				return *this;
			}
			const_iterator operator++(int) {
				const_iterator temp = *this;
				++(*this);
				return temp;
			}
			bool operator==(const const_iterator& other) const noexcept {
				return this->current == other.current;
			}
			bool operator!=(const const_iterator& other) const noexcept {
				return this->current != other.current;
			}
		};
		const_iterator begin() const noexcept {
			return const_iterator(root);
		}
		const_iterator end() const noexcept {
			return const_iterator(nullptr);
		}
		const_iterator cbegin() const noexcept { return begin(); }
		const_iterator cend() const noexcept { return end(); }
		void clear() {
			clear(root);
		}
		~Bst() {
			clear(root);
		}
		inline long long getHeight() const noexcept { return getHeight(root); }
		inline size_t getSize() const noexcept { return size; }
	};
}