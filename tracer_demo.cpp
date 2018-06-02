#include "tracer.hpp"
#include <iostream>

struct A
{
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

int main()
{
  {
    std::cout << "--- demo for type w/o support of another one ---" << std::endl;
    A a1;
    A a2 = a1;
    a2 = a1;
    A a3 = std::move(a1);
    a3 = std::move(a2);
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
}
