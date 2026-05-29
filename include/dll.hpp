#pragma once

#include <iostream>
#include <string>
#include <stdexcept>
#include <random>
#include <initializer_list>
#include "mergeSort.hpp"
#include "debug.hpp" // @TODO: not used, write once
namespace mLib {
    template<typename T>
    class Dll {
    private:
        struct Node {
            T value;
            Node* next;
            Node* prev;
            Node(const T& value) noexcept : value(value), next(nullptr), prev(nullptr) {}

            Node(T&& value) noexcept : value(static_cast<T&&>(value)), next(nullptr), prev(nullptr) {}

            Node(Node&& other) = delete;
            Node(const Node& other) = delete;
            Node& operator=(Node&& other) = delete;
            Node& operator=(const Node& other) = delete;
        };
        Node* head;
        Node* tail;
        size_t size;
        Node* getNodeAddress(size_t index) { // BigO(n)
            Node* temp = nullptr;
            if (index < size / 2) {
                temp = head;
                for (size_t i = 0; i < index; i++) {
                    temp = temp->next;
                }
            }
            else {
                temp = tail;
                for (size_t i = size - 1; i > index; i--) {
                    temp = temp->prev;
                }
            }
            return temp;
        }
        const Node* getNodeAddress(size_t index) const { // BigO(n)
            return const_cast<Dll*>(this)->getNodeAddress(index);
        }
        void checkEmpty() const {
            if (isEmpty()) { throw std::out_of_range("List is empty."); }
        }
        void checkAvailableBounds(size_t index) const {
            if (index >= size) { throw std::out_of_range("Index out of bounds. Size = " + std::to_string(size)); }
        }
        void checkAddBounds(size_t index) const {
            if (index > size) { throw std::out_of_range("Index out of bounds. Size = " + std::to_string(size)); }
        }
    public:
        ~Dll() {
            clear();
        }
        Dll() : head(nullptr), tail(nullptr), size(0) {}
        Dll(const Dll& other) : head(nullptr), tail(nullptr), size(0) {
            for (const auto& item : other) {
                addLast(item);
            }
        }
        Dll(Dll&& other) noexcept : head(other.head), tail(other.tail), size(other.size) {
            other.head = nullptr;
            other.tail = nullptr;
            other.size = 0;
        }
        Dll(std::initializer_list<T> initList) : head(nullptr), tail(nullptr), size(0) {
            for (const T& item : initList) {
                addLast(item);
            }
        }
        Dll& operator=(const Dll& other) {
            if (this != &other) {
                clear();
                Node* temp = other.head;
                while (temp != nullptr) {
                    addLast(temp->value);
                    temp = temp->next;
                }
            }
            return *this;
        }
        Dll& operator=(Dll&& other) noexcept {
            if (this != &other) {
                Node* moveHead = other.head;
                Node* moveTail = other.tail;
                int moveSize = other.size;

                other.size = size;
                other.head = head;
                other.tail = tail;

                size = moveSize;
                head = moveHead;
                tail = moveTail;
            }
            return *this;
        }
        bool isEmpty() const noexcept { return size == 0; }
        size_t getSize() const noexcept { return size; }
        void addFirst(const T& value) { // BigO(1)
            Node* myNode = new Node(value);
            if (size == 0) {
                head = myNode;
                tail = myNode;
            }
            else {
                head->prev = myNode;
                myNode->next = head;
                head = myNode;
            }
            size++;
        }
        void addFirst(T&& value) { // BigO(1)
            Node* myNode = new Node(static_cast<T&&>(value));
            if (size == 0) {
                head = myNode;
                tail = myNode;
            }
            else {
                head->prev = myNode;
                myNode->next = head;
                head = myNode;
            }
            size++;
        }
        void addLast(const T& value) { // BigO(1)
            Node* myNode = new Node(value);
            if (size == 0) {
                head = myNode;
                tail = myNode;
            }
            else {
                tail->next = myNode;
                myNode->prev = tail;
                tail = myNode;
            }
            size++;
        }
        void addLast(T&& value) { // BigO(1)
            Node* myNode = new Node(static_cast<T&&>(value));
            if (size == 0) {
                head = myNode;
                tail = myNode;
            }
            else {
                tail->next = myNode;
                myNode->prev = tail;
                tail = myNode;
            }
            size++;
        }
        void insert(size_t index, T&& value) { // BigO(n)
            if (index == 0) {
                addFirst(static_cast<T&&>(value));
                return;
            }
            else if (index == size) {
                addLast(static_cast<T&&>(value));
                return;
            }
            checkAddBounds(index);
            Node* temp = getNodeAddress(index);
            Node* myNode = new Node(static_cast<T&&>(value));

            myNode->next = temp;
            myNode->prev = temp->prev;
            myNode->prev->next = myNode;
            temp->prev = myNode;
            size++;
        }
        void insert(size_t index, const T& value) { // BigO(n)
            if (index == 0) {
                addFirst(value);
                return;
            }
            else if (index == size) {
                addLast(value);
                return;
            }
            checkAddBounds(index);
            Node* temp = getNodeAddress(index);
            Node* myNode = new Node(value);

            myNode->next = temp;
            myNode->prev = temp->prev;
            myNode->prev->next = myNode;
            temp->prev = myNode;
            size++;
        }
        const T& get(size_t index) const { // BigO(n)
            checkEmpty();
            checkAvailableBounds(index);
            const Node* temp = getNodeAddress(index);
            return temp->value;
        }
        T removeFirst() { // BigO(1)
            checkEmpty();
            Node* target = head;
            if (size == 1) {
                head = nullptr;
                tail = nullptr;
            }
            else {
                head = head->next;
                head->prev = nullptr;
            }

            T returnValue = static_cast<T&&>(target->value);
            delete target;
            size--;
            return returnValue;
        }
        T removeLast() { // BigO(1)
            checkEmpty();
            Node* target = tail;
            if (size == 1) {
                head = nullptr;
                tail = nullptr;
            }
            else {
                tail = tail->prev;
                tail->next = nullptr;
            }
            T returnValue = static_cast<T&&>(target->value);
            delete target;
            size--;
            return returnValue;
        }
        long long indexOf(const T& target) const { // BigO(n)
            checkEmpty();
            Node* temp = head;
            for (size_t i = 0; i < size; i++) {
                if (temp->value == target) {
                    return i;
                }
                temp = temp->next;
            }
            return -1;
        }
        T remove(size_t index) { // BigO(n)
            if (index == 0) {
                return removeFirst();
            }
            else if (index == size - 1) {
                return removeLast();
            }
            checkEmpty();
            checkAvailableBounds(index);
            Node* temp = getNodeAddress(index);
            T returnValue = static_cast<T&&>(temp->value);
            temp->prev->next = temp->next;
            temp->next->prev = temp->prev;
            delete temp;
            size--;
            return returnValue;
        }
        void set(size_t index, const T& value) { // BigO(n)
            checkEmpty();
            checkAvailableBounds(index);
            Node* temp = getNodeAddress(index);
            temp->value = value;
        }
        void set(size_t index, T&& value) { // BigO(n)
            checkEmpty();
            checkAvailableBounds(index);
            Node* temp = getNodeAddress(index);
            temp->value = static_cast<T&&>(value);
        }
        void swap(size_t index1, size_t index2) { // BigO(n)
            checkEmpty();
            checkAvailableBounds(index1);
            checkAvailableBounds(index2);
            if (index1 == index2) {
                return;
            }
            Node* temp1 = getNodeAddress(index1);
            Node* temp2 = getNodeAddress(index2);
            T tempValue = static_cast<T&&>(temp1->value);
            temp1->value = static_cast<T&&>(temp2->value);
            temp2->value = static_cast<T&&>(tempValue);
        }
        void sort() { // BigO(n * log(n))
            mLib::mergeSort(begin(), end());
        }
        template<typename Compare>
        void sort(Compare comp) { // BigO(n * log(n))
            mLib::mergeSort(begin(), end(), comp);
        }
        void clear() noexcept { // BigO(n)
            Node* temp = head;
            head = nullptr;
            tail = nullptr;
            size = 0;
            while (temp != nullptr) {
                Node* next = temp->next;
                delete temp;
                temp = next;
            }
        }
        void shuffle() { // BigO(n)
            if (isEmpty() || size == 1) { return; }
            std::mt19937& gen = mLib::getGlobalRNG();
            T** array = new T*[size];
            T** pin = array;
            try{
                for (auto& item : *this) {
                    *array = &item;
                    array++;
                }
                array -= size;
                for (size_t i = size - 1; i > 0; --i) {
                    std::uniform_int_distribution<size_t> distr(0, i);
                    size_t random = distr(gen);
                    T move = static_cast<T&&>(*array[random]);
                    *array[random] = static_cast<T&&>(*array[i]);
                    *array[i] = static_cast<T&&>(move);
                }
            }catch (...) {
                delete[] pin;
                throw;
            }
            delete[] array;
        }
        const T& getRandom(size_t index1, size_t index2) const{ // BigO(n)
            checkEmpty();
            checkAvailableBounds(index1);
            checkAvailableBounds(index2);
            if (index1 > index2) {
                throw std::invalid_argument("Invalid range: index1 (" + std::to_string(index1) + ") cannot be greater than index2 (" + std::to_string(index2) + ").");
            }
            std::mt19937& gen = mLib::getGlobalRNG();
            std::uniform_int_distribution<size_t> distr(index1, index2);
            return getNodeAddress(distr(gen))->value;
        }
        const T& getRandom() const { // BigO(n)
            checkEmpty();
            std::mt19937& gen = mLib::getGlobalRNG();
            std::uniform_int_distribution<size_t> distr(0, size - 1);
            return getNodeAddress(distr(gen))->value;
        }
        class Iterator {
        private:
            Node* current;
        public:
            using iterator_category = std::bidirectional_iterator_tag;
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
            Iterator& operator--() noexcept {
                if (current != nullptr) {
                    current = current->prev;
                }
                return *this;
            }
            Iterator operator--(int) noexcept {
                Iterator temp = *this;
                --(*this);
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
            using iterator_category = std::bidirectional_iterator_tag;
            using value_type = T;
            using difference_type = std::ptrdiff_t;
            using reference = const T&;
            using pointer = const T*;

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
            Const_Iterator& operator--() noexcept {
                if (current != nullptr) {
                    current = current->prev;
                }
                return *this;
            }
            Const_Iterator operator--(int) noexcept {
                Const_Iterator temp = *this;
                --(*this);
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
        T& back() { // BigO(1)
            checkEmpty();
            return tail->value;
        }
        const T& front() const { // BigO(1)
            checkEmpty();
            return head->value;
        }
        const T& back() const { // BigO(1)
            checkEmpty();
            return tail->value;
        }
        friend std::ostream& operator<<(std::ostream& os, const Dll<T>& list) { // BigO(n)
            if (list.isEmpty()) { os << "NULL"; }
            else {
                for (const auto& put : list) {
                    os << put << " <--> ";
                }
                os << "END";
            }
            return os;
        }
    };
}