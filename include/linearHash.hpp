#pragma once

#include <iostream>
#include <string>
#include <stdexcept>
#include <initializer_list> 
#include "array.hpp"
#include "utils.hpp"

namespace mLib {
    template<typename T, typename U = size_t> 
    class LinearHash {
    private:
        struct Entry {
            U key;
            bool isOccupied;
            union { T value; char dummy; };

            Entry(const T& value_param, const U& key_param) : key(key_param), isOccupied(true) {
                new (&value) T(value_param);
            }

            Entry(T&& value_param, U&& key_param) noexcept : key(static_cast<U&&>(key_param)), isOccupied(true) {
                new (&value) T(static_cast<T&&>(value_param));
            }

            Entry(const T& value_param, U&& key_param) noexcept : key(static_cast<U&&>(key_param)), isOccupied(true) {
                new (&value) T(value_param);
            }

            Entry(T&& value_param, const U& key_param) noexcept : key(key_param), isOccupied(true) {
                new (&value) T(static_cast<T&&>(value_param));
            }

            Entry() : key(U{}), isOccupied(false), dummy(0) {} // default constructor

            Entry(Entry&& other) noexcept : key(static_cast<U&&>(other.key)), isOccupied(other.isOccupied), dummy(0) {
                if (other.isOccupied) { new (&value) T(static_cast<T&&>(other.value)); }
                other.isOccupied = false;
            }
            Entry(const Entry& other) noexcept : key(other.key), isOccupied(other.isOccupied), dummy(0) {
                if (other.isOccupied) { new (&value) T(other.value); }
            }
            Entry& operator=(Entry&& other) noexcept { // BigO(1)
                if (this != &other) {
                    key = static_cast<U&&>(other.key);
                    if (isOccupied) {
                        value.~T();
                    }
                    if (other.isOccupied) {
                        new (&value) T(static_cast<T&&>(other.value));
                    }
                    isOccupied = other.isOccupied;
                    other.isOccupied = false;
                }
                return *this;
            }
            Entry& operator=(const Entry& other) noexcept { // BigO(1)
                if (this != &other) {
                    key = other.key;
                    if (isOccupied) {
                        value.~T();
                    }
                    if (other.isOccupied) {
                        new (&value) T(other.value);
                    }
                    isOccupied = other.isOccupied;
                }
                return *this;
            }
            ~Entry() {
                if (isOccupied) {
                    value.~T();
                }
                key = U{};
                isOccupied = false;
            }
            friend std::ostream& operator<<(std::ostream& os, const Entry& entry) {
                if (!entry.isOccupied) { os << "NULL"; }
                else {
                    os << "Value: " << entry.value << " - Key: " << entry.key << " ";
                }
                return os;
            }
        };
        mLib::Array<Entry> table;
        size_t size;
        bool autoRehash;
        size_t getHashIndex(const U& key) const {
            size_t converted_key = mLib::Hash<U>{}(key);
            return converted_key % mLib::max(table.getSize(), static_cast<size_t>(1));
        }
        inline void next(size_t& index) const {
            index = (index + 1) % mLib::max(table.getSize(), static_cast<size_t>(1));
        }
        void shiftingDelete(size_t removeIndex) { // BigO(n)
            if (removeIndex >= table.getSize() || removeIndex < 0) { throw std::out_of_range("Index out of bounds: Attempted to remove index " + std::to_string(removeIndex) + ", but capacity is " + std::to_string(table.getSize()) + "."); }
            table[removeIndex].~Entry();
            size_t i = removeIndex;
            for (next(i); table[i].isOccupied; next(i)) {
                if (table[i].key % table.getSize() != i && isBetweenCircular(removeIndex, getHashIndex(table[i].key), i)) {
                    table[removeIndex] = static_cast<Entry&&>(table[i]);
                    removeIndex = i;
                }
            }
        }
        bool isBetweenCircular(size_t target, size_t keyIndex, size_t current) const {
            return (keyIndex <= target && target < current) || (current < keyIndex && keyIndex <= target) || (target < current && current < keyIndex);
        }
        void add(Entry&& myEntry) { // BigO(n)
            rehash();
            size_t index = getHashIndex(myEntry.key);
            if (table[index].isOccupied == false) {
                table[index] = static_cast<Entry&&>(myEntry);
                size++;
            }
            else {
                while (table[index].isOccupied && table[index].key != myEntry.key) {
                    next(index);
                } 
                if (table[index].isOccupied == false) {
                    size++;
                }
                table[index] = static_cast<Entry&&>(myEntry);
            }
        }
        void move(Entry&& myEntry) { // BigO(n)
            size_t index = getHashIndex(myEntry.key);
            if (table[index].isOccupied == false) {
                table[index] = static_cast<Entry&&>(myEntry);
            }
            else {
                while (table[index].isOccupied) {
                    next(index);
                }
                table[index] = static_cast<Entry&&>(myEntry);
            }
        }
        void rehash() { // BigO(n)
            if (!autoRehash) { throw std::overflow_error("Hash is full (Size: " + std::to_string(size) + " / Capacity: " + std::to_string(getCap()) + "). Rehash is not activated. Consider activating auto-rehash or increasing capacity manually."); }
            if (size >= (getCap() - (getCap() / 4))) {
                LinearHash temp(getCap() * 2);
                for (auto& item : table) {
                    if (!item.isOccupied) { continue; }
                    temp.move(static_cast<Entry&&>(item));
                }
                temp.size = size;
                *this = static_cast<LinearHash&&>(temp);
            }
        }
        void manualRehash(size_t target) { // BigO(n)
            if (target < size) { throw std::invalid_argument("Invalid target capacity: Target capacity (" + std::to_string(target) + ") cannot be less than the current size (" + std::to_string(size) + ")."); }
            LinearHash temp(target);
            for (auto& item : table) {
                if (!item.isOccupied) { continue; }
                temp.move(static_cast<Entry&&>(item));
            }
            temp.size = size;
            *this = static_cast<LinearHash&&>(temp);
        }
    public:
        explicit LinearHash(size_t capacity_param) : table(capacity_param), size(0), autoRehash(true) {}

