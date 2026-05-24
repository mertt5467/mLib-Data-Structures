#pragma once

#include <iostream>
#include <string>
#include <stdexcept>
#include <initializer_list> // @TODO: not used, write once
#include "debug.hpp"
namespace mLib {
    template<typename T>
    class Array {
        int capacity;
        T* array;
    public:
        explicit Array(int capacity) : capacity(capacity) { // BigO(n)
            array = new T[capacity]{};
            DEBUG_LOG(this, "Array created. Capacity: " << capacity);
        }
        Array(const Array& other) : capacity(other.capacity) { // BigO(n)
            array = new T[capacity];
            for (int i = 0; i < capacity; i++) { array[i] = other.array[i]; }
            DEBUG_LOG(this, "Array copy-constructed from obj: " << &other << " (Cap: " << capacity << ")");
        }

        Array(Array&& other) noexcept : array(other.array), capacity(other.capacity) { // BigO(1)
            other.array = nullptr;
            other.capacity = 0;
            DEBUG_LOG(this, "Array move-constructed from obj: " << &other << " (Cap: " << capacity << ")");
        }
        ~Array() {
            delete[] array;
            DEBUG_LOG(this, "Array destroyed. Memory freed.");
        }
        T& operator[](int index) { // BigO(1)
            if (index < 0 || index >= capacity) { throw std::out_of_range("Index out of bounds. Capacity = " + std::to_string(capacity)); }
            return array[index];
        }
        const T& operator[](int index) const { // BigO(1)
            if (index < 0 || index >= capacity) { throw std::out_of_range("Index out of bounds. Capacity = " + std::to_string(capacity)); }
            return array[index];
        }
        Array& operator=(Array&& other) noexcept { // BigO(1)
            if (this != &other) {
                DEBUG_LOG(this, "Array move-assigned from obj: " << &other << " (Old Cap: " << capacity << " -> New Cap: " << other.capacity << ")");
                delete[] array;
                array = other.array;
                capacity = other.capacity;
                other.array = nullptr;
                other.capacity = 0;
            }
            return *this;
        }
        Array& operator=(const Array& other) { // BigO(n)
            if (this != &other) {
                int oldCap = capacity;
                T* newArray = new T[other.capacity];
                int i = 0;
                try {
                    for (i = 0; i < other.capacity; i++) {
                        newArray[i] = other[i];
                    }
                }
                catch (...) {
                    DEBUG_LOG(this, "[EXCEPTION] Array copy assignment failed at index: " << i << ". RAII taking over, freeing memory.");
                    delete[] newArray;
                    throw;
                }
                delete[] array;
                array = newArray;
                capacity = other.capacity;
                DEBUG_LOG(this, "Array copy-assigned from obj: " << &other << " (Old Cap: " << oldCap << " -> New Cap: " << other.capacity << ")");
            }
            return *this;
        }
        T* begin() noexcept { return array; }
        T* end() noexcept { return array + capacity; }
        const T* begin() const noexcept { return array; }
        const T* end() const noexcept { return array + capacity; }
        int length() const noexcept { return capacity; }
        friend std::ostream& operator<<(std::ostream& os, const Array<T>& arr) {
            DEBUG_LOG(&arr, "Running Array << operator");
            for (const auto& item : arr) {
                os << item << " | ";
            }
            os << "END";
            return os;
        }
    };
}