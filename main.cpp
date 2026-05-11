#include <iostream>
#include <string>
#include <stdexcept>

void tryLineerHash();
namespace LineerHash{
    template<typename T>
    struct Entry{
        int key;
        bool isOccupied;
        T value;
        Entry(int key, T value): key(key), value(value), isOccupied(true){}
        Entry(): key(0), isOccupied(false){}

        Entry(Entry&& other) noexcept : key(other.key), isOccupied(other.isOccupied), value(static_cast<T&&>(other.value)) {
            other.isOccupied = false;
            other.key = 0;
        }
        Entry(Entry& other) noexcept : key(other.key), isOccupied(other.isOccupied), value(static_cast<T&>(other.value)) {
            // Empty
        }
        Entry& operator=(Entry&& other) noexcept {
            if (this != &other) {
                key = other.key;
                isOccupied = other.isOccupied;
                value = static_cast<T&&>(other.value);
                other.isOccupied = false;
                other.key = 0;
            }
            return *this;
        }
        Entry& operator=(const Entry& other) noexcept {
            if (this != &other) {
                key = other.key;
                isOccupied = other.isOccupied;
                value = other.value;
            }
            return *this;
        }
    };
    template<typename T>
    class HashTable {
    private:
       Entry<T>* table;
        int capacity;
        int size = 0;
        int getHashCode(unsigned int key) const{
           return key % capacity;
        }
        inline void next(int& index) const{
            index = (index + 1) % capacity;
        }
        void shiftingDelete(int removeIndex) {
            if (removeIndex >= capacity || removeIndex < 0) { throw std::out_of_range("Index out of bounds"); }
            table[removeIndex].isOccupied = false;
            int i = removeIndex;
            for (next(i); table[i].isOccupied; next(i)) {
                if (table[i].key % capacity != i &&  isBetweenCircular(removeIndex, getHashCode(table[i].key), i)) {
                    table[removeIndex] = static_cast<Entry<T>&&>(table[i]);
                    table[i].isOccupied = false;
                    removeIndex = i;
                }
            }
        }
        bool isBetweenCircular(int target, int keyIndex, int current) const{
            return (keyIndex <= target && target < current) || (current < keyIndex && keyIndex <= target) || (target < current && current < keyIndex);
        }
    public:
        HashTable(int capacity) : capacity(capacity){ table = new Entry<T>[capacity]; }
        bool isEmpty() const{return size == 0;}
        bool isFull() const{return size == capacity;}
        ~HashTable() {delete[] table;}

        void put(const Entry<T>& myEntry) {
            if (isFull()) {
                throw std::out_of_range("Hash is full");
            }
            int index = getHashCode(myEntry.key);
            if (table[index].isOccupied == false) {
                table[index] = myEntry;
                size++;
            }else{
                while (table[index].isOccupied && table[index].key != myEntry.key) {
                    next(index);
                } 
                if (table[index].isOccupied == false) {
                    size++;
                }
                table[index] = myEntry;
            }
        }
        const T& get(int key) const{
            int index, finish;
            finish = index = getHashCode(key);
            if (isEmpty()) {
                throw std::out_of_range("Hash is Empty");
            }else {
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
        void print() const{
            for (int i = 0; i < capacity; i++) {
                if (table[i].isOccupied == false) {
                    std::cout << "NULL" << " | ";
                }else{
                    std::cout << table[i].value << " | ";
                }
            }
            std::cout << std::endl;
        }
        T remove(int key) {
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
namespace LinkedList {
    template<typename T>
    class Dll {
    private:
        struct Node {
            T value;
            Node* next;
            Node* prev;
            Node(T value) : value(value), next(nullptr), prev(nullptr) {}
            
        };
        Node* head = nullptr;
        Node* tail = nullptr;
        int size = 0;
        void clear() {
            Node* current = head;
            head = nullptr;
            tail = nullptr;
            while (current != nullptr) {
                Node* next = current->next;
                delete current;
                current = next;
            }
        }
    public:
        ~Dll() {
            clear();
        }
        Dll() : head(nullptr), tail(nullptr) {}
        void addFirst(T& value) {
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
        void addLast(T& value) {
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
        int getSize() const {
            return size;
        }
        const T& get(int index) const {
            if (index < 0) { throw std::out_of_range("Invalid Index, cannot be less than 0."); }
            else if(index >= size){ throw std::out_of_range("Index out of bounds. List size = " + size); }
            Node* current = head;
            int i = 0;
            while (current != nullptr) {
                if (i == index) {return current->value;}
                current = current->next;
                i++;
            }
        }
        void deleteFirst() {
            if (size == 0) { throw std::out_of_range("Empty"); }
            else if (size == 1) {
                delete head;
                head = nullptr;
                tail = nullptr;
                size = 0;
            }
            else {
                Node* next = head->next;
                delete head;
                head = next;
                head->prev = nullptr;
                size--;
            }
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
        // @TODO
        T remove() {

        }
    };
}
namespace ChainingHash {
    
}
using namespace LineerHash;
static HashTable<std::string>* h1 = new HashTable<std::string>(10);
int main() {
    tryLineerHash();
    h1->print();
    delete h1;
    return 0;
}
void tryLineerHash() {
    h1->put(Entry<std::string>(4, "Ali Demir"));
    h1->put(Entry<std::string>(2, "Fatma Bor"));
    h1->put(Entry<std::string>(5, "Mehmet Uranyum"));
    h1->put(Entry<std::string>(6, "Ural Celik"));
    h1->put(Entry<std::string>(10, "Feyza Cinko"));
    h1->put(Entry<std::string>(12, "Islam Mayda"));
    h1->put(Entry<std::string>(22, "Eren Zumrut"));
    h1->print();
    h1->put(Entry<std::string>(7, "Sego Hidrojen"));
    h1->put(Entry<std::string>(1, "Yagmur Komur"));
    const std::string& e1 = h1->get(12);
    h1->put(Entry<std::string> (28, e1));
    h1->remove(12);
    try {
        std::cout << h1->get(3) << std::endl;
    }catch (std::out_of_range& e) {
        std::cout << e.what() << std::endl;
    }
}