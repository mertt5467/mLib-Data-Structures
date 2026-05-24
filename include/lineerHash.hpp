#pragma once

#include <iostream>
#include <string>
#include <stdexcept>
#include <random> // not used
#include <initializer_list> // not used

// @DEPRECATED 
namespace mLib {
    template<typename T>
    struct Entry {
        int key;
        bool isOccupied;
        T value;
        Entry(int key, T value) noexcept : key(key), value(static_cast<T&&>(value)), isOccupied(true) {}
        Entry() : key(0), isOccupied(false) {}

        Entry(Entry&& other) noexcept : key(other.key), isOccupied(other.isOccupied), value(static_cast<T&&>(other.value)) {
            other.isOccupied = false;
            other.key = 0;
        }
        Entry(Entry& other) noexcept : key(other.key), isOccupied(other.isOccupied), value(static_cast<T&>(other.value)) {
            // Empty
        }
        Entry& operator=(Entry&& other) noexcept { // BigO(1)
            if (this != &other) {
                key = other.key;
                isOccupied = other.isOccupied;
                value = static_cast<T&&>(other.value);
                other.isOccupied = false;
                other.key = 0;
            }
            return *this;
        }
        Entry& operator=(const Entry& other) noexcept { // BigO(1)
            if (this != &other) {
                key = other.key;
                isOccupied = other.isOccupied;
                value = other.value;
            }
            return *this;
        }
    };
    template<typename T>
    class LineerHash {
    private:
        Entry<T>* table;
        int capacity;
        int size = 0;
        int getHashCode(unsigned int key) const {
            return key % capacity;
        }
        inline void next(int& index) const {
            index = (index + 1) % capacity;
        }
        void shiftingDelete(int removeIndex) { // BigO(n)
            if (removeIndex >= capacity || removeIndex < 0) { throw std::out_of_range("Index out of bounds"); }
            table[removeIndex].isOccupied = false;
            int i = removeIndex;
            for (next(i); table[i].isOccupied; next(i)) {
                if (table[i].key % capacity != i && isBetweenCircular(removeIndex, getHashCode(table[i].key), i)) {
                    table[removeIndex] = static_cast<Entry<T>&&>(table[i]);
                    table[i].isOccupied = false;
                    removeIndex = i;
                }
            }
        }
        bool isBetweenCircular(int target, int keyIndex, int current) const {
            return (keyIndex <= target && target < current) || (current < keyIndex && keyIndex <= target) || (target < current && current < keyIndex);
        }
    public:
        LineerHash(int capacity) : capacity(capacity) { table = new Entry<T>[capacity]; }
        bool isEmpty() const { return size == 0; }
        bool isFull() const { return size == capacity; }
        ~LineerHash() { delete[] table; }

        void put(const Entry<T>& myEntry) { // BigO(n)
            if (isFull()) {
                throw std::out_of_range("Hash is full");
            }
            int index = getHashCode(myEntry.key);
            if (table[index].isOccupied == false) {
                table[index] = myEntry;
                size++;
            }
            else {
                while (table[index].isOccupied && table[index].key != myEntry.key) {
                    next(index);
                }
                if (table[index].isOccupied == false) {
                    size++;
                }
                table[index] = myEntry;
            }
        }
        const T& get(int key) const { // BigO(n)
            int index, finish;
            finish = index = getHashCode(key);
            if (isEmpty()) {
                throw std::out_of_range("Hash is Empty");
            }
            else {
                while (table[index].isOccupied) {
                    if (table[index].key == key) { return table[index].value; }
                    next(index);
                    if (index == finish) {
                        break;
                    }
                }
                throw std::out_of_range("Value is not found");
            }
        }
        void print() const {
            for (int i = 0; i < capacity; i++) {
                if (table[i].isOccupied == false) {
                    std::cout << "NULL" << " | ";
                }
                else {
                    std::cout << table[i].value << " | ";
                }
            }
            std::cout << std::endl;
        }
        T remove(int key) { // BigO(n)
            if (isEmpty()) {
                throw std::out_of_range("Hash is Empty");
            }
            else {
                int index = getHashCode(key);
                int finish = index;
                do {
                    if (table[index].key == key) {
                        T temp = table[index].value;
                        shiftingDelete(index);
                        size--;
                        return temp;
                    }
                    next(index);
                } while (index != finish && table[index].isOccupied);
                throw std::out_of_range("Value is not found");
            }
        }
    };
}