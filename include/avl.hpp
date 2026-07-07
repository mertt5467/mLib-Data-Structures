#pragma once

#include <iostream>
#include <stdexcept>
#include <string>
#include <initializer_list>
#include "debug.hpp"
#include "utils.hpp"
#include "queue.hpp"
#include "stack.hpp"

namespace mLib {
	template <typename T>
	class Avl {
	private:
		struct Node {
			T value;
			Node* left;
			Node* right;
			int height;

			Node(const T& value_param) : value(value_param), left(nullptr), right(nullptr), height(0){}

			Node(T&& value_param) noexcept : value(static_cast<T&&>(value_param)), left(nullptr), right(nullptr), height(0){}

		};
		Node* root;
		size_t size;


		void add(Node*& root, const T& value) { // BigO(log(n))
			if (!root) {
				root = new Node(value);
				size++;
			}
			else if (root->value <= value) {
				add(root->right, value);
			}
			else {
				add(root->left, value);
			}
			root->height = mLib::max(getHeight(root->left), getHeight(root->right)) + 1;
			reBalance(root);
		}
		void add(Node*& root, T&& value) { // BigO(log(n))
			if (!root) {
				root = new Node(static_cast<T&&>(value));
				size++;
			}
			else if (root->value <= value) {
				add(root->right, static_cast<T&&>(value));
			}
			else {
				add(root->left, static_cast<T&&>(value));
			}
			root->height = mLib::max(getHeight(root->left), getHeight(root->right)) + 1;
			reBalance(root);
		}
		void remove(Node*& root, const T& value) { // BigO(log(n))
			if (!root) { throw std::invalid_argument("Value is not found."); }
			else if (root->value > value) {
				remove(root->left, value);
			}
			else if (root->value < value) {
				remove(root->right, value);
			}
			else {
				if (!root->left and !root->right) {
					delete root;
					root = nullptr;
				}
				else if (!root->left) {
					Node* next = root->right;
					delete root;
					root = next;
				}
				else if (!root->right) {
					Node* next = root->left;
					delete root;
					root = next;
				}
				else {
					T min = removeMin(root->right);
					root->value = static_cast<T&&>(min);
				}
				size--;
			}
			if (root) {
				root->height = mLib::max(getHeight(root->left), getHeight(root->right)) + 1;
				reBalance(root);
			}
		}
		T removeMin(Node*& root) { // BigO(log(n))
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
				T rtn =	removeMin(root->left);
				root->height = mLib::max(getHeight(root->left), getHeight(root->right)) + 1;
				reBalance(root);
				return rtn;
			}
		}
		bool search(Node* root, const T& value) const noexcept { // BigO(log(n))
			if (!root) { return false; }
			else if (root->value < value) {
				return search(root->right, value);
			}
			else if (root->value > value) {
				return search(root->left, value);
			}
			else {
				return true;
			}
		}
		void printInOrder(const Node* root, std::ostream& os) const noexcept { // BigO(n)
			if (!root) { return; }
			printInOrder(root->left, os);
			os << root->value << " ";
			printInOrder(root->right, os);
		}
		void printPreOrder(const Node* root, std::ostream& os) const noexcept { // BigO(n)
			if (!root) { return; }
			os << root->value << " ";
			printPreOrder(root->left, os);
			printPreOrder(root->right, os);
		}
		void printPostOrder(const Node* root, std::ostream& os) const noexcept { // BigO(n)
			if (!root) { return; }
			printPostOrder(root->left, os);
			printPostOrder(root->right, os);
			os << root->value << " ";
		}
		void printLevelOrder(const Node* root, std::ostream& os) const noexcept {
			if (!root) { return; }
			mLib::Queue<const Node*> queue = { root };

			while (!queue.isEmpty()) {
				const Node* current = queue.dequeue();

				os << current->value << " ";

				if (current->left) { queue.enqueue(current->left); }
				if (current->right) { queue.enqueue(current->right); }
			}
		}
		Node* copyTree(const Node* root) { // BigO(n)
			if (!root) { return nullptr; }
			Node* myNode = new Node(root->value);
			myNode->height = root->height;
			myNode->left = copyTree(root->left);
			myNode->right = copyTree(root->right);
			return myNode;
		}
		int getHeight(Node* root) const noexcept{
			return (!root) ? -1 : static_cast<int>(root->height);
		}
		int getBalance(Node* root) const noexcept{
			return (!root) ? 0 : getHeight(root->left) - getHeight(root->right);
		}
		void reBalance(Node*& root) {
			if (getBalance(root) == 2) {
				if (getBalance(root->left) >= 0) {
					rotateRight(root);
				}
				else {
					rotateLeft(root->left);
					rotateRight(root);
				}
			}
			else if (getBalance(root) == -2) {
				if (getBalance(root->right) <= 0) {
					rotateLeft(root);
				}
				else {
					rotateRight(root->right);
					rotateLeft(root);
				}
			}
		}
		void rotateRight(Node*& root) {
			if (!root) { return; }
			Node* temp = root->left;
			root->left = temp->right;
			temp->right = root;

			root->height = mLib::max(getHeight(root->left), getHeight(root->right)) + 1;
			temp->height = mLib::max(getHeight(temp->left), getHeight(temp->right)) + 1;

			root = temp;
		}
		void rotateLeft(Node*& root) {
			if (!root) { return; }
			Node* temp = root->right;
			root->right = temp->left;
			temp->left = root;

			root->height = mLib::max(getHeight(root->left), getHeight(root->right)) + 1;
			temp->height = mLib::max(getHeight(temp->left), getHeight(temp->right)) + 1;

			root = temp;
		}
		void clear(Node*& root) {
			if (!root) { return; }
			clear(root->left);
			clear(root->right);
			delete root;
			size--;
			root = nullptr;
		}
	public:

