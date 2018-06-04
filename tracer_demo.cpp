#include "tracer.hpp"
#include <iostream>
#include <vector>

// NOTE: Assume each of the types below is included from separate header file.

struct A
{
  A() = default;
  A(int i, float f, std::string s)
  {
    std::cout << "  " << ArgumentsPrinter<decltype(i), decltype(f), decltype(s)>(i, f, s) << std::endl;
  }
};
#define A Tracer<A>

struct B
{
  B()
  {
    std::cout << "  implementation of B default-constructor" << std::endl;
  }
  ~B()
  {
    std::cout << "  implementation of B destructor" << std::endl;
  }

  B(B &&)
  {
    std::cout << "  implementation of B move-constructor" << std::endl;
  }
  B(const B &)
  {
    std::cout << "  implementation of B copy-constructor" << std::endl;
  }
  B & operator = (B &&)
  {
    std::cout << "  implementation of B move-assignment" << std::endl;
    return *this;
  }
  B & operator = (const B &)
  {
    std::cout << "  implementation of B copy-assignment" << std::endl;
    return *this;
  }

  B(A &&)
  {
    std::cout << "  implementation of B move-constructor from A" << std::endl;
  }
  B(const A &)
  {
    std::cout << "  implementation of B copy-constructor from A" << std::endl;
  }
  B & operator = (A &&)
  {
    std::cout << "  implementation of B move-assignment from A" << std::endl;
    return *this;
  }
  B & operator = (const A &)
  {
    std::cout << "  implementation of B copy-assignment from A" << std::endl;
    return *this;
  }
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


int main()
{
  {
    std::cout << "\n--- Solo demo for traced type ---\n" << std::endl;
    A a1;
    A a2 = a1;
    a2 = a1;
    A a3 = std::move(a1);
    a3 = std::move(a2);
    A a4(1, 2.0, "text");
  }

  {
    std::cout << "\n--- Solo demo for traced type with support for different traced type ---\n" << std::endl;
    B b1;
    B b2 = b1;
    b2 = b1;
    B b3 = std::move(b1);
    b3 = std::move(b2);
  }

  {
    std::cout << "\n--- Demo for traced type compatible with different traced type ---\n" << std::endl;
    A a;
    B b1 = a;
    b1 = a;
    B b2 = std::move(a);
    b2 = std::move(a);
  }

  {
    std::cout << "\n--- Demo for traced type compatible with custom non-traced type ---\n" << std::endl;
    std::vector<int> a;
    V v1 = a;
    v1 = a;
    V v2 = std::move(a);
    v2 = std::move(a);
  }
}
