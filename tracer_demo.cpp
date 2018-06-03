#include "tracer.hpp"
#include <iostream>
#include <vector>

struct A
{
  A() = default;
  A(int, int) {}
};

#define A Tracer<A>

struct B
{
  B() = default;
  B(A &&) {}
  B(const A &) {}

  B & operator = (A &&) { return *this; }
  B & operator = (const A &) { return *this; }
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
    std::cout << "--- demo for type w/o support of another one ---" << std::endl;
    A a1;
    A a2 = a1;
    a2 = a1;
    A a3 = std::move(a1);
    a3 = std::move(a2);
    A a4(1, 1);
  }

  {
    std::cout << "--- demo for type with support of another one ---" << std::endl;
    B b1;
    B b2 = b1;
    b2 = b1;
    B b3 = std::move(b1);
    b3 = std::move(b2);
  }

  {
    std::cout << "--- demo for different but compatible types ---" << std::endl;
    A a;
    B b1 = a;
    b1 = a;
    B b2 = std::move(a);
    b2 = std::move(a);
  }

  {
    std::cout << "--- demo for different but compatible types ---" << std::endl;
    std::vector<int> a;
    V v1 = a;
    v1 = a;
    V v2 = std::move(a);
    v2 = std::move(a);
  }
}
