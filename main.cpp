#define DEBUG
#include <iostream>
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
struct myObj {
    string name;
    int value;

    myObj(string name, int value) : name(name), value(value) {}

    bool operator>(const myObj& other) const noexcept{
        return (this->value) > (other.value);
    }
    bool operator==(const myObj& other) const noexcept {
        return (this->value) == (other.value);
    }
    bool operator<(const myObj& other) const noexcept {
        return (this->value) < (other.value);
    }
    bool operator<=(const myObj& other) const noexcept {
        return (this->value) <= (other.value);
    }
    bool operator>=(const myObj& other) const noexcept {
        return (this->value) >= (other.value);
    }
    friend std::ostream& operator<<(std::ostream& os, const myObj& put) {
        os << "[ ";
        if (put.name == "") { os << "NULL"; }
        else { os << put.name << " - " << put.value << "$"; }
        os << " ]";
        return os;
    }
};
// @TEST !!! [mLib::LineerHash] (@DEPRECATED)
using mLib::LineerHash;
using mLib::Entry;
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
// @TEST !!! [mLib::Array]
void tryArray() {
    myObj o1("Apple", 5);
    myObj o2("Pear", 10);
    myObj o3("Banana", 14);
    myObj o4("Avocado", 25);
    myObj o5("Tomato", 8);
    myObj o6("Watermelon", 30);
    myObj o7("Stawberry", 28);
    mLib::Array<myObj> a1(10, myObj("", 0));
    try {
        a1[0] = o1;
        a1[1] = o2;
        a1[2] = o3;
        a1[3] = o4;
        a1[4] = o5;
        a1[5] = o6;
        a1[6] = o7;
    }
    catch(std::exception& e){
        std::cout << e.what() << std::endl;
    }
    mLib::mergeSort(a1.begin(), mLib::itNext(a1.begin(), 7), [](auto& a, auto& b) {
        return a > b;
        });
    for (auto& item : a1) {
        std::cout << item << std::endl;
    }
}
// @TEST !!! [mLib::List]
using mLib::List;
void tryList() {
    myVector v1("v1", 20, -29, 50);
    myVector v2("v2", 23, -29, 50);
    myVector v3("v3", 25, 95, 28);
    myVector v4("v4", 34, 21, -4);
    myVector v5("v5", 100, 86, -28);
    myVector v6("v6", 76, 3, -9);
    myVector v7("v7", 24, 67, 21);
    myVector v8("v8", 32, 41, -2);
    myVector v9("v9", 256, 216, -238);
    myVector v10("v10", 132, 16, -38);
    List<myVector> l1;
    l1.add(v1);
    l1.add(std::move(v2));
    l1.add(std::move(v3));
    l1.add(std::move(v4));
    l1.add(std::move(v5));
    l1.add(v7);
    l1.add(v6);
    l1.swap(l1.indexOf(v7), l1.indexOf(v6));
    l1.add(v9);
    l1.set(l1.indexOf(v9), v8);
    l1.add(v10);
    l1.add(l1.indexOf(v10), v9);
    l1.add(myVector("v11", 32, 41, 01));
    for (auto& vector : l1) {
        std::cout << vector << std::endl;
    }
    std::cout << l1.getRandom() << std::endl;
    l1.clear();
    std::cout << l1.getCap() << std::endl;
    std::cout << l1 << std::endl;
}
void tryList2() {
    myObj* o1 = new myObj("Apple", 5);
    myObj* o2 = new myObj("Pear", 10);
    myObj* o3 = new myObj("Banana", 14);
    myObj* o4 = new myObj("Avocado", 25);
    myObj* o5 = new myObj("Tomato", 8);
    myObj* o6 = new myObj("Watermelon", 30);
    mLib::List<myObj*> l1;
    l1.add(o1);
    l1.add(o2);
    l1.add(o3);
    l1.add(o4);
    l1.add(o5);
    l1.add(o6);
    for (auto& item : l1) {
        std::cout << item << std::endl;
    }
    for (auto& item : l1) {
        std::cout << *item << std::endl;
    }
    l1.sort(mLib::greater<myObj*>{});
    for (auto& item : l1) {
        std::cout << item << std::endl;
    }
    for (auto& item : l1) {
        std::cout << *item << std::endl;
    }
    for (auto& item : l1) {
        delete item;
    }
}
void tryList3() {
    string a = "a1";
    string b = "b2";
    string c = "c3";
    string d = "d4";
    string e = "e5";
    string f = "f6";
    string g = "g7";
    mLib::List<string> l1(1);
    l1.add(a);
    l1.add(b);
    l1.add(c);
    l1.add(d);
    l1.add(e);
    l1.add(f);
    l1.add(g);
    l1.shuffle();
    std::cout << l1 << std::endl;
    l1.sort(mLib::less<string>{});
    std::cout << l1 << std::endl;
}
// @TEST !!! [mLib::Dll]
using mLib::Dll;
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
void tryDll2() {
    myObj* o1 = new myObj("Apple", 5);
    myObj* o2 = new myObj("Pear", 10);
    myObj* o3 = new myObj("Banana", 14);
    myObj* o4 = new myObj("Avocado", 25);
    myObj* o5 = new myObj("Tomato", 8);
    myObj* o6 = new myObj("Watermelon", 30);
    mLib::Dll<myObj*> d1;
    d1.addFirst(o1);
    d1.addFirst(o2);
    d1.addFirst(o3);
    d1.addFirst(o4);
    d1.addFirst(o5);
    d1.addFirst(o6);
    for (auto& item : d1) {
        std::cout << *item << std::endl;
    }
    d1.sort([](auto* a, auto* b) {
        return *a > *b;
    });
    std::cout << " ------ " << std::endl;
    for (auto& item : d1) {
        std::cout << *item << std::endl;
    }
    for (auto& item : d1) {
        delete item;
    }
}
// @TEST !!! [mLib::Sll]
void trySll() {
    myObj* o1 = new myObj("Apple", 5);
    myObj* o2 = new myObj("Pear", 10);
    myObj* o3 = new myObj("Banana", 14);
    myObj* o4 = new myObj("Avocado", 25);
    myObj* o5 = new myObj("Tomato", 8);
    myObj* o6 = new myObj("Watermelon", 30);
    mLib::Sll<myObj*> s1;
    s1.addFirst(o1);
    s1.addFirst(o2);
    s1.addFirst(o3);
    s1.addLast(o4);
    s1.addFirst(o5);
    s1.addFirst(o6);
    for (auto& item : s1) {
        std::cout << *item << std::endl;
    }
    std::cout << " ----- " << std::endl;
    s1.sort([](auto* a, auto* b) {
        return *a < *b;
    });
    for (auto& item : s1) {
        std::cout << *item << std::endl;
    }
    for (auto& item : s1) {
        delete item;
    }
}
// @TEST !!! [mLib::Cll]
void tryCll() {
    myObj* o1 = new myObj("Apple", 5);
    myObj* o2 = new myObj("Pear", 10);
    myObj* o3 = new myObj("Banana", 14);
    myObj* o4 = new myObj("Avocado", 25);
    myObj* o5 = new myObj("Tomato", 8);
    myObj* o6 = new myObj("Watermelon", 30);
    myObj* o7 = new myObj("Stawberry", 28);
    mLib::Cll<myObj*> c1;
    c1.addFirst(o1);
    c1.addFirst(o2);
    c1.addFirst(o3);
    c1.addFirst(o4);
    c1.addLast(o5);
    c1.insert(o6, 2);
    c1.addFirst(o7);
    std::cout << c1 << std::endl;
    std::cout << "-----" << std::endl;
    for (const auto& item : c1) {
        std::cout << *item << std::endl;
    }
    c1.shuffle();
    std::cout << "-----" << std::endl;
    for (const auto& item : c1) {
        std::cout << *item << std::endl;
    }
    c1.sort([](auto* a, auto* b) {
        return *a > *b;
    });
    std::cout << "-----" << std::endl;
    for (const auto& item : c1) {
        std::cout << *item << std::endl;
    }
    for (auto& item : c1) {
        delete item;
    }
}
// @TEST !!! [mLib::Stack]
void tryStack() {
    myObj o1("Apple", 5);
    myObj o2("Pear", 10);
    myObj o3("Banana", 14);
    myObj o4("Avocado", 25);
    myObj o5("Tomato", 8);
    myObj o6("Watermelon", 30);
    myObj o7("Stawberry", 28);
    mLib::Stack<myObj> s1(6);
    try {
        s1.push(o1);
        s1.push(o2);
        s1.push(o3);
        s1.push(o4);
        s1.push(o5);
        s1.push(o6);
        s1.setCap(7);
        s1.push(o7);
    }
    catch (std::exception& e) {
        std::cout << e.what() << std::endl;
    }
    std::cout << (s1.peek())<< std::endl;
    std::cout << std::endl;
    std::cout << s1 << std::endl;
}
// @TEST !!! [mLib::Queue]
void tryQueue() {
    myObj o1("Apple", 5);
    myObj o2("Pear", 10);
    myObj o3("Banana", 14);
    myObj o4("Avocado", 25);
    myObj o5("Tomato", 8);
    myObj o6("Watermelon", 30);
    myObj o7("Stawberry", 28);
    mLib::Queue<myObj> q1(4);
    try {
        q1.enqueue(o1);
        q1.enqueue(o2);
        q1.enqueue(o3);
        q1.enqueue(o4);
        q1.setCap(10);
        q1.enqueue(o5);
        q1.enqueue(o6);
        q1.enqueue(o7);
    }
    catch (std::exception& e) {
        std::cout << e.what() << std::endl;
    }
    std::cout << (q1.front()) << std::endl;
    std::cout << std::endl;
    std::cout << q1 << std::endl;
}
// @TEST !!! [mLib::PriorityQueue]
void tryPriorityQueue() {
    myObj o1("Apple", 1);
    myObj o2("Pear", 2);
    myObj o3("Banana", 3);
    myObj o4("Avocado", 4);
    myObj o5("Tomato", 5);
    myObj o6("Watermelon", 6);
    myObj o7("Stawberry", 6);
    mLib::PriorityQueue<myObj, double, mLib::greater<double>> h1;

    h1.enqueue(o7, 7);
    h1.enqueue(o5, 5);
    h1.enqueue(o2, 2);
    h1.enqueue(o3, 3);
    h1.enqueue(o1, 1);
    h1.enqueue(o6, 6);
    h1.enqueue(o4, 4);

    std::cout << h1 << std::endl;
    std::cout << "---" << std::endl;
    h1.printConsole();
    std::cout << "---" << std::endl;
    h1.printConsoleBottomUp();
}
// @TEST !!! [mLib::Heap]
void tryHeap() {
    myObj o1("Apple", 5);
    myObj o2("Pear", 10);
    myObj o3("Banana", 14);
    myObj o4("Avocado", 25);
    myObj o5("Tomato", 8);
    myObj o6("Watermelon", 30);
    myObj o7("Stawberry", 28);
    mLib::Heap<myObj, mLib::greater<myObj>> h1;

    h1.add(o2);
    h1.add(o3);
    h1.add(o4);
    h1.add(o1);
    h1.add(o5);
    h1.add(o6);
    h1.add(o7);

    std::cout << h1 << std::endl;

    h1.printConsole();

    h1.printConsoleBottomUp();
}
// @TEST !!! [mLib::Bst]
void tryBst() {
    myObj o1("Apple", 5);
    myObj o2("Pear", 10);
    myObj o3("Banana", 14);
    myObj o4("Avocado", 25);
    myObj o5("Tomato", 8);
    myObj o6("Watermelon", 30);
    myObj o7("Stawberry", 28);
    myObj o8("Cherry", 18);
    myObj o9("Pumpkin", 35);
    myObj o10("Melon", 29);

    mLib::Bst<myObj> b1;

    b1.add(o2);
    b1.add(o1);
    b1.add(o4);
    b1.add(o3);
    b1.add(o5);
    b1.add(o7);
    b1.add(o6);

    for (const auto& item : b1) {
        std::cout << item << " ";
    }
    std::cout << std::endl;

    mLib::Bst<myObj> b2;

    b2.add(o8);
    b2.add(o9);
    b2.add(o10);

    b1 = static_cast<mLib::Bst<myObj>&&>(b2);

    b1.printInOrder();
    std::cout << std::endl;
    b1.printPreOrder();
    std::cout << std::endl;
    b1.printPostOrder();
    std::cout << std::endl;
    b1.printLevelOrder();
    std::cout << std::endl;

    std::cout << "------------" << std::endl << std::endl;

    b2.printInOrder();
    std::cout << std::endl;
    b2.printPreOrder();
    std::cout << std::endl;
    b2.printPostOrder();
    std::cout << std::endl;

    std::cout << b1.getHeight() << std::endl;

    mLib::Bst<myObj> b3 = { o2, o1, o4, o9, o5, o10, o6, o3, o7, o8,};
    for (const auto& item : b3) {
        std::cout << item << " ";
    }
    std::cout << b3.getHeight() << std::endl;
    std::cout << std::endl;
}
// @TEST !!! [mLib::Avl]
void tryAvl() {
    myObj o1("Apple", 5);
    myObj o2("Pear", 10);
    myObj o3("Banana", 14);
    myObj o4("Avocado", 25);
    myObj o5("Tomato", 8);
    myObj o6("Watermelon", 30);
    myObj o7("Stawberry", 28);
    myObj o8("Cherry", 18);
    myObj o9("Pumpkin", 35);
    myObj o10("Melon", 29);

    mLib::Avl<myObj> b1;

    b1.add(o1);
    b1.add(o5);
    b1.add(o2);
    b1.add(o3);
    b1.add(o4);
    b1.add(o7);
    b1.add(o6);

    for (const auto& item : b1) {
        std::cout << item << " ";
    }
    std::cout << std::endl;

    
    mLib::Avl<myObj> b2;

    b2.add(o8);
    b2.add(o9);
    b2.add(o10);

    b1 = static_cast<mLib::Avl<myObj>&& > (b2);

    b1.printInOrder();
    std::cout << std::endl;
    b1.printPreOrder();
    std::cout << std::endl;
    b1.printPostOrder();
    std::cout << std::endl;
    b1.printLevelOrder();
    std::cout << std::endl;

    std::cout << "------------" << std::endl << std::endl;

    b2.printInOrder();
    std::cout << std::endl;
    b2.printPreOrder();
    std::cout << std::endl;
    b2.printPostOrder();
    std::cout << std::endl;

    std::cout << b1.getHeight() << std::endl;

    mLib::Avl<myObj> b3 = { o2, o1, o4, o9, o5, o10, o6, o3, o7, o8, };
    for (const auto& item : b3) {
        std::cout << item << " ";
    }
    std::cout << b3.getHeight() << std::endl;
    std::cout << std::endl;

}
int main() {
    tryAvl();
    return 0;
}
