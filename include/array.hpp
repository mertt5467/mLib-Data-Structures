#pragma once

#include <iostream>
#include <string>
#include <stdexcept>
#include <initializer_list> // @TODO: not used, write once
#include "debug.hpp"
namespace mLib {
    template<typename T>
    class Array {
    private:
        size_t capacity;
        T* array;

        inline static void RAII_DESTRUCTOR(Array& trash){
            if (trash.array != nullptr) {
                for (size_t i = 0; i < trash.capacity; ++i) {
                    trash.array[i].~T();
                }
                ::operator delete(trash.array);
            }
            trash.capacity = 0;
            trash.array = nullptr;
        }
    public:
        explicit Array(size_t capacity) : capacity(capacity) { // BigO(n)
            if (capacity == 0) { array = nullptr; }
            else {
                array = static_cast<T*>(::operator new(capacity * sizeof(T)));
                for (size_t i = 0; i < capacity; ++i) {
                    new (&array[i]) T();
                }
            }
            DEBUG_LOG(this, "Array created. Capacity: " << capacity);
        }
        Array(size_t capacity, const T& defaultValue) : capacity(capacity) { // BigO(n)
            if (capacity == 0) { array = nullptr; }
            else {
                array = static_cast<T*>(::operator new(capacity * sizeof(T)));
                for (size_t i = 0; i < capacity; ++i) {
                    new (&array[i]) T(defaultValue);
                }
            }
            DEBUG_LOG(this, "Array created. Capacity: " << capacity);
        }
        Array(std::initializer_list<T> initList) : capacity(initList.size()) { // BigO(n)
            if (capacity == 0) { array = nullptr; }
            else {
                array = static_cast<T*>(::operator new(capacity * sizeof(T)));
                size_t i = 0;
                for (const auto& item : initList) {
                    new (&array[i]) T(item);
                    i++;
                }
            }
            DEBUG_LOG(this, "Array created. Capacity: " << capacity);
        }
        explicit Array(const Array& other) : capacity(other.capacity) { // BigO(n)
            if (capacity == 0) { array = nullptr; }
            else {
                array = static_cast<T*>(::operator new(capacity * sizeof(T)));
                for (size_t i = 0; i < capacity; i++) { new (&array[i]) T(other.array[i]); }
            }
            DEBUG_LOG(this, "Array copy-constructed from obj: " << &other << " (Cap: " << capacity << ")");
        }

        Array(Array&& other) noexcept : array(other.array), capacity(other.capacity) { // BigO(1)
            other.array = nullptr;
            other.capacity = 0;
            DEBUG_LOG(this, "Array move-constructed from obj: " << &other << " (Cap: " << capacity << ")");
        }
        ~Array() {
            RAII_DESTRUCTOR(*this);
            DEBUG_LOG(this, "Array destroyed. Memory freed.");
        }
        T& operator[](size_t index) { // BigO(1)
            if (index >= capacity) { throw std::out_of_range("Index out of bounds. Capacity = " + std::to_string(capacity)); }
            return array[index];
        }
        const T& operator[](size_t index) const { // BigO(1)
            if (index >= capacity) { throw std::out_of_range("Index out of bounds. Capacity = " + std::to_string(capacity)); }
            return array[index];
        }
        Array& operator=(Array&& other) noexcept { // BigO(1)
            if (this != &other) {
                DEBUG_LOG(this, "Array move-assigned from obj: " << &other << " (Old Cap: " << capacity << " -> New Cap: " << other.capacity << ")");
                T* moveArray = array;
                size_t moveCap = capacity;
                array = other.array;
                capacity = other.capacity;
                other.array = moveArray;
                other.capacity = moveCap;
            }
            return *this;
        }
        Array& operator=(const Array& other) { // BigO(n)
            if (this != &other) {
                size_t oldCap = capacity;
                T* newArray = static_cast<T*>(::operator new (other.capacity * sizeof(T)));
                size_t i = 0;
                try {
                    for (i = 0; i < other.capacity; i++) {
                        new (&newArray[i]) T(other[i]);
                    }
                }
                catch (...) {
                    DEBUG_LOG(this, "[EXCEPTION] Array copy assignment failed at index: " << i << ". RAII taking over, freeing memory.");
                    for (size_t j = 0; j < i; ++j) {
                        newArray[j].~T();
                    }
                    ::operator delete(newArray);
                    throw;
                }
                RAII_DESTRUCTOR(*this);
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
        size_t getSize() const noexcept { return capacity; }
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