#pragma once

#include <iostream>
#include <string>
#include <stdexcept>
#include <random>
#include <initializer_list>

#ifdef DEBUG
#define MLIB_COLOR_RED   "\033[31m"
#define MLIB_COLOR_RESET "\033[0m"
#define DEBUG_LOG(obj_ptr, x) std::clog << MLIB_COLOR_RED << "[DEBUG]obj: " << obj_ptr << " | " <<  x << MLIB_COLOR_RESET << std::endl
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
            T* begin() noexcept{ return array; }
            T* end() noexcept{ return array + capacity; }
            const T* begin() const noexcept{ return array; }
            const T* end() const noexcept{ return array + capacity; }
            int length() const noexcept{ return capacity; }
            friend std::ostream& operator<<(std::ostream& os, const Array<T>& arr) {
                DEBUG_LOG(&arr, "Running Array << operator");
                for (const auto& item : arr) {
                    os << item << " | ";
                }
                os << "END";
                return os;
            }
        };
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
            void destroyerClear() noexcept{
                DEBUG_LOG(this, "Destroying list contents...");
                for (int i = 0; i < size; i++) {
                    array[i].~T();
                }
                size = 0;
                capacity = 0;
                ::operator delete(array);
                array = nullptr;
            }
        public:
            ~List() {
                destroyerClear();
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
                    reSize(capacity == 0 ? 1 : capacity * 2, true);
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
                    reSize(capacity == 0 ? 1 : capacity * 2, true);
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
                DEBUG_LOG(this, "Shuffling the list...");
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
            void clear() noexcept{
                DEBUG_LOG(this, "Clearing list contents...");
                for (int i = 0; i < size; i++) {
                    array[i].~T();
                }
                size = 0;
                if (autoShrink && capacity > 1) { reSize(1, false); }
            }
            T* begin() noexcept{ return array; }
            T* end() noexcept { return array + size; }
            const T* begin() const noexcept { return array; }
            const T* end() const noexcept { return array + size; }
            int getSize() const noexcept { return size; }
            bool isEmpty() const noexcept { return size == 0; }
            int getCapacity() const noexcept{ return capacity; }
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
            void clear() noexcept{ // BigO(n)
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
            Node* getNodeAddress(int index) { // Teoric BigO(n) -> BigO(n/2)
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
                return temp;
            }
            const Node* getNodeAddress(int index) const{ // Teoric BigO(n) -> BigO(n/2)
                return const_cast<Dll*>(this)->getNodeAddress(index);
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
            ~Dll() {
                clear();
            }
            Dll() : head(nullptr), tail(nullptr), size(0) {}
            Dll(const Dll& other) : head(nullptr), tail(nullptr), size(0){
                Node* temp = other.head;
                while (temp != nullptr) {
                    addLast(temp->value);
                    temp = temp->next;
                }
            }
            Dll(Dll&& other) noexcept : head(other.head), tail(other.tail), size(other.size){
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
            Dll& operator=(Dll&& other) noexcept{
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
            bool isEmpty() const noexcept{ return size == 0; }
            int getSize() const noexcept{ return size; }
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
            void insert(int index, T&& value) { // Teoric BigO(n) -> BigO(n/2)
                if(index == 0){
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
            void insert(int index, const T& value) { // Teoric BigO(n) -> BigO(n/2)
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
            const T& get(int index) const { // Teoric BigO(n) -> (n/2)
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
            int indexOf(const T& target) const { // BigO(n)
                checkEmpty();
                Node* temp = head;
                for (int i = 0; i < size; i++) {
                    if (temp->value == target) {
                        return i;
                    }
                    temp = temp->next;
                }
                return -1;
            }
            T remove(int index) { // BigO(n)
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
            void set(int index, const T& value) { // Teoric BigO(n) -> BigO(n/2)
                checkEmpty();
                checkAvailableBounds(index);
                Node* temp = getNodeAddress(index);
                temp->value = value;
            }
            void set(int index, T&& value) { // Teoric BigO(n) -> BigO(n/2)
                checkEmpty();
                checkAvailableBounds(index);
                Node* temp = getNodeAddress(index);
                temp->value = static_cast<T&&>(value);
            }
            void swap(int index1, int index2) { // Teoric BigO(n) -> BigO(n/2)
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
            class Iterator {
            private:
                Node* current;
            public:
                using iterator_category = std::bidirectional_iterator_tag;
                using value_type = T;
                using difference_type = std::ptrdiff_t;
                using pointer = T*;
                using reference = T&;

                Iterator(Node* ptr) : current(ptr){}

                reference operator*() {
                    return current->value;
                }
                pointer operator->() {
                    return &(current->value);
                }
                Iterator& operator++() {
                    if (current != nullptr) {
                        current = current->next;
                    }
                    return *this;
                }
                Iterator operator++(int) {
                    Iterator temp = *this;
                    ++(*this);
                    return temp;
                }
                Iterator& operator--() {
                    if (current != nullptr) {
                        current = current->prev;
                    }
                    return *this;
                }
                bool operator==(const Iterator& other) const {
                    return this->current == other.current;
                }
                bool operator!=(const Iterator& other) const {
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

                Const_Iterator(const Node* ptr) : current(ptr){}

                reference operator*() const{
                    return current->value;
                }
                pointer operator->() const {
                    return &(current->value);
                }
                Const_Iterator& operator++() {
                    if (current != nullptr) {
                        current = current->next;
                    }
                    return *this;
                }
                Const_Iterator operator++(int) {
                    Const_Iterator temp = *this;
                    ++(*this);
                    return temp;
                }
                Const_Iterator operator--() {
                    if (current != nullptr) {
                        current = current->prev;
                    }
                    return *this;
                }
                bool operator==(const Const_Iterator& other) const {
                    return this->current == other.current;
                }
                bool operator!=(const Const_Iterator& other) const {
                    return this->current != other.current;
                }
            };
            Iterator begin() noexcept{
                return Iterator(head);
            }
            Iterator end() noexcept{
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
                        os << put << " | ";
                    }
                    os << "END";
                }
                return os;
            }
        };
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
            void clear() noexcept{ // BigO(n)
                Node* temp = head;
                head = nullptr;
                while (temp != nullptr) {
                    Node* next = temp->next;
                    delete temp;
                    temp = next;

                }
            }
            Node* getNodeAddress(int index) noexcept{ // BigO(n)
                Node* temp = head;
                for (int i = 0; i < index; i++) {
                    temp = temp->next;
                }
                return temp;
            }
            const Node* getNodeAddress(int index) const{ // BigO(n)
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
            Sll(const Sll& other) : head(nullptr), size(0){ // BigO(n)
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
                }catch (...) {
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
            bool isEmpty() const noexcept{ return size == 0; }
            int getSize() const noexcept{ return size; }
            ~Sll() {
                clear();
            }
            void addLast(const T& value) { // BigO(n)
                if (isEmpty()) {
                    head = new Node(value);
                }else {
                    Node* temp = getNodeAddress(size-1);
                    temp->next = new Node(value);
                }
                size++;
            }
            void addLast(T&& value) { // BigO(n)
                if (isEmpty()) {
                    head = new Node(static_cast<T&&>(value));
                }else {
                    Node* temp = getNodeAddress(size-1);
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
            const T& get(int index) const{ // BigO(n)
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
                if(size == 1){ 
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
                }else {
                    checkEmpty();
                    checkAvailableBounds(index);
                    Node* temp = getNodeAddress(index-1);
                    Node* trash = temp->next;
                    temp->next = trash->next;
                    T returnValue = static_cast<T&&>(trash->value);
                    delete trash;
                    size--;
                    return returnValue;
                }
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
            int indexOf(const T& value) { // BigO(n)
                Node* temp = head;
                for (int i = 0; i < size; i++) {
                    if (value == temp) {
                        return i;
                    }
                }
                return -1;
            }
            class Iterator {
                // @TODO: WRITE ME!!!
            };
            class Const_Iterator {
                // @TODO: WRITE ME!!!
            };
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
}