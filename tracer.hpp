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
struct Tracer
{
  Tracer()
    : object()
  {
    std::cout << '[' << *this << " default-constructed]" << std::endl;
  }

  template<typename ...Args>
  Tracer(Args ...args)
    : object(args...)
  {
    std::cout << '[' << *this << " constructed with (";
    printArgs(args...);
    std::cout << ")]" << std::endl;
  }

  ~Tracer()
  {
    std::cout << '[' << *this << " destructed]" << std::endl;
  }


  // Support the same type wrapped by Tracer.

  Tracer(Tracer && other)
    : object(std::move(other.object))
  {
    std::cout << '[' << *this << " move-constructed from same-typed " << other << ']' << std::endl;
  }

  Tracer(const Tracer & other)
    : object(other.object)
  {
    std::cout << '[' << *this << " copy-constructed from same-typed " << other << ']' << std::endl;
  }

  Tracer & operator = (Tracer && other)
  {
    object = std::move(other.object);
    std::cout << '[' << *this << " move-assigned from same-typed " << other << ']' << std::endl;
    return *this;
  }

  Tracer & operator = (const Tracer & other)
  {
    object = other.object;
    std::cout << '[' << *this << " copy-assigned from same-typed " << other << ']' << std::endl;
    return *this;
  }


  // Support for different types wrapped by Tracer.

  template<typename U>
  Tracer(Tracer<U> && other)
    : object(std::move(other))
  {
    std::cout << '[' << *this << " move-constructed from different-typed " << other << ']' << std::endl;
  }

  template<typename U>
  Tracer(const Tracer<U> & other)
    : object(other)
  {
    std::cout << '[' << *this << " copy-constructed from different-typed " << other << ']' << std::endl;
  }

  template<typename U>
  Tracer & operator = (Tracer<U> && other)
  {
    object = std::move(other);
    std::cout << '[' << *this << " move-assigned from different-typed " << other << ']' << std::endl;
    return *this;
  }

  template<typename U>
  Tracer & operator = (const Tracer<U> & other)
  {
    object = other;
    std::cout << '[' << *this << " copy-assigned from different-typed " << other << ']' << std::endl;
    return *this;
  }


  // Support for custom types.

  template<typename U, typename = typename std::enable_if<not std::is_lvalue_reference<U>::value>::type>
  Tracer(U && other)
    : object(std::move(other))
  {
    std::cout << '[' << *this << " move-constructed from non-traced " << typeName<U>() << ']' << std::endl;
  }

  template<typename U>
  Tracer(const U & other)
    : object(other)
  {
    std::cout << '[' << *this << " copy-constructed from non-traced " << typeName<U>() << ']' << std::endl;
  }

  template<typename U, typename = typename std::enable_if<not std::is_lvalue_reference<U>::value>::type>
  Tracer & operator = (U && other)
  {
    object = std::move(other);
    std::cout << '[' << *this << " move-assigned from non-traced " << typeName<U>() << ']' << std::endl;
    return *this;
  }

  template<typename U>
  Tracer & operator = (const U & other)
  {
    object = other;
    std::cout << '[' << *this << " copy-assigned from non-traced " << typeName<U>() << ']' << std::endl;
    return *this;
  }

private:
  template<typename Arg, typename ...Args>
  static
  void printArgs(Arg arg, Args ...args)
  {
    printArgs(arg);
    std::cout << ", ";
    printArgs(args...);
  }

  template<typename Arg>
  static
  void printArgs(Arg arg)
  {
    std::cout << typeName<Arg>() << '=' << arg;
  }

  friend std::ostream & operator << (std::ostream & out, const Tracer & tracer)
  {
    return out << tracer.name << '.' << tracer.id;
  }

private:
  template<typename U>
  friend struct Tracer;
  T object;

  static const std::string name;
  static std::size_t counter;
  const decltype(counter) id{++counter};
};

template<typename T>
const std::string Tracer<T>::name = typeName<T>();

template<typename T>
std::size_t Tracer<T>::counter = 0;
