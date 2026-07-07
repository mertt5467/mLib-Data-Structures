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
        Node* tail;
        size_t size;
        void clear() noexcept { // BigO(n)
            Node* temp = head;
            head = nullptr;
            tail = nullptr;
            while (temp != nullptr) {
                Node* next = temp->next;
                delete temp;
                temp = next;

            }
        }
        Node* getNodeAddress(size_t index) noexcept { // BigO(n)
            if (index == size - 1) {
                return tail;
            }
            Node* temp = head;
            for (size_t i = 0; i < index; i++) {
                temp = temp->next;
            }
            return temp;
        }
        const Node* getNodeAddress(size_t index) const { // BigO(n)
            return const_cast<Sll*>(this)->getNodeAddress(index);
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
        Sll() : head(nullptr), tail(nullptr), size(0) {}
        explicit Sll(const Sll& other) : head(nullptr), tail(nullptr), size(0) { // BigO(n)
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
                tail = temp2;
            }
            catch (...) {
                clear();
                throw;
            }
        }
        Sll(Sll&& other) noexcept : head(other.head), tail(other.tail), size(other.size) { // BigO(1)
            other.head = nullptr;
            other.tail = nullptr;
            other.size = 0;
        }
        Sll(std::initializer_list<T> initList) : head(nullptr), tail(nullptr), size(0) { // BigO(n)
            try {
                bool first = true;
                Node* temp = nullptr;
                for (const auto& item : initList) {
                    if (first) {
                        first = false;
                        head = new Node(item);
                        temp = head;
                    }
                    else {
                        temp->next = new Node(item);
                        temp = temp->next;
                    }
                    size++;
                }
                tail = temp;
            }
            catch (...) {
                clear();
                throw;
            }
        }
        Sll& operator=(const Sll& other) { // BigO(n)
            if (this != &other) {
                Sll temp(other);
                *this = static_cast<Sll&&>(temp);
            }
            return *this;
        }
        Sll& operator=(Sll&& other) noexcept { // BigO(1)
            if (this != &other) {
                size_t moveSize = size;
                Node* moveHead = head;
                Node* moveTail = tail;
                head = other.head;
                tail = other.tail;
                size = other.size;
                other.head = moveHead;
                other.tail = moveTail;
                other.size = moveSize;
            }
            return *this;
        }
        ~Sll() {
            clear();
        }
        void addLast(const T& value) { // BigO(1)
            if (isEmpty()) {
                head = new Node(value);
                tail = head;
            }
            else {
                tail->next = new Node(value);
                tail = tail->next;
            }
            size++;
        }
        void addLast(T&& value) { // BigO(1)
            if (isEmpty()) {
                head = new Node(static_cast<T&&>(value));
                tail = head;
            }
            else {
                tail->next = new Node(static_cast<T&&>(value));
                tail = tail->next;
            }
            size++;
        }
        void addFirst(const T& value) { // BigO(1)
            if (isEmpty()) {
                head = new Node(value);
                tail = head;
            }
            else {
                Node* temp = new Node(value);
                temp->next = head;
                head = temp;
            }
            size++;
        }
        void addFirst(T&& value) { // BigO(1)
            if (isEmpty()) {
                head = new Node(static_cast<T&&>(value));
                tail = head;
            }
            else {
                Node* temp = new Node(static_cast<T&&>(value));
                temp->next = head;
                head = temp;
            }
            size++;
        }
        void insert(size_t index, const T& value) { // BigO(n)
            if (index == 0) {
                addFirst(value);
            }
            else if (index == size) {
                addLast(value);
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
        void insert(size_t index, T&& value) { // BigO(n)
            if (index == 0) {
                addFirst(static_cast<T&&>(value));
            }
            else if (index == size) {
                addLast(static_cast<T&&>(value));
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
        T& get(size_t index) { // BigO(n)
            checkEmpty();
            checkAvailableBounds(index);
            Node* temp = getNodeAddress(index);
            return temp->value;
        }
        const T& get(size_t index) const { // BigO(n)
            checkEmpty();
            checkAvailableBounds(index);
            const Node* temp = getNodeAddress(index);
            return temp->value;
        }
        T removeFirst() { // BigO(1)
            checkEmpty();
            Node* trash = head;
            T returnValue = static_cast<T&&>(trash->value);
            if (size == 1) {
                tail = nullptr;
            }
            head = head->next;
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
                tail = nullptr;
            }
            else {
                tail = getNodeAddress(size - 2);
                trash = tail->next;
                tail->next = nullptr;
            }
            T returnValue = static_cast<T&&>(trash->value);
            delete trash;
            size--;
            return returnValue;
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
            Node* temp = getNodeAddress(index - 1);
            Node* trash = temp->next;
            temp->next = trash->next;
            T returnValue = static_cast<T&&>(trash->value);
            delete trash;
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
            else {
                Node* temp1 = getNodeAddress(index1);
                Node* temp2 = getNodeAddress(index2);
                T move = static_cast<T&&>(temp1->value);
                temp1->value = static_cast<T&&>(temp2->value);
                temp2->value = static_cast<T&&>(move);
            }
        }
        long long indexOf(const T& value) const noexcept { // BigO(n)
            Node* temp = head;
            for (size_t i = 0; i < size; i++) {
                if (value == temp->value) {
                    return i;
                }
                temp = temp->next;
            }
            return -1;
        }
        void sort() { // BigO(n * log(n))
            mLib::mergeSort(begin(), end());
        }
        template<typename Compare>
        void sort(Compare comp) { // BigO(n * log(n))
            mLib::mergeSort(begin(), end(), comp);
        }
        void shuffle() { // BigO(n)
            if (isEmpty() || size == 1) { return; }
            std::mt19937& gen = mLib::getGlobalRNG();
            T** array = new T * [size];
            T** pin = array;
            try {
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
            }
            catch (...) {
                delete[] pin;
                throw;
            }
            delete[] pin;
        }
        const T& getRandom(size_t index1, size_t index2) const { // BigO(n)
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
        Const_Iterator cbegin() const noexcept { return begin(); }
        Const_Iterator cend() const noexcept { return end(); }
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
        friend std::ostream& operator<<(std::ostream& os, const Sll<T>& list) {
            if (list.isEmpty()) { os << "EMPTY"; }
            else {
                for (const auto& item : list) {
                    os << item << " --> ";
                }
                os << "END";
            }
            return os;
        }
        bool isEmpty() const noexcept { return size == 0; }
        size_t getSize() const noexcept { return size; }
    };
}