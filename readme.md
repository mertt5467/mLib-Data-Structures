🌐 [English](#english) | [Türkçe](#turkish)

---
<h1 id="english">🛠️ mLib (Handcrafted Data Structures & Algorithms Library)</h1>

`mLib` is a comprehensive data structures and algorithms library built entirely from scratch using **low-level memory management (raw memory management)** and modern C++ standards (Rule of 5, Move Semantics, Exception Safety). It is designed to keep dependencies on the C++ standard library (`std::`) to an absolute minimum.

---

## 🗺️ mLib Roadmap & Development Status

*   ✅ **Completed** 
*   🚧 **In Progress** (Code written, undergoing optimization/refactoring)
*   ❌ **Not Started**

### 📦 Data Structures

| Structure / Module | Status | Description |
| :--- | :---: | :--- |
| `mLib::Array` (Static Array) | ✅ | Completed. |
| `mLib::List` (Dynamic Array) | ✅ | Completed. |
| `mLib::Sll` (Singly Linked List) | ✅ | Completed. |
| `mLib::Dll` (Doubly Linked List) | ✅ | Completed. |
| `mLib::Cll` (Circular Linked List) | ✅ | Completed. |
| `mLib::LinearHash` (Linear Probing) | ✅ | Completed. |
| `mLib::ChainingHash` (Chaining) | ❌ |  |
| `mLib::Stack` | ✅ | Completed. |
| `mLib::Queue` | ✅ | Completed. |
| `mLib::PriorityQueue` | ✅ | Completed. |
| `mLib::Heap` | ✅ | Completed. |
| `mLib::BST` (Binary Search Tree) | ✅ | Completed. |
| `mLib::AVL` (Self-Balancing Tree) | ✅ | Completed. |
| `mLib::MatrisGraph` (Adjacency Matrix) | ❌ | |
| `mLib::ListGraph` (Adjacency List / Chaining) | ❌ | |

### 🧮 Algorithms

| Algorithm Group | Status | Planned Content / Methods |
| :--- | :---: | :--- |
| mLib::(...)Sort | 🚧 | Bubble Sort and Merge Sort algorithms complete. Other sorting algorithms will be added later. |
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

*   ✅ **Tamamlandı** 
*   🚧 **Yapım Aşamasında** (Kod yazıldı/yazılıyor, optimizasyon/refactor yapılıyor)
*   ❌ **Başlanmadı**

### 📦 Veri Yapıları 

| Yapı / Modül | Durum | Açıklama |
| :--- | :---: | :--- |
| `mLib::Array` (Statik Dizi) | ✅ | Tamamlandı. |
| `mLib::List` (Dinamik Dizi) | ✅ | Tamamlandı. |
| `mLib::Sll` (Singly Linked List) | ✅ | Tamamlandı. |
| `mLib::Dll` (Doubly Linked List) | ✅ | Tamamlandı. |
| `mLib::Cll` (Circular Linked List) | ✅ | Tamamlandı. |
| `mLib::LinearHash` (Linear Probing) | ✅ | Tamamlandı. |
| `mLib::ChainingHash` (Chaining) | ❌ | |
| `mLib::Stack` | ✅ | Tamamlandı. |
| `mLib::Queue` | ✅ | Tamamlandı. |
| `mLib::PriorityQueue` | ✅ | Tamamlandı. |
| `mLib::Heap` | ✅ | Tamamlandı. |
| `mLib::BST` (Binary Search Tree) | ✅ | Tamamlandı. |
| `mLib::AVL` (Self-Balancing Tree) | ✅ | Tamamlandı. |
| `mLib::MatrisGraph` (Adjacency Matrix) | ❌ | |
| `mLib::ListGraph` (Adjacency List / Chaining) | ❌ | |

### 🧮 Algoritmalar 

| Algoritma Grubu | Durum | Planlanan İçerik / Metotlar |
| :--- | :---: | :--- |
| mLib::(...)Sort | 🚧 | Bubble Sort ve Merge Sort algoritmaları tamamlandı. Diğer sıralama algoritmaları daha sonra eklenecektir. |
| **Arama (Search)** | ❌ | Binary Search, Linear Search |

---

## 🔍 Debug Modu

`DEBUG` makrosunu aktif etmek için:

```cpp
#define DEBUG 
#include "mLib.hpp"
```
