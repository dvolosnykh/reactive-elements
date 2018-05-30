#include "tracer.hpp"
#include <iostream>

struct A
{
  Tracer<A> tracer;
};

struct B
{
  Tracer<B> tracer;

  B() = default;
  B(A && a) : tracer(std::move(a.tracer)) {}
  B(const A & a) : tracer(a.tracer) {}


  B & operator = (A && a)
  {
    tracer = std::move(a.tracer);
    return *this;
  }

  B & operator = (const A & a)
  {
    tracer = a.tracer;
    return *this;
  }
};

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
