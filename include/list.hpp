#pragma once

#include <iostream>
#include <string>
#include <stdexcept>
#include <random>
#include <initializer_list>
#include "debug.hpp"
#include "utils.hpp"
#include "mergeSort.hpp"

namespace mLib {
    template<typename T>
    class List {
    private:
        T* array;
        size_t capacity;
        size_t size;
        bool autoShrink;
        List(size_t capacity, size_t size, bool autoShrink) : capacity(capacity), size(size), autoShrink(autoShrink) {
            if (capacity == 0) { array = nullptr; }
            else {
                array = static_cast<T*>(::operator new(capacity * sizeof(T)));
            }
            size_t totalBytes = capacity * sizeof(T);
            size_t totalBits = totalBytes * 8; // 1 Byte == 8 Bit
            DEBUG_LOG(this, "List dynamically allocated. Cap = " + std::to_string(capacity) + " | Size = " + std::to_string(size) + " | Shrink mode = " + std::to_string(autoShrink) + " | Total Size = " + std::to_string(totalBytes) + "Byte (" + std::to_string(totalBits) + " bits)");
        }

        void reSize(size_t targetSize, bool extend) { // BigO(n)
            DEBUG_LOG(this, "Running reSize. Target Cap: " << targetSize << " | Extend Mode: " << extend);
            if (capacity == 1 && extend == false) {
                DEBUG_LOG(this, "Capacity is already 1; no shrink operation executed.");
                return;
            }
            T* newArray = nullptr; 
            size_t i = 0;
            try {
                newArray = static_cast<T*>(::operator new(targetSize * sizeof(T)));
                if constexpr (std::is_nothrow_move_constructible_v<T>) {
                    for (i = 0; i < size; ++i) {
                        new (&newArray[i]) T(static_cast<T&&>(array[i]));
                        array[i].~T();
                    }
                }
                else {
                    for (i = 0; i < size; ++i) {
                        new (&newArray[i]) T(array[i]);
                    }
                    for (size_t j = 0; j < size; ++j) {
                        array[j].~T();
                    }
                }
            }
            catch (...) {
                if (newArray != nullptr) {
                    for (size_t j = 0; j < i; ++j) {
                        newArray[j].~T();
                    }
                    ::operator delete(newArray);
                }
                throw;
            }
            ::operator delete(array);
            array = newArray;
            capacity = targetSize;
            DEBUG_LOG(this, "reSize completed successfully.");
        }
        void RAII_DESTRUCTOR() noexcept { // BigO(n)
            DEBUG_LOG(this, "Destroying list contents...");
            for (size_t i = 0; i < size; i++) {
                array[i].~T();
            }
            size = 0;
            capacity = 0;
            ::operator delete(array);
            array = nullptr;
        }
        void checkEmpty() const{
            if (isEmpty()) { throw std::out_of_range("List is empty."); }
        }
        void checkAvailableBounds(size_t index) const{
            if (index >= size) { throw std::out_of_range("Index out of bounds. Size = " + std::to_string(size)); }
        }
        void checkAddBounds(size_t index) const{
            if (index > size) { throw std::out_of_range("Index out of bounds. Size = " + std::to_string(size)); }
        }
    public:
        ~List() {
            RAII_DESTRUCTOR();
            DEBUG_LOG(this, "List destroyed. Cleaning up elements and freeing memory.");
        }
        List() : List(0, 0, true) {}
        explicit List(size_t startCapacity) : List(startCapacity, 0, true) {}
        explicit List(bool autoShrink) : List(0, 0, autoShrink) {}
        explicit List(int startCapacity) : List( startCapacity < 0 ? throw std::invalid_argument("Capacity cannot be negative.") : static_cast<size_t>(startCapacity), 0, true){}
        List(size_t startCapacity, bool autoShrink) : List(startCapacity, 0, autoShrink) {}
        List(std::initializer_list<T> initList) : List(initList.size(), 0, true) {
            for (const auto& item : initList) {
                add(item);
            }
            DEBUG_LOG(this, "List constructed via initializer_list (Elements: " << initList.size() << ")");
        }
        List(const List& other) : List(other.capacity, 0, other.autoShrink) {
            for (const auto& item : other) {
                add(item);
            }
            DEBUG_LOG(this, "List copy-constructed from obj: " << &other);
        }
        List(List&& other) noexcept : array(other.array), capacity(other.capacity), size(other.size), autoShrink(other.autoShrink) {
            other.array = nullptr;
            other.capacity = 0;
            other.size = 0;
            DEBUG_LOG(this, "List move-constructed from obj: " << &other);
        }
        List& operator=(const List& other) {
            if (this != &other) {
                size_t newCap = other.capacity;
                size_t newSize = other.size;
                bool newShrink = other.autoShrink;

                T* newArray = nullptr;
                if (newCap != 0) {
                    newArray = static_cast<T*>(::operator new(other.capacity * sizeof(T)));
                }
                size_t catchItemCount = 0;
                try {
                    for (size_t i = 0; i < newSize; i++) {
                        new (&newArray[i]) T(other[i]);
                        catchItemCount++;
                    }
                }
                catch (...) {
                    DEBUG_LOG(this, "[EXCEPTION] Copying failed at index: " << catchItemCount << ". Rolling back and destroying " << catchItemCount << " constructed elements.");
                    for (size_t i = 0; i < catchItemCount; i++) {
                        newArray[i].~T();
                    }
                    ::operator delete(newArray);
                    throw;
                }
                for (size_t i = 0; i < size; i++) {
                    array[i].~T();
                }
                ::operator delete(array);
                array = newArray;
                capacity = newCap;
                size = newSize;
                autoShrink = newShrink;
                DEBUG_LOG(this, "List copy-assigned from obj: " << &other);
            }
            return *this;
        }
        List& operator=(List&& other) noexcept {
            if (this != &other) {
                T* tempArr = array;
                size_t tempCap = capacity;
                size_t tempSize = size;
                bool tempAutoShrink = autoShrink;

                array = other.array;
                capacity = other.capacity;
                size = other.size;
                autoShrink = other.autoShrink;

                other.array = tempArr;
                other.capacity = tempCap;
                other.size = tempSize;
                other.autoShrink = tempAutoShrink;
            }
            DEBUG_LOG(this, "List move-assigned from obj: " << &other);
            return *this;
        }
        void reserve(size_t newCapacity) {
            if (newCapacity > capacity) {
                reSize(newCapacity, true);
            }
            autoShrink = false;
            DEBUG_LOG(this, "Reserving capacity manually to: " << newCapacity);
        }
        T& operator[](size_t index) { // BigO(1)
            checkAvailableBounds(index);
            return array[index];
        }
        const T& operator[](size_t index) const { // BigO(1)
            checkAvailableBounds(index);
            return array[index];
        }
        void add(const T& value) { // (Amortized O(1))
            add(size, value);
        }
        void add(T&& value) { // (Amortized O(1))
            add(size, static_cast<T&&>(value));
        }
        void add(size_t index, const T& value) { // BigO(n)
            DEBUG_LOG(this, "Adding L-Value at index: " << index);
            checkAddBounds(index);
            if (size == capacity) {
                reSize(capacity == 0 ? 1 : capacity * 2, true);
            }
            if (index == size) {
                new (&array[index]) T(value);
            }
            else {
                new (&array[size]) T(static_cast<T&&>(array[size - 1]));
                for (size_t i = size - 1; i > index; i--) {
                    array[i] = static_cast<T&&>(array[i - 1]);
                }
                array[index] = value;

            }
            size++;
        }
        void add(size_t index, T&& value) { // BigO(n)
            DEBUG_LOG(this, "Adding R-Value (Move) at index: " << index);
            checkAddBounds(index);
            if (size == capacity) {
                reSize(capacity == 0 ? 1 : capacity * 2, true);
            }
            if (index == size) {
                new (&array[index]) T(static_cast<T&&>(value));
            }
            else {
                new (&array[size]) T(static_cast<T&&>(array[size - 1]));
                for (size_t i = size - 1; i > index; i--) {
                    array[i] = static_cast<T&&>(array[i - 1]);
                }
                array[index] = static_cast<T&&>(value);

            }
            size++;
        }
        const T& get(size_t index) const { // BigO(1)
            checkAvailableBounds(index);
            return array[index];
        }
        long long indexOf(const T& value) const { // BigO(n)
            for (size_t i = 0; i < size; i++) {
                if (array[i] == value) {
                    return i;
                }
            }
            return -1;
        }
        void set(size_t index, T&& value) { // BigO(1)
            DEBUG_LOG(this, "Setting R-Value (Move) at index: " << index);
            checkAvailableBounds(index);
            array[index] = static_cast<T&&>(value);
        }
        void set(size_t index, const T& value) { // BigO(1)
            DEBUG_LOG(this, "Setting L-Value at index: " << index);
            checkAvailableBounds(index);
            array[index] = value;
        }
        void shuffle() { // BigO(n)
            DEBUG_LOG(this, "Shuffling the list...");
            if (isEmpty() || size == 1) {
                return;
            }
            std::mt19937& gen = mLib::getGlobalRNG();

            for (size_t i = size - 1; i > 0; i--) {
                std::uniform_int_distribution<size_t> distr(0, i);
                size_t randomNumber = distr(gen);
                swap(i, randomNumber);
            }
        }
        const T& getRandom(size_t index1, size_t index2) const{ //BigO(1)
            checkEmpty();
            checkAvailableBounds(index1);
            checkAvailableBounds(index2);
            if (index1 > index2) {
                throw std::invalid_argument("Invalid range: index1 (" + std::to_string(index1) + ") cannot be greater than index2 (" + std::to_string(index2) + ").");
            }
            if (index1 == index2) {
                return array[index1];
            }
            std::mt19937& gen = mLib::getGlobalRNG();
            std::uniform_int_distribution<size_t> distr(index1, index2);
            return array[distr(gen)];
        }
        const T& getRandom() const{ // BigO(1)
            checkEmpty();
            std::mt19937& gen = mLib::getGlobalRNG();
            std::uniform_int_distribution<size_t> distr(0, size-1);
            return array[distr(gen)];
        }
        void swap(size_t index1, size_t index2) { // BigO(1)
            DEBUG_LOG(this, "Swapping elements at indices: " << index1 << " & " << index2);
            checkAvailableBounds(index1);
            checkAvailableBounds(index2);
            if (index1 == index2) { return; }
            else {
                T temp = static_cast<T&&>(array[index1]);
                array[index1] = static_cast<T&&>(array[index2]);
                array[index2] = static_cast<T&&>(temp);
            }
        }
        T remove() { // BigO(1)
            DEBUG_LOG(this, "Removing element from end (pop_back). Target index: " << size - 1);
            checkEmpty();
            T temp = static_cast<T&&>(array[size - 1]);
            array[size - 1].~T();
            size--;
            if (size > 0 && size < (capacity / 4) && autoShrink) {
                reSize(capacity / 2, false);
            }
            return temp;
        }
        T remove(size_t index) { // BigO(n)
            DEBUG_LOG(this, "Removing element at index: " << index);
            checkEmpty();
            checkAvailableBounds(index);
            T temp = static_cast<T&&>(array[index]);
            for (size_t i = index; i < size - 1; i++) {
                array[i] = static_cast<T&&>(array[i + 1]);
            }
            array[size - 1].~T();
            size--;
            if (size > 0 && size < (capacity / 4) && autoShrink) {
                reSize(capacity / 2, false);
            }
            return temp;
        }
        void sort() { // BigO(n * log(n))
            DEBUG_LOG(this, "List is sorting...");
            mLib::mergeSort(begin(), end());
        }
        template<typename Compare>
        void sort(Compare comp) { // BigO(n * log(n))
            DEBUG_LOG(this, "List is custom sorting...");
            mLib::mergeSort(begin(), end(), comp);
        }
        T& front() { // BigO(1)
            checkEmpty();
            return array[0];
        }
        const T& front() const { // BigO(1)
            checkEmpty();
            return array[0];
        }
        T& back() { // BigO(1)
            checkEmpty();
            return array[size - 1];
        }
        const T& back() const { // BigO(1)
            checkEmpty();
            return array[size - 1];
        }
        void clear() { // BigO(n)
            if (autoShrink && capacity > 0) {
                RAII_DESTRUCTOR();
            }else {
                DEBUG_LOG(this, "Clearing list contents...");
                for (size_t i = 0; i < size; i++) {
                    array[i].~T();
                }
                size = 0;
            }
        }
        void reset() { // BigO(n)
            RAII_DESTRUCTOR();
        }
        T* begin() noexcept { return array; }
        T* end() noexcept { return array + size; }
        const T* begin() const noexcept { return array; }
        const T* end() const noexcept { return array + size; }
        size_t getSize() const noexcept { return size; }
        bool isEmpty() const noexcept { return size == 0; }
        size_t getCap() const noexcept { return capacity; }
        bool infoAutoShrink() const noexcept { return autoShrink; }
        void setAutoShrink(bool autoShrink) { DEBUG_LOG(this, "AutoShrink modified to: " << autoShrink); this->autoShrink = autoShrink; if (size > 0 && size < (capacity / 4) && autoShrink) { reSize(capacity / 2, false); } }
        friend std::ostream& operator<<(std::ostream& os, const List<T>& list) {
            DEBUG_LOG(&list, "Running List << operator");
            if (list.isEmpty()) {
                os << "EMPTY";
            }
            else {
                for (const auto& item : list) {
                    os << item << " | ";
                }
                os << "END";
            }
            return os;
        }
    };
}