#pragma once

#include <iostream>
#include <stdexcept>
#include <string>
#include <initializer_list> // not used
#include "debug.hpp" // not used

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
		void add(Node*& root, const T& value) {
			if (!root) {
				root = new Node(value);
				return;
			}
			else if (value >= root->value) {
				add(root->right, value);
			}
			else {
				add(root->left, value);
			}
		}
		void add(Node*& root, T&& value) {
			if (!root) {
				root = new Node(static_cast<T&&>(value));
				return;
			}
			else if (value >= root->value) {
				add(root->right, static_cast<T&&>(value));
			}
			else {
				add(root->left, static_cast<T&&>(value));
			}
		}

		T removeMin(Node*& root) {
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

		void remove(Node*& root, const T& value) {
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
			}
		}
		bool search(Node* root, const T& value) const noexcept {
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
		void printInOrder(const Node* root) const noexcept {
			if (!root) { return; }
			printInOrder(root->left);
			std::cout << root->value << " ";
			printInOrder(root->right);
		}
		void clear(Node*& root) {
			if (!root) { return; }
			clear(root->left);
			clear(root->right);
			delete root;
			root = nullptr;
		}
	public:

		Bst() : root(nullptr) {}

		void add(const T& value) {
			add(root, value);
		}
		void add(T&& value) {
			add(root, static_cast<T&&>(value));
		}
		void remove(const T& value) {
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
		}
		friend std::ostream& operator<<(std::ostream& os, const Bst& tree) {
			if (!tree.root) {
				os << "EMPTY";
			}
			else {
				tree.printInOrder(tree.root);
			}
			return os << std::endl;
		}
		void clear() {
			clear(root);
		}
		~Bst() {
			clear(root);
		}
	};
}