        LinearHash(size_t capacity_param, bool rehash_param) : table(capacity_param), size(0), autoRehash(rehash_param) {}

        LinearHash(const LinearHash& other) : table(other.table), size(other.size), autoRehash(other.autoRehash) {}

        LinearHash(LinearHash&& other) noexcept : table(static_cast<mLib::Array<Entry>&&>(other.table)), size(other.size), autoRehash(other.autoRehash) {
            other.size = 0;
        }

        LinearHash(std::initializer_list<mLib::pair<T, U>> initList) : table(initList.size() * 2), size(0), autoRehash(true) {
            for (const auto& item : initList) {
                add(item.first, item.second);
            }
        }

        LinearHash& operator=(const LinearHash& other) { // BigO(n)
            if (this != &other) {
                table = other.table;
                size = other.size;
            }
            return *this;
        }

        LinearHash& operator=(LinearHash&& other) noexcept{ // BigO(1)
            if (this != &other) {
                table = static_cast<mLib::Array<Entry>&&>(other.table);
                size = other.size;
            }
            return *this;
        }
        ~LinearHash() { 
            size = 0;
        }
        void add(const T& value, const U& key) { // BigO(n)
            add(Entry(value, key));
        }
        void add(T&& value, U&& key) { // BigO(n)
            add(Entry(static_cast<T&&>(value), static_cast<U&&>(key)));
        }
        void add(const T& value, U&& key) { // BigO(n)
            add(Entry(value, static_cast<U&&>(key)));
        }
        void add(T&& value, const U& key) { // BigO(n)
            add(Entry(static_cast<T&&>(value), key));
        }
        T& get(const U& key) { // BigO(n)
            size_t index, finish;
            finish = index = getHashIndex(key);
            if (isEmpty()) {
                throw std::underflow_error("Hash underflow: Operation not allowed on an empty hash table. Capacity = " + std::to_string(getCap()));
            }
            else {
                while (table[index].isOccupied) {
                    if (table[index].key == key) { return table[index].value; }
                    next(index);
                    if (index == finish) {
                        break;
                    }
                }
                throw std::invalid_argument("Key not found in the hash table.");
            }
        }
        const T& get(const U& key) const { // BigO(n)
            size_t index, finish;
            finish = index = getHashIndex(key);
            if (isEmpty()) {
                throw std::underflow_error("Hash underflow: Operation not allowed on an empty hash table. Capacity = " + std::to_string(getCap()));
            }
            else {
                while (table[index].isOccupied) {
                    if (table[index].key == key) { return table[index].value; }
                    next(index);
                    if (index == finish) {
                        break;
                    }
                }
                throw std::invalid_argument("Key not found in the hash table.");
            }
        }
        T remove(const U& key) { // BigO(n)
            if (isEmpty()) {
                throw std::underflow_error("Hash underflow: Operation not allowed on an empty hash table. Capacity = " + std::to_string(getCap()));
            }
            else {
                size_t index = getHashIndex(key);
                size_t finish = index;
                if(table[index].isOccupied){  
                    do {
                        if (table[index].key == key) {
                            T temp = static_cast<T&&>(table[index].value);
                            shiftingDelete(index);
                            size--;
                            return temp;
                        }
                        next(index);
                    } while (index != finish && table[index].isOccupied);
                }
                throw std::invalid_argument("Key not found in the hash table.");
            }
        }
        void set(const U& key, const T& value) {
            if (isEmpty()) {
                throw std::underflow_error("Hash underflow: Operation not allowed on an empty hash table. Capacity = " + std::to_string(getCap()));
            }
            size_t index = getHashIndex(key);
            size_t finish = index;
            if (table[index].isOccupied) {
                do {
                    if (table[index].key == key) {
                        table[index].value = value;
                    }
                    next(index);
                } while (index != finish && table[index].isOccupied);
            }
            throw std::invalid_argument("Key not found in the hash table.");
        }
        friend std::ostream& operator<<(std::ostream& os, const LinearHash& hash) { // BigO(n)
            for (size_t i = 0; i < hash.table.getSize(); i++) {
                if (hash.table[i].isOccupied == false) {
                    os << "NULL" << " | ";
                }
                else {
                    os << hash.table[i] << " | ";
                }
            }
            return os << std::endl;
        }
        void clear() { // BigO(n)
            for (Entry& item : table) {
                item.~Entry();
            }
            size = 0;
        }
        class Iterator {
        private:
            size_t current_index;
            mLib::Array<Entry>* table_ptr;
        public:
            struct HashProxy {
                T& value;
                const U& key;
                friend std::ostream& operator<<(std::ostream& os, const HashProxy& proxy) {
                    os << "Value: " << proxy.value << " - Key: " << proxy.key << " ";
                    return os;
                }
            };
            using iterator_category = std::forward_iterator_tag;
            using value_type = HashProxy;
            using difference_type = std::ptrdiff_t;
            using reference = HashProxy;
            using pointer = void;

