#define DEBUG
#include "mLib.hpp"
// @TEST !!! [Hash::LineerHash]
/*
using mLib::Hash::LineerHash;
using mLib::Hash::Entry;
static HashTable<std::string>* h1 = new HashTable<std::string>(10);
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
    h1->put(Entry<std::string>(28, e1));
    h1->remove(12);
    try {
        std::cout << h1->get(3) << std::endl;
    }
    catch (std::out_of_range& e) {
        std::cout << e.what() << std::endl;
    }
}
*/
// @TEST !!! [Arrays::List]
using mLib::Arrays::List;
using std::string;
struct myVector {
    string name;
    int x;
    int y;
    int z;
    myVector(){}
    myVector(string name, int x, int y, int z) : name(name), x(x), y(y), z(z) {}
    bool operator==(const myVector& other) const{
        return (this->x == other.x) && (this->y == other.y) && (this->z == other.z);
    }
    bool operator!=(const myVector& other) const {
        return !(*this == other);
    }
};
std::ostream& operator<<(std::ostream& os, const myVector& vector) {
    os << vector.name << " -> [" << vector.x << ", " << vector.y << ", " << vector.z << "]";
    return os;
}
void tryList() {
    myVector v1("v1", 20, -29, 50);
    myVector v2("v2", 20, -29, 50);
    myVector v3("v3", 25, 95, 28);
    myVector v4("v4", 34, 21, -4);
    List<myVector> l1 = { v1, v2, v3 };
    l1.add(v4);
    /*
    if (v1 == v3) {
        std::cout << "true ";
    }
    else {
        std::cout << "false ";
    }
    */
    std::cout << l1;
}

int main() {
    tryList();
    return 0;
}
