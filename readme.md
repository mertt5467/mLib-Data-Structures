🌐 [English](#english) | [Türkçe](#turkish)

---
<h1 id="english">🛠️ mLib (Handcrafted Data Structures & Algorithms Library)</h1>

`mLib` is a comprehensive data structures and algorithms library built entirely from scratch using **low-level memory management (raw memory management)** and modern C++ standards (Rule of 5, Move Semantics, Exception Safety). It is designed to keep dependencies on the C++ standard library (`std::`) to an absolute minimum.

---

## 🗺️ mLib Roadmap & Development Status

*   ✅ **Completed** (Industrial Standard / Fully Optimized)
*   🚧 **In Progress** (Code written, undergoing optimization/refactoring)
*   ❌ **Not Started**

### 📦 Data Structures

| Structure / Module | Status | Description |
| :--- | :---: | :--- |
| `mLib::Arrays::Array` (Static Array) | ✅ | Completed. |
| `mLib::Arrays::List` (Dynamic Array) | 🚧 | Missing only sort method |
| `mLib::LinkedList::Sll` (Singly Linked List) | 🚧 | In Progress |
| `mLib::LinkedList::Dll` (Doubly Linked List) | 🚧 | Core logic ready, undergoing refactoring |
| `mLib::LinkedList::Cll` (Circular Linked List) | ❌ | |
| `mLib::Hash::LineerHash` (Linear Probing) | 🚧 | Core logic ready, undergoing refactoring |
| `mLib::Hash::ChainingHash` (Chaining) | ❌ |  |
| `mLib::Containers::Stack` | ❌ | |
| `mLib::Containers::Queue` | ❌ | |
| `mLib::Containers::PriorityQueue` | ❌ | |
| `mLib::Trees::BST` (Binary Search Tree) | ❌ | |
| `mLib::Trees::AVL` (Self-Balancing Tree) | ❌ | |
| `mLib::Graphs::Graph` (Adjacency Matrix) | ❌ | |
| `mLib::Graphs::Graph` (Adjacency List / Chaining) | ❌ | |

### 🧮 Algorithms

| Algorithm Group | Status | Planned Content / Methods |
| :--- | :---: | :--- |
| **Sorting (Sort)** | ❌ | Merge Sort, Quick Sort, Bubble/Insertion/Selection Sort |
| **Searching (Search)** | ❌ | Binary Search, Linear Search |

---

## 🔍 Debug Mode

To activate the `DEBUG` macro:

```cpp
#define DEBUG 
#include "mLib.hpp"
```
---

<h1 id="turkish">🛠️ mLib (El Yapımı Veri Yapıları ve Algoritmalar Kütüphanesi)</h1>

`mLib`, C++ standart kütüphanesine (`std::`) bağımlılığı minimumda tutarak, tamamen **low-level bellek yönetimi (raw memory management)** ve modern C++ standartları (Rule of 5, Move Semantics, Exception Safety) kullanılarak sıfırdan geliştirilmiş kapsamlı bir veri yapıları ve algoritmalar kütüphanesidir.

---

## 🗺️ mLib Yol Haritası & Gelişim Durumu

*   ✅ **Tamamlandı** (Endüstriyel Standart / Optimize)
*   🚧 **Yapım Aşamasında** (Kod yazıldı/yazılıyor, optimizasyon/refactor yapılıyor)
*   ❌ **Başlanmadı**

### 📦 Veri Yapıları 

| Yapı / Modül | Durum | Açıklama |
| :--- | :---: | :--- |
| `mLib::Arrays::Array` (Statik Dizi) | ✅ | Tamamlandı. |
| `mLib::Arrays::List` (Dinamik Dizi) | 🚧 | Sadece sıralama algoritması eksik. |
| `mLib::LinkedList::Sll` (Singly Linked List) | 🚧 | Yapım Aşamasında |
| `mLib::LinkedList::Dll` (Doubly Linked List) | 🚧 | Temel mantık hazır, refactor edilecek |
| `mLib::LinkedList::Cll` (Circular Linked List) | ❌ | |
| `mLib::Hash::LineerHash` (Linear Probing) | 🚧 | Temel mantık hazır, refactor edilecek |
| `mLib::Hash::ChainingHash` (Chaining) | ❌ | |
| `mLib::Containers::Stack` | ❌ | |
| `mLib::Containers::Queue` | ❌ | |
| `mLib::Containers::PriorityQueue` | ❌ | |
| `mLib::Trees::BST` (Binary Search Tree) | ❌ | |
| `mLib::Trees::AVL` (Self-Balancing Tree) | ❌ | |
| `mLib::Graphs::Graph` (Adjacency Matrix) | ❌ | |
| `mLib::Graphs::Graph` (Adjacency List / Chaining) | ❌ | |

### 🧮 Algoritmalar 

| Algoritma Grubu | Durum | Planlanan İçerik / Metotlar |
| :--- | :---: | :--- |
| **Sıralama (Sort)** | ❌ | Merge Sort, Quick Sort, Bubble/Insertion/Selection |
| **Arama (Search)** | ❌ | Binary Search, Linear Search |

---

## 🔍 Debug Modu

`DEBUG` makrosunu aktif etmek için:

```cpp
#define DEBUG 
#include "mLib.hpp"
```