            Iterator(mLib::Array<Entry>* table_ptr_param, size_t index_param) noexcept : table_ptr(table_ptr_param), current_index(index_param){}

            reference operator*() const noexcept{
                return HashProxy{ (*table_ptr)[current_index].value, (*table_ptr)[current_index].key };
            }

            Iterator& operator++() noexcept{
                for (size_t i = current_index + 1; i < table_ptr->getSize(); ++i) {
                    if ((*table_ptr)[i].isOccupied) {
                        current_index = i;
                        return *this;
                    }
                }
                current_index = table_ptr->getSize();
                return *this;
            }
            Iterator operator++(int) noexcept {
                Iterator temp = *this;
                ++(*this);
                return temp;
            }

            bool operator==(const Iterator& other) const noexcept{
                return (this->table_ptr == other.table_ptr) && (this->current_index == other.current_index);
            }
            bool operator!=(const Iterator& other) const noexcept {
                return !(operator==(other));
            }
        };

        class Const_Iterator {
        private:
            size_t current_index;
            const mLib::Array<Entry>* table_ptr;
        public:
            struct HashProxy {
                const T& value;
                const U& key;
                friend std::ostream& operator<<(std::ostream& os, const HashProxy& proxy) {
                    os << "Value: " << proxy.value << " - Key: " << proxy.key << " ";
                    return os;
                }
            };
            using iterator_category = std::forward_iterator_tag;
            using value_type = HashProxy;
            using difference_type = std::ptrdiff_t;
            using reference = const HashProxy;
            using pointer = void;

            Const_Iterator(const mLib::Array<Entry>* table_ptr_param, size_t index_param) noexcept : table_ptr(table_ptr_param), current_index(index_param) {}

            reference operator*() const noexcept {
                return HashProxy{ (*table_ptr)[current_index].value, (*table_ptr)[current_index].key };
            }

            Const_Iterator& operator++() noexcept {
                for (size_t i = current_index + 1; i < table_ptr->getSize(); ++i) {
                    if ((*table_ptr)[i].isOccupied) {
                        current_index = i;
                        return *this;
                    }
                }
                current_index = table_ptr->getSize();
                return *this;
            }
            Const_Iterator operator++(int) noexcept {
                Const_Iterator temp = *this;
                ++(*this);
                return temp;
            }

            bool operator==(const Const_Iterator& other) const noexcept {
                return (this->table_ptr == other.table_ptr) && (this->current_index == other.current_index);
            }
            bool operator!=(const Const_Iterator& other) const noexcept {
                return !(operator==(other));
            }
        };
        Iterator begin() noexcept {
            for (size_t i = 0; i < table.getSize(); ++i) {
                if (table[i].isOccupied) {
                    return Iterator(&table, i);
                }
            }
            return end();
        }
        Iterator end() noexcept {
            return Iterator(&table, this->getCap());
        }
        Const_Iterator begin() const noexcept {
            for (size_t i = 0; i < table.getSize(); ++i) {
                if (table[i].isOccupied) {
                    return Const_Iterator(&table, i);
                }
            }
            return end();
        }
        Const_Iterator end() const noexcept {
            return Const_Iterator(&table, this->getCap());
        }
        Const_Iterator cbegin() const noexcept { return begin(); }
        Const_Iterator cend() const noexcept { return end(); }

        inline bool isEmpty() const noexcept { return size == 0; }
        inline bool isFull() const noexcept { return size == table.getSize(); }
        inline size_t getSize() const noexcept { return size; }
        inline size_t getCap() const noexcept { return table.getSize(); }
        inline void setAutoRehash(bool rehash_param) noexcept { autoRehash = rehash_param; }
        inline bool infoAutoRehash() const noexcept { return autoRehash; }
        inline void setCap(size_t size) const noexcept { manualRehash(size); }
    };
}