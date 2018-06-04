#include "tracer.hpp"
#include <iostream>
#include <vector>

// NOTE: Assume each of the types below is included from separate header file.

struct A
{
  A() = default;
  A(int, int) {}
};
#define A Tracer<A>

struct B
{
  B() { std::cout << "    B default-constructed" << std::endl; }
  ~B() { std::cout << "    B destructed" << std::endl; }

  B(B &&) { std::cout << "    B move-constructed" << std::endl; }
  B(const B &) { std::cout << "    B copy-constructed" << std::endl; }
  B & operator = (B &&) { std::cout << "    B move-assigned" << std::endl; return *this; }
  B & operator = (const B &) { std::cout << "    B copy-assigned" << std::endl; return *this; }

  B(A &&) { std::cout << "    B move-constructed from A" << std::endl; }
  B(const A &) { std::cout << "    B copy-constructed from A" << std::endl; }
  B & operator = (A &&) { std::cout << "    B move-assigned from A" << std::endl; return *this; }
  B & operator = (const A &) { std::cout << "    B copy-assigned from A" << std::endl; return *this; }
};
#define B Tracer<B>

struct V
{
  V() = default;
  V(std::vector<int> &&) {}
  V(const std::vector<int> &) {}

  V & operator = (std::vector<int> &&) { return *this; }
  V & operator = (const std::vector<int> &) { return *this; }
};
#define V Tracer<V>

struct C {};


int main()
{
  {
    std::cout << "--- Solo demo for Tracer-wrapped type ---" << std::endl;
    A a1;
    A a2 = a1;
    a2 = a1;
    A a3 = std::move(a1);
    a3 = std::move(a2);
    A a4(1, 1);
  }

  {
    std::cout << "--- Solo demo for Tracer-wrapped type with support for different Tracer-wrapped type ---" << std::endl;
    B b1;
    B b2 = b1;
    b2 = b1;
    B b3 = std::move(b1);
    b3 = std::move(b2);
  }

  {
    std::cout << "--- Demo for Tracer-wrapped type compatible with different Tracer-wrapped type ---" << std::endl;
    A a;
    B b1 = a;
    b1 = a;
    B b2 = std::move(a);
    b2 = std::move(a);
  }

  {
    std::cout << "--- Demo for Tracer-wrapped type compatible with custom non-Tracer-wrapped type ---" << std::endl;
    std::vector<int> a;
    V v1 = a;
    v1 = a;
    V v2 = std::move(a);
    v2 = std::move(a);
  }
}
