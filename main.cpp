#define DEBUG
#include "mLib.hpp"

using std::string;
struct myVector {
    string name;
    int x;
    int y;
    int z;
    myVector() : name("vx"), x(0), y(0), z(0) {}
    myVector(string name, int x, int y, int z) : name(name), x(x), y(y), z(z) {}
    bool operator==(const myVector& other) const{
        return (this->x == other.x) && (this->y == other.y) && (this->z == other.z);
    }
    bool operator!=(const myVector& other) const {
        return !(*this == other);
    }
    myVector& operator++() {
        x *= 2;
        y *= 2;
        z *= 2;
        return *this;
    }
    myVector operator++(int) {
        myVector prev = *this;
        ++(*this);
        return prev;
    }
    friend std::ostream& operator<<(std::ostream& os, const myVector& vector) {
        os << vector.name << " -> [" << vector.x << ", " << vector.y << ", " << vector.z << "]";
        return os;
    }
};
// @TEST !!! [Hash::LineerHash] (@REFACTOR)
using mLib::Hash::LineerHash;
using mLib::Hash::Entry;
void tryLineerHash() {
    LineerHash<std::string>* h1 = new LineerHash<std::string>(10);
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
    h1->put(Entry<std::string>(28, e1));
    h1->remove(12);
    try {
        std::cout << h1->get(3) << std::endl;
    }
    catch (std::out_of_range& e) {
        std::cout << e.what() << std::endl;
    }
}
// @TEST !!! [Arrays::List]
using mLib::Arrays::List;
void tryList() {
    myVector v1("v1", 20, -29, 50);
    myVector v2("v2", 20, -29, 50);
    myVector v3("v3", 25, 95, 28);
    myVector v4("v4", 34, 21, -4);
    List<myVector> l1 = { v1, v2, v3 };
    l1.add(v4);
    std::cout << l1 << std::endl;
}
// @TEST !!! [LinkedList::Dll]
using mLib::LinkedList::Dll;
void tryDll() {
    myVector v1("v1", 20, -29, 50);
    myVector v2("v2", 20, -29, 50);
    myVector v3("v3", 25, 95, 28);
    myVector v4("v4", 34, 21, -4);
    myVector v5("v5", 100, 86, -28);
    myVector v6("v6", 76, 3, -9);
    myVector v7("v7", 24, 67, 21);
    myVector v8("v8", 32, 41, -2);
    myVector v9("v9", 100, 86, -28);
    Dll<myVector> dll1 = { v2, v3 };
    dll1.addFirst(v1);
    dll1.addLast(v4);
    dll1.addLast(v6);
    dll1.addLast(v5);
    dll1.swap(dll1.indexOf(v5), dll1.indexOf(v6));
    try {
        dll1.remove(dll1.indexOf(v7));
    }catch (std::exception& e) {
        std::cout << e.what() << std::endl;
    }
    dll1.addLast(v8);
    dll1.addLast(v9);
    dll1.addLast(myVector("v10", 23, 34, 1));
    dll1.insert(dll1.indexOf(v8),v7);
    // std::cout << dll1 << std::endl;
    for (auto& item : dll1) {
        std::cout << item++ << std::endl;
    }
}
int main() {
    tryDll();
    return 0;
}
