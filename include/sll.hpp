#pragma once

#include <iostream>
#include <string>
#include <stdexcept>
#include <random>
#include <initializer_list>
#include "debug.hpp" // @TODO: not used, write once
namespace mLib {
    template<typename T>
    class Sll {
    private:
        struct Node {
            T value;
            Node* next;

            Node(const T& value) : value(value), next(nullptr) {}

            Node(T&& value) noexcept : value(static_cast<T&&>(value)), next(nullptr) {}

            Node(Node&& other) = delete;

            Node(const Node& other) = delete;

            Node& operator=(Node&& other) = delete;

            Node& operator=(const Node& other) = delete;
        };
        Node* head;
        int size;
        void clear() noexcept { // BigO(n)
            Node* temp = head;
            head = nullptr;
            while (temp != nullptr) {
                Node* next = temp->next;
                delete temp;
                temp = next;

            }
        }
        Node* getNodeAddress(int index) noexcept { // BigO(n)
            Node* temp = head;
            for (int i = 0; i < index; i++) {
                temp = temp->next;
            }
            return temp;
        }
        const Node* getNodeAddress(int index) const { // BigO(n)
            return const_cast<Sll*>(this)->getNodeAddress(index);
        }
        void checkEmpty() const {
            if (isEmpty()) { throw std::out_of_range("List is empty."); }
        }
        void checkAvailableBounds(int index) const {
            if (index < 0 || index >= size) { throw std::out_of_range("Index out of bounds. Size = " + std::to_string(size)); }
        }
        void checkAddBounds(int index) const {
            if (index < 0 || index > size) { throw std::out_of_range("Index out of bounds. Size = " + std::to_string(size)); }
        }
    public:
        Sll() : head(nullptr), size(0) {}
        Sll(const Sll& other) : head(nullptr), size(0) { // BigO(n)
            try {
                if (other.isEmpty()) { return; }
                Node* temp1 = other.head;
                Node* temp2 = new Node(temp1->value);
                temp1 = temp1->next;
                head = temp2;
                size++;
                while (temp1 != nullptr) {
                    temp2->next = new Node(temp1->value);
                    temp1 = temp1->next;
                    temp2 = temp2->next;
                    size++;
                }
            }
            catch (...) {
                clear();
                throw;
            }
        }
        Sll(Sll&& other) noexcept : head(other.head), size(other.size) { // BigO(1)
            other.head = nullptr;
            other.size = 0;
        }
        Sll(std::initializer_list<T> initList) : head(nullptr), size(0) {
            // @TODO: WRITE ME!!!
        }
        Sll& operator=(const Sll& other) { // BigO(n)
            if (this != &other) {
                Sll temp(other);
                clear();
                head = temp.head;
                size = temp.size;
                temp.head = nullptr;
                temp.size = 0;
            }
            return *this;
        }
        Sll& operator=(Sll&& other) noexcept { // BigO(1)
            if (this != &other) {
                clear();
                head = other.head;
                size = other.size;
                other.head = nullptr;
                other.size = 0;
            }
            return *this;
        }
        bool isEmpty() const noexcept { return size == 0; }
        int getSize() const noexcept { return size; }
        ~Sll() {
            clear();
        }
        void addLast(const T& value) { // BigO(n)
            if (isEmpty()) {
                head = new Node(value);
            }
            else {
                Node* temp = getNodeAddress(size - 1);
                temp->next = new Node(value);
            }
            size++;
        }
        void addLast(T&& value) { // BigO(n)
            if (isEmpty()) {
                head = new Node(static_cast<T&&>(value));
            }
            else {
                Node* temp = getNodeAddress(size - 1);
                temp->next = new Node(static_cast<T&&>(value));
            }
            size++;
        }
        void addFirst(const T& value) { // BigO(1)
            Node* temp = new Node(value);
            temp->next = head;
            head = temp;
            size++;
        }
        void addFirst(T&& value) { // BigO(1)
            Node* temp = new Node(static_cast<T&&>(value));
            temp->next = head;
            head = temp;
            size++;
        }
        void insert(int index, const T& value) { // BigO(n)
            if (index == 0) {
                addFirst(value);
            }
            else {
                checkAddBounds(index);
                Node* temp = getNodeAddress(index - 1);
                Node* myNode = new Node(value);
                myNode->next = temp->next;
                temp->next = myNode;
                size++;
            }
        }
        void insert(int index, T&& value) { // BigO(n)
            if (index == 0) {
                addFirst(static_cast<T&&>(value));
            }
            else {
                checkAddBounds(index);
                Node* temp = getNodeAddress(index - 1);
                Node* myNode = new Node(static_cast<T&&>(value));
                myNode->next = temp->next;
                temp->next = myNode;
                size++;
            }
        }
        const T& get(int index) const { // BigO(n)
            checkEmpty();
            checkAvailableBounds(index);
            const Node* temp = getNodeAddress(index);
            return temp->value;
        }
        T removeFirst() { // BigO(1)
            checkEmpty();
            Node* trash = head;
            head = head->next;
            T returnValue = static_cast<T&&>(trash->value);
            delete trash;
            size--;
            return returnValue;
        }
        T removeLast() { // BigO(n)
            checkEmpty();
            Node* trash = nullptr;
            if (size == 1) {
                trash = head;
                head = nullptr;
            }
            else {
                Node* temp = getNodeAddress(size - 2);
                trash = temp->next;
                temp->next = trash->next;
            }
            T returnValue = static_cast<T&&>(trash->value);
            delete trash;
            size--;
            return returnValue;
        }
        T remove(int index) { // BigO(n)
            if (index == 0) {
                return removeFirst();
            }
            checkEmpty();
            checkAvailableBounds(index);
            Node* temp = getNodeAddress(index - 1);
            Node* trash = temp->next;
            temp->next = trash->next;
            T returnValue = static_cast<T&&>(trash->value);
            delete trash;
            size--;
            return returnValue;
        }
        void set(int index, const T& value) { // BigO(n)
            checkEmpty();
            checkAvailableBounds(index);
            Node* temp = getNodeAddress(index);
            temp->value = value;
        }
        void set(int index, T&& value) { // BigO(n)
            checkEmpty();
            checkAvailableBounds(index);
            Node* temp = getNodeAddress(index);
            temp->value = static_cast<T&&>(value);
        }
        void swap(int index1, int index2) { // BigO(n)
            checkEmpty();
            checkAvailableBounds(index1);
            checkAvailableBounds(index2);
            if (index1 == index2) {
                return;
            }
            else {
                Node* temp1 = getNodeAddress(index1);
                Node* temp2 = getNodeAddress(index2);
                T move = static_cast<T&&>(temp1->value);
                temp1->value = static_cast<T&&>(temp2->value);
                temp2->value = static_cast<T&&>(move);
            }
        }
        int indexOf(const T& value) const noexcept { // BigO(n)
            Node* temp = head;
            for (int i = 0; i < size; i++) {
                if (value == temp->value) {
                    return i;
                }
                temp = temp->next;
            }
            return -1;
        }
        class Iterator {
        private:
            Node* current;
        public:
            using iterator_category = std::forward_iterator_tag;
            using value_type = T;
            using difference_type = std::ptrdiff_t;
            using pointer = T*;
            using reference = T&;

