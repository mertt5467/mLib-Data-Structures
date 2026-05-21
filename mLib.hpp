#pragma once

#include <iostream>
#include <string>
#include <stdexcept>
#include <random>
#include <initializer_list>

#ifdef DEBUG
#define MLIB_COLOR_RED   "\033[31m"
#define MLIB_COLOR_RESET "\033[0m"
#define DEBUG_LOG(obj_ptr, x) std::cout << MLIB_COLOR_RED << "[DEBUG]" << "obj: " << obj_ptr << " | " <<  x << MLIB_COLOR_RESET << std::endl
#else
#define DEBUG_LOG(obj_ptr, x) void(0)
#endif


using std::to_string;
namespace mLib {
    namespace Arrays {
        template<typename T>
        class Array {
            int capacity;
            T* array;
        public:
            Array(int capacity) : capacity(capacity) { // BigO(n)
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
            Array& operator=(Array&& other) noexcept { // BigO(n)
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
                    }catch (...) {
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
            T* begin() { return array; }
            T* end() { return array + capacity; }
            const T* begin() const { return array; }
            const T* end() const { return array + capacity; }
            int size() const { return capacity; }
        };
        template<typename T>
        std::ostream& operator<<(std::ostream& os, const Array<T>& arr) {
            DEBUG_LOG(&arr, "Running Array << operator");
            for (const auto& item : arr) {
                os << item << " | ";
            }
            os << "END";
            return os;
        }
        template<typename T>
        class List {
        private:
            T* array;
            int capacity;
            int size;
            bool autoShrink;
            List(int capacity, int size, bool autoShrink) : capacity(capacity), size(size), autoShrink(autoShrink) {
                if (capacity <= 0) { throw std::out_of_range("Capacity cannot be 0 or less than 0."); }
                array = static_cast<T*>(::operator new(capacity * sizeof(T)));

                size_t totalBytes = capacity * sizeof(T);
                size_t totalBits = totalBytes * 8; // 1 Byte == 8 Bit
                DEBUG_LOG(this, "List dynamically allocated. Cap = " + to_string(capacity) + " | Size = " + to_string(size) + " | Shrink mode = " + to_string(autoShrink) + " | Total Size = " + to_string(totalBytes) + "Byte (" + to_string(totalBits) + " bits)");
            }

            void reSize(int targetSize, bool extend) { // BigO(n)
                DEBUG_LOG(this, "Running reSize. Target Cap: " << targetSize << " | Extend Mode: " << extend);
                if (capacity == 1 && extend == false) {
                    DEBUG_LOG(this, "Capacity is already 1; no shrink operation executed.");
                    return;
                }
                T* newArray = static_cast<T*>(::operator new(targetSize * sizeof(T)));
                for (int i = 0; i < size; i++) {
                    new (&newArray[i]) T(static_cast<T&&>(array[i]));
                    array[i].~T();
                }
                ::operator delete(array);

                array = newArray;
                capacity = targetSize;
                DEBUG_LOG(this, "reSize completed successfully.");
            }
        public:
            ~List() {
                clear();
                ::operator delete(array);
                DEBUG_LOG(this, "List destroyed. Cleaning up elements and freeing memory.");
            }
            List() : List(1, 0, true) {}
            explicit List(int startCapacity) : List(startCapacity, 0, true) {}
            explicit List(bool autoShrink) : List(1, 0, autoShrink) {}
            List(int startCapacity, bool autoShrink) : List(startCapacity, 0, autoShrink) {}
            List(std::initializer_list<T> initList) : List(initList.size() > 0 ? initList.size() : 1, 0, true) {
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
                    int newCap = other.capacity;
                    int newSize = other.size;
                    bool newShrink = other.autoShrink;

                    T* newArray = static_cast<T*>(::operator new(other.capacity * sizeof(T)));

                    int catchItemCount = 0;
                    try {
                        for (int i = 0; i < newSize; i++) {
                            new (&newArray[i]) T(other[i]);
                            catchItemCount++;
                        }
                    }
                    catch (...) {
                        DEBUG_LOG(this, "[EXCEPTION] Copying failed at index: " << catchItemCount << ". Rolling back and destroying " << catchItemCount << " constructed elements.");
                        for (int i = 0; i < catchItemCount; i++) {
                            newArray[i].~T();
                        }
                        ::operator delete(newArray);
                        throw;
                    }
                    for (int i = 0; i < size; i++) {
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
                    int tempCap = capacity;
                    int tempSize = size;
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
            void reserve(int newCapacity) {
                if (newCapacity > capacity) {
                    reSize(newCapacity, true);
                }
                autoShrink = false;
                DEBUG_LOG(this, "Reserving capacity manually to: " << newCapacity);
            }
            T& operator[](int index) { // BigO(1)
                if (index < 0 || index >= size) { throw std::out_of_range("Index out of bounds. Size = " + to_string(size)); }
                return array[index];
            }
            const T& operator[](int index) const { // BigO(1)
                if (index < 0 || index >= size) { throw std::out_of_range("Index out of bounds. Size = " + to_string(size)); }
                return array[index];
            }
            void add(const T& value) { // (Amortized O(1))
                add(size, value);
            }
            void add(T&& value) { // (Amortized O(1))
                add(size, static_cast<T&&>(value));
            }
            void add(int index, const T& value) { // BigO(n)
                DEBUG_LOG(this, "Adding L-Value at index: " << index);
                if (index < 0 || index > size) { throw std::out_of_range("Index out of bounds. Size = " + to_string(size)); }
                else if (size == capacity) {
                    reSize(capacity * 2, true);
                }
                if (index == size) {
                    new (&array[index]) T(value);
                }
                else {
                    new (&array[size]) T(static_cast<T&&>(array[size - 1]));
                    for (int i = size - 1; i > index; i--) {
                        array[i] = static_cast<T&&>(array[i - 1]);
                    }
                    array[index] = value;

                }
                size++;
            }
            void add(int index, T&& value) { // BigO(n)
                DEBUG_LOG(this, "Adding R-Value (Move) at index: " << index);
                if (index < 0 || index > size) { throw std::out_of_range("Index out of bounds. Size = " + to_string(size)); }
                else if (size == capacity) {
                    reSize(capacity * 2, true);
                }
                if (index == size) {
                    new (&array[index]) T(static_cast<T&&>(value));
                }
                else {
                    new (&array[size]) T(static_cast<T&&>(array[size - 1]));
                    for (int i = size - 1; i > index; i--) {
                        array[i] = static_cast<T&&>(array[i - 1]);
                    }
                    array[index] = static_cast<T&&>(value);

                }
                size++;
            }
            const T& get(int index) const { // BigO(1)
                if (index < 0 || index >= size) { throw std::out_of_range("Index out of bounds. Size = " + to_string(size)); }
                return array[index];
            }
            int indexOf(const T& value) const { // BigO(n)
                for (int i = 0; i < size; i++) {
                    if (array[i] == value) {
                        return i;
                    }
                }
                return -1;
            }
            void set(int index, T&& value) { // BigO(1)
                DEBUG_LOG(this, "Setting R-Value (Move) at index: " << index);
                if (index < 0 || index >= size) { throw std::out_of_range("Index out of bounds. Size = " + to_string(size)); }
                else {
                    array[index] = static_cast<T&&>(value);
                }
            }
            void set(int index, const T& value) { // BigO(1)
                DEBUG_LOG(this, "Setting L-Value at index: " << index);
                if (index < 0 || index >= size) { throw std::out_of_range("Index out of bounds. Size = " + to_string(size)); }
                else {
                    array[index] = value;
                }
            }
            void shuffle() {
                DEBUG_LOG(this, "Shuffling the list.");
                if (isEmpty() || size == 1) { // BigO(n)
                    return;
                }
                std::random_device rd;
                std::mt19937 gen(rd());
                std::uniform_int_distribution<int> distr;
                for (int i = size - 1; i > 0; i--) {
                    int randomNumber = distr(gen, std::uniform_int_distribution<int>::param_type(0, i));
                    swap(i, randomNumber);
                }
            }
            void swap(int index1, int index2) { // BigO(1)
                DEBUG_LOG(this, "Swapping elements at indices: " << index1 << " & " << index2);
                if (index1 < 0 || index1 >= size || index2 < 0 || index2 >= size) { throw std::out_of_range("Index out of bounds. Size = " + to_string(size)); }
                else if (index1 == index2) { return; }
                else {
                    T temp = static_cast<T&&>(array[index1]);
                    array[index1] = static_cast<T&&>(array[index2]);
                    array[index2] = static_cast<T&&>(temp);
                }
            }
            T remove() {
                DEBUG_LOG(this, "Removing element from end (pop_back). Target index: " << size - 1);
                if (size == 0) {
                    throw std::out_of_range("List is empty.");
                }
                else {
                    T temp = static_cast<T&&>(array[size - 1]);
                    array[size - 1].~T();
                    size--;
                    if (size > 0 && size < (capacity / 4) && autoShrink) {
                        reSize(capacity / 2, false);
                    }
                    return temp;
                }
            }
            T remove(int index) { // BigO(n)
                DEBUG_LOG(this, "Removing element at index: " << index);
                if (index < 0 || index >= size) { throw std::out_of_range("Index out of bounds. Size = " + to_string(size)); }
                else {
                    T temp = static_cast<T&&>(array[index]);
                    for (int i = index; i < size - 1; i++) {
                        array[i] = static_cast<T&&>(array[i + 1]);
                    }
                    array[size - 1].~T();
                    size--;
                    if (size > 0 && size < (capacity / 4) && autoShrink) {
                        reSize(capacity / 2, false);
                    }
                    return temp;
                }
            }
            void sort() {
                DEBUG_LOG(this, "Sorting the list (TODO).");
                // @TODO: WRITE ME!!! (MERGE SORT OR QUICK SORT)
            }
            T& front() { // BigO(1)
                if (size == 0) {
                    throw std::out_of_range("List is empty.");
                }
                else {
                    return array[0];
                }
            }
            const T& front() const { // BigO(1)
                if (size == 0) {
                    throw std::out_of_range("List is empty.");
                }
                else {
                    return array[0];
                }
            }
            T& back() { // BigO(1)
                if (size == 0) {
                    throw std::out_of_range("List is empty.");
                }
                else {
                    return array[size - 1];
                }
            }
            const T& back() const { // BigO(1)
                if (size == 0) {
                    throw std::out_of_range("List is empty.");
                }
                else {
                    return array[size - 1];
                }
            }
            void clear() {
                DEBUG_LOG(this, "Clearing list contents.");
                for (int i = 0; i < size; i++) {
                    array[i].~T();
                }
                size = 0;
                if (autoShrink) { reSize(1, false); }
            }
            T* begin() { return array; }
            T* end() { return array + size; }
            const T* begin() const { return array; }
            const T* end() const { return array + size; }
            int getSize() const { return size; }
            bool isEmpty() const { return size == 0; }
            int getCapacity() const { return capacity; }
            bool infoAutoShrink() const { return autoShrink; }
            void setAutoShrink(bool autoShrink) { DEBUG_LOG(this, "AutoShrink modified to: " << autoShrink); this->autoShrink = autoShrink; if (size > 0 && size < (capacity / 4) && autoShrink) { reSize(capacity / 2, false); } }
        };
        template<typename T>
        std::ostream& operator<<(std::ostream& os, const List<T>& list) {
            DEBUG_LOG(&list, "Running List << operator");
            if (list.getSize() == 0) {
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
    }
    namespace Hash {
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
        class ChainingHash {
            // @TODO: WRITE ME!!! (SLL)
        };
    }
    namespace LinkedList {
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
            int size;
            void clear() {
                Node* temp = head;
                head = nullptr;
                tail = nullptr;
                while (temp != nullptr) {
                    Node* next = temp->next;
                    delete temp;
                    temp = next;
                }
            }
        public:
            ~Dll() {
                clear();
            }
            Dll() : head(nullptr), tail(nullptr), size(0) {}
            bool isEmpty() const { return size == 0; }
            int getSize() const { return size; }
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
            void addFirst(T&& value) {
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
            void insertAt(int index, T&& value) {
                if (index < 0 || index > size) { throw std::out_of_range("Index out of bounds. Size = " + std::to_string(size)); }
                else if(index == 0){
                    addFirst(static_cast<T&&>(value));
                }
                else if (index == size) {
                    addLast(static_cast<T&&>(value));
                }
                else {
                    Node* temp = nullptr;
                    if (index < size / 2) {
                        temp = head;
                        for (int i = 0; i < index; i++) {
                            temp = temp->next;
                        }
                    }
                    else {
                        temp = tail;
                        for (int i = size - 1; i > index; i--) {
                            temp = temp->prev;
                        }
                    }
                    Node* myNode = new Node(static_cast<T&&>(value));

                    myNode->next = temp;
                    myNode->prev = temp->prev;
                    myNode->prev->next = myNode;
                    temp->prev = myNode;
                    size++;
                }
            }
            void insertAt(int index, const T& value) {
                if (index < 0 || index > size) { throw std::out_of_range("Index out of bounds. Size = " + std::to_string(size)); }
                else if (index == 0) {
                    addFirst(value);
                }
                else if (index == size) {
                    addLast(value);
                }
                else {
                    Node* temp = nullptr;
                    if (index < size / 2) {
                        temp = head;
                        for (int i = 0; i < index; i++) {
                            temp = temp->next;
                        }
                    }
                    else {
                        temp = tail;
                        for (int i = size - 1; i > index; i--) {
                            temp = temp->prev;
                        }
                    }
                    Node* myNode = new Node(value);

                    myNode->next = temp;
                    myNode->prev = temp->prev;
                    myNode->prev->next = myNode;
                    temp->prev = myNode;
                    size++;
                }
            }
            const T& get(int index) const { // BigO(n)
                if (isEmpty()) { throw std::out_of_range("List is Empty."); }
                else if (index < 0 || index >= size) { throw std::out_of_range("Index out of bounds. Size = " + std::to_string(size)); }
                Node* temp = head;
                int i = 0;
                while (temp != nullptr && i < index) {
                    temp = temp->next;
                    i++;
                }
                return temp->value;
            }
            T removeFirst() { // BigO(1)
                if (isEmpty()) { throw std::out_of_range("List is empty"); }
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
                if (isEmpty()) { throw std::out_of_range("List is empty"); }
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
            void print() const {
                if (size == 0) { std::cout << "NULL" << std::endl; return; }
                Node* temp = head;
                while (temp != nullptr) {
                    std::cout << temp->value << " -> ";
                    temp = temp->next;
                }
                std::cout << "TAIL" << std::endl;
            }
            void printReverse() const {
                if (size == 0) { std::cout << "NULL" << std::endl; return; }
                Node* temp = tail;
                while (temp != nullptr) {
                    std::cout << temp->value << " <- ";
                    temp = temp->prev;
                }
                std::cout << "HEAD" << std::endl;
            }
            int indexOf(const T& target) const { // BigO(n)
                if (isEmpty()) { throw std::out_of_range("List is Empty."); }
                Node* temp = head;
                for (int i = 0; i < size; i++) {
                    if (temp->value == target) {
                        return i;
                    }
                    temp = temp->next;
                }
                throw std::out_of_range("Value is not found.");
            }
            T remove(int index) { // BigO(n)
                Node* target = nullptr;
                if (isEmpty()) { throw std::out_of_range("List is empty."); }
                else if (index < 0 || index >= size) { throw std::out_of_range("Index out of bounds. Size = " + std::to_string(size)); }
                else if (head->next == nullptr) {
                    target = head;
                    head = nullptr;
                    tail = nullptr;
                }
                else if (index == 0) {
                    target = head;
                    head = head->next;
                    head->prev = nullptr;
                }
                else {
                    Node* temp = head;
                    for (int step = 0; step <= index; step++) {
                        if (index == step) {
                            if (temp->next == nullptr) {
                                target = temp;
                                temp->prev->next = nullptr;
                                tail = temp->prev;
                            }
                            else {
                                target = temp;
                                temp->prev->next = temp->next;
                                temp->next->prev = temp->prev;
                            }
                        }
                        temp = temp->next;
                    }
                }
                T returnValue = static_cast<T&&>(target->value);

                delete target;
                size--;
                return returnValue;
            }
        };
        template<typename T>
        class Sll {
        private:
            struct Node {
                T value;
                Node* next;

                Node(T value) noexcept : value(static_cast<T&&>(value)), next(nullptr) {}

                Node(Node&& other) noexcept : value(static_cast<T&&>(other.value)), next(nullptr) {
                    other.next = nullptr;
                }

                Node(Node& other) : value(other.value), next(nullptr) {
                    // Empty
                }

                Node& operator=(Node&& other) noexcept { // BigO(1)
                    if (this != &other) {
                        value = static_cast<T&&>(other.value);
                    }
                    return *this;
                }

                Node& operator=(const Node& other) noexcept { // BigO(1)
                    if (this != &other) {
                        value = other.value;
                    }
                    return *this;
                }
            };
            Node* head;
            int size;
            void clear() {
                Node* temp = head;
                head = nullptr;
                while (temp != nullptr) {
                    Node* next = temp->next;
                    delete temp;
                    temp = next;

                }
            }
        public:
            Sll() : head(nullptr), size(0) {}
            bool isEmpty() const { return size == 0; }
            int getSize() const { return size; }
            ~Sll() {
                clear();
            }
            // @TODO: WRITE ME!!! (SLL)
        };
    }
}