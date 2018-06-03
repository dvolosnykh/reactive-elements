#pragma once

#include <iostream>
#include <typeinfo>
#include <boost/core/demangle.hpp>


template<typename T>
static inline
std::string typeName()
{
  return boost::core::demangle(typeid(T).name());
}


template<typename T>
struct Tracer : T
{
  // NOTE: This case of default construction may well be covered by template constructor
  // with varying number of arguments.
  Tracer()
    : T()
  {
    std::cout << '[' << *this << " default-constructed]" << std::endl;
  }

  template<typename ...Args>
  Tracer(Args ...args)
    : T(args...)
  {
    // TODO: print types (and maybe values) of arguments.
    std::cout << '[' << *this << " constructed with args]" << std::endl;
  }

  ~Tracer()
  {
    std::cout << '[' << *this << " destructed]" << std::endl;
  }


  // Support the same type wrapped by Tracer.

  Tracer(Tracer && other)
  {
    std::cout << '[' << *this << " move-constructed from same-typed " << other << ']' << std::endl;
  }

  Tracer(const Tracer & other)
  {
    std::cout << '[' << *this << " copy-constructed from same-typed " << other << ']' << std::endl;
  }

  Tracer & operator = (Tracer && other)
  {
    std::cout << '[' << *this << " move-assigned from same-typed " << other << ']' << std::endl;
    return *this;
  }

  Tracer & operator = (const Tracer & other)
  {
    std::cout << '[' << *this << " copy-assigned from same-typed " << other << ']' << std::endl;
    return *this;
  }


  // Support for different type wrapped by Tracer.

  template<typename U>
  Tracer(Tracer<U> && other)
  {
    std::cout << '[' << *this << " move-constructed from different-typed " << other << ']' << std::endl;
  }

  template<typename U>
  Tracer(const Tracer<U> & other)
  {
    std::cout << '[' << *this << " copy-constructed from different-typed " << other << ']' << std::endl;
  }

  template<typename U>
  Tracer & operator = (Tracer<U> && other)
  {
    std::cout << '[' << *this << " move-assigned from different-typed " << other << ']' << std::endl;
    return *this;
  }

  template<typename U>
  Tracer & operator = (const Tracer<U> & other)
  {
    std::cout << '[' << *this << " copy-assigned from different-typed " << other << ']' << std::endl;
    return *this;
  }


  // Support for custom type.

  template<typename U, typename = typename std::enable_if<not std::is_lvalue_reference<U>::value>::type>
  Tracer(U &&)
  {
    std::cout << '[' << *this << " move-constructed from non-traced " << typeName<U>() << ']' << std::endl;
  }

  template<typename U>
  Tracer(const U &)
  {
    std::cout << '[' << *this << " copy-constructed from non-traced " << typeName<U>() << ']' << std::endl;
  }

  template<typename U, typename = typename std::enable_if<not std::is_lvalue_reference<U>::value>::type>
  Tracer & operator = (U &&)
  {
    std::cout << '[' << *this << " move-assigned from non-traced " << typeName<U>() << ']' << std::endl;
    return *this;
  }

  template<typename U>
  Tracer & operator = (const U &)
  {
    std::cout << '[' << *this << " copy-assigned from non-traced " << typeName<U>() << ']' << std::endl;
    return *this;
  }

private:
  friend std::ostream & operator << (std::ostream & out, const Tracer & tracer)
  {
    return out << tracer.name << '.' << tracer.id;
  }

private:
  static const std::string name;
  static std::size_t counter;
  const decltype(counter) id{++counter};
};

template<typename T>
const std::string Tracer<T>::name = typeName<T>();

template<typename T>
std::size_t Tracer<T>::counter = 0;