            Iterator(Node* ptr) noexcept : current(ptr) {}

            reference operator*() const noexcept {
                return current->value;
            }
            pointer operator->() const noexcept {
                return &(current->value);
            }
            Iterator& operator++() noexcept {
                if (current != nullptr) {
                    current = current->next;
                }
                return *this;
            }
            Iterator operator++(int) noexcept {
                Iterator temp = *this;
                ++(*this);
                return temp;
            }
            bool operator==(const Iterator& other) const noexcept {
                return this->current == other.current;
            }
            bool operator!=(const Iterator& other) const noexcept {
                return this->current != other.current;
            }
        };
        class Const_Iterator {
        private:
            const Node* current;
        public:
            using iterator_category = std::forward_iterator_tag;
            using value_type = T;
            using difference_type = std::ptrdiff_t;
            using pointer = const T*;
            using reference = const T&;

            Const_Iterator(const Node* ptr) noexcept : current(ptr) {}

            reference operator*() const noexcept {
                return current->value;
            }
            pointer operator->() const noexcept {
                return &(current->value);
            }
            Const_Iterator& operator++() noexcept {
                if (current != nullptr) {
                    current = current->next;
                }
                return *this;
            }
            Const_Iterator operator++(int) noexcept {
                Const_Iterator temp = *this;
                ++(*this);
                return temp;
            }
            bool operator==(const Const_Iterator& other) const noexcept {
                return this->current == other.current;
            }
            bool operator!=(const Const_Iterator& other) const noexcept {
                return this->current != other.current;
            }
        };
        Iterator begin() noexcept {
            return Iterator(head);
        }
        Iterator end() noexcept {
            return Iterator(nullptr);
        }
        Const_Iterator begin() const noexcept {
            return Const_Iterator(head);
        }
        Const_Iterator end() const noexcept {
            return Const_Iterator(nullptr);
        }
        T& front() { // BigO(1)
            checkEmpty();
            return head->value;
        }
        T& back() { // BigO(n)
            checkEmpty();
            return (getNodeAddress(size - 1))->value;
        }
        const T& front() const { // BigO(1)
            checkEmpty();
            return head->value;
        }
        const T& back() const { // BigO(n)
            checkEmpty();
            return (getNodeAddress(size - 1))->value;
        }
    };
}