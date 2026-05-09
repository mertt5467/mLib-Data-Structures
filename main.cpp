#include <iostream>
#include <string>

void tryLineerHash();
namespace LineerHash{
    struct Entry{
        int key;
        bool isOccupied;
        std::string value;
        Entry(int key, std::string value): key(key), value(value), isOccupied(true){}
        Entry(): key(0), isOccupied(false){}
        ~Entry() {
            key = 0;
            value.clear();
        }
    };

    class HashTable {
    private:
       Entry* table;
        int capacity;
        int size = 0;
        int getHashCode(int key) {
           return key%capacity;
        }
    public:
        HashTable(int capacity) : capacity(capacity){table = new Entry[capacity];}
        bool isEmpty() {return size == 0;}
        bool isFull() {return size == capacity;}
        ~HashTable() {delete[] table;}

        void put(const Entry& myEntry) {
            if (isFull()) {
                throw std::out_of_range("Hash is full");
            }
            int index = getHashCode(myEntry.key);
            if (table[index].isOccupied == false) {
                table[index] = myEntry;
                size++;
            }else{
                while (table[index].isOccupied && table[index].key != myEntry.key) {
                        index++;
                        if (index == capacity) {index = 0;}
                }if (table[index].isOccupied == false) {table[index] = myEntry; size++;}
                else{table[index].value = myEntry.value;}
            }
        }
        const std::string& get(int key) {
            int index = getHashCode(key);
            if (isEmpty()) {
                throw std::out_of_range("Value is not found");
            }else if (table[index].key == key) {
                return table[index].value;
            }else {
                int start = index;
                while(table[index].isOccupied && table[index].key != key) {
                    index++;
                    if (index == start){break;}
                    if (index == capacity) {index = 0;}
                }if (index == start || table[index].isOccupied == false) {
                    throw std::out_of_range("Value is not found");
                }else{
                    return table[index].value;
                }
            }
        }
        void print() {
            for (int i = 0; i < capacity; i++) {
                if (table[i].isOccupied == false) {
                    std::cout << "NULL" << " | ";
                }else{
                    std::cout << table[i].value << " | ";
                }
            }
            std::cout << std::endl;
        }
    };
}
namespace ChainingHash {
    class LinkedList {
    private:
        struct Node {
            int key;
            std::string value;
            Node* next;
            Node* prev;
            Node(int data, std::string value): key(data), value(value), next(nullptr), prev(nullptr) {}
        };
        Node* head;
        Node* tail;
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
        ~LinkedList() {
            clear();
        }
        LinkedList() : head(nullptr), tail(nullptr){}
        void addFirst(int key, std::string value) {
            Node* myNode = new Node(key, value);
            if (size == 0) {
                head = myNode;
                tail = myNode;
            }else {
                head->prev = myNode;
                myNode->next = head;
                head = myNode;
            }
            size++;
        }
        void addLast(int key, std::string value) {
            Node* myNode = new Node(key, value);
            if (size == 0) {
                head = myNode;
                tail = myNode;
            }else {
                tail->next = myNode;
                myNode->prev = tail;
                tail = myNode;
            }
            size++;
        }
        int getSize() {
            return size;
        }
        const std::string& get(int key) {
            Node* current = head;
            while (current != nullptr) {
                if (current->key == key) {
                    return current->value;
                }
                current = current->next;
            }
            throw std::out_of_range("Key not found");
        }
        void deleteFirst() {
            if (size == 0) {throw std::out_of_range("Empty");}
            else if(size == 1){delete this;}
            else {
                Node* next = head->next;
                delete head;
                head = head->next;
                head->prev = nullptr;   
            }
        }
        void print() {
            if (size == 0) { std::cout << "NULL" << std::endl; return; }
            Node* temp = head;
            while (temp != nullptr) {
                std::cout << temp->value << " -> ";
                temp = temp->next;
            }
            std::cout << "TAIL" << std::endl;
        }
        void printReverse() {
            if (size == 0){std::cout << "NULL" << std::endl; return;}
            Node* temp = tail;
            while (temp != nullptr) {
                std::cout << temp->value << " <- ";
                temp = temp->prev;
            }
            std::cout << "HEAD" << std::endl;
        }
    };
}

int main() {
    tryLineerHash();
}
void tryLineerHash() {
    using namespace LineerHash;
    HashTable h1(10);
    h1.put(Entry(4, "Ali Demir"));
    h1.put(Entry(2, "Fatma Gul"));
    h1.put(Entry(5, "Mehmet Uranyum"));
    h1.put(Entry(6, "Ural Celik"));
    h1.put(Entry(10, "Feyza Cinko"));
    h1.put(Entry(2, "Islam Mayda"));
    h1.print();
    try {
        std::cout << h1.get(3) << std::endl;
    }catch (std::out_of_range& e) {
        std::cout << e.what() << std::endl;
    }
}