		Avl() : root(nullptr), size(0){}

		Avl(std::initializer_list<T> initList) : root(nullptr), size(0) {
			for (auto& item : initList) {
				add(item);
			}
		}
		Avl(const Avl& other) : root(nullptr), size(0) { // BigO(n)
			root = copyTree(other.root);
		}

		Avl(Avl&& other) noexcept : root(other.root), size(other.size) { // BigO(1)
			other.root = nullptr;
			other.size = 0;
		}

		Avl& operator=(const Avl& other) { // BigO(n)
			if (this != &other) {
				Avl<T> temp(other);
				*this = static_cast<Avl&&>(temp);
			}
			return *this;
		}
		Avl& operator=(Avl&& other) noexcept { // BigO(n)
			if (this != &other) {
				clear();
				root = other.root;
				size = other.size;

				other.root = nullptr;
				other.size = 0;
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
			remove(root, value);
		}
		void clear() {
			clear(root);
		}
		bool search(const T& value) const noexcept {
			return search(root, value);
		}
		void printInOrder(std::ostream& os = std::cout) const noexcept {
			if (!root) { os << "EMPTY"; }
			else {
				printInOrder(root, os);
			}
			os << std::endl;
		}
		void printPreOrder(std::ostream& os = std::cout) const noexcept {
			if (!root) { os << "EMPTY"; }
			else {
				printPreOrder(root, os);
			}
			os << std::endl;
		}
		void printPostOrder(std::ostream& os = std::cout) const noexcept {
			if (!root) { os << "EMPTY"; }
			else {
				printPostOrder(root, os);
			}
			os << std::endl;
		}
		void printLevelOrder(std::ostream& os = std::cout) const noexcept {
			if (!root) { os << "EMPTY"; }
			else {
				printLevelOrder(root, os);
			}
			os << std::endl;
		}
		friend std::ostream& operator<<(std::ostream& os, const Avl& tree) {
			tree.printInOrder(os);
			return os;
		}
		~Avl() {
			clear(root);
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

		inline size_t getSize() const noexcept { return size; }
		int getHeight() const noexcept { return getHeight(root); }
	};
}