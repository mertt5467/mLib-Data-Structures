#pragma once

#include <iostream>
#include <string>
#include <stdexcept>
#include <initializer_list> 
#include "debug.hpp"
namespace mLib {
    template<typename T>
    class Array {
    private:
        size_t capacity;
        T* array;

        inline void RAII_DESTRUCTOR(){
            if (array != nullptr) {
                for (size_t i = 0; i < capacity; ++i) {
                    array[i].~T();
                }
                ::operator delete(array);
            }
            capacity = 0;
            array = nullptr;
        }
        void MEMORY_EXCEPTION_HANDLING(T*& trash, size_t obj_count) {
            if (obj_count > 0) {
                for (size_t i = 0; i < obj_count; ++i) {
                    trash[i].~T();
                }
            }
            if (trash != nullptr) {
                ::operator delete(trash);
            }
            trash = nullptr;
        }
    public:
        explicit Array(size_t capacity) : capacity(capacity) { // BigO(n)
            if (capacity == 0) { array = nullptr; }
            else {
                array = static_cast<T*>(::operator new(capacity * sizeof(T)));
                size_t i;
                try {
                    for (i = 0; i < capacity; ++i) {
                        new (&array[i]) T();
                    }
                }catch(...){
                    DEBUG_LOG(this, "[EXCEPTION] Array constructor failed at index: " << i << ". RAII taking over, freeing memory.");
                    MEMORY_EXCEPTION_HANDLING(array, i);
                    throw;
                }
            }
            DEBUG_LOG(this, "Array created. Capacity: " << capacity);
        }
        Array(size_t capacity, const T& defaultValue) : capacity(capacity) { // BigO(n)
            if (capacity == 0) { array = nullptr; }
            else {
                array = static_cast<T*>(::operator new(capacity * sizeof(T)));
                size_t i;
                try {
                    for (i = 0; i < capacity; ++i) {
                        new (&array[i]) T(defaultValue);
                    }
                }catch (...) {
                    DEBUG_LOG(this, "[EXCEPTION] Array constructor failed at index: " << i << ". RAII taking over, freeing memory.");
                    MEMORY_EXCEPTION_HANDLING(array, i);
                    throw;
                }
            }
            DEBUG_LOG(this, "Array created. Capacity: " << capacity);
        }
        Array(std::initializer_list<T> initList) : capacity(initList.size()) { // BigO(n)
            if (capacity == 0) { array = nullptr; }
            else {
                array = static_cast<T*>(::operator new(capacity * sizeof(T)));
                size_t i = 0;
                try {
                    for (const auto& item : initList) {
                        new (&array[i]) T(item);
                        i++;
                    }
                }catch (...) {
                    DEBUG_LOG(this, "[EXCEPTION] Array initializer_list constructor failed at index: " << i << ". RAII taking over, freeing memory.");
                    MEMORY_EXCEPTION_HANDLING(array, i);
                    throw;
                }
            }
            DEBUG_LOG(this, "Array created. Capacity: " << capacity);
        }
        explicit Array(const Array& other) : capacity(other.capacity) { // BigO(n)
            if (capacity == 0) { array = nullptr; }
            else {
                array = static_cast<T*>(::operator new(capacity * sizeof(T)));
                size_t i;
                try {
                    for (i = 0; i < capacity; i++) { new (&array[i]) T(other.array[i]); }
                }
                catch (...) {
                    DEBUG_LOG(this, "[EXCEPTION] Array copy constructor failed at index: " << i << ". RAII taking over, freeing memory.");
                    MEMORY_EXCEPTION_HANDLING(array, i);
                    throw;
                }
                
            }
            DEBUG_LOG(this, "Array copy-constructed from obj: " << &other << " (Cap: " << capacity << ")");
        }

        Array(Array&& other) noexcept : array(other.array), capacity(other.capacity) { // BigO(1)
            other.array = nullptr;
            other.capacity = 0;
            DEBUG_LOG(this, "Array move-constructed from obj: " << &other << " (Cap: " << capacity << ")");
        }
        ~Array() {
            RAII_DESTRUCTOR();
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
                    MEMORY_EXCEPTION_HANDLING(newArray, i);
                    throw;
                }
                RAII_DESTRUCTOR();
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
        inline size_t getSize() const noexcept { return capacity; }
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