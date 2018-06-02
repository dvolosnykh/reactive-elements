#pragma once

#include <iostream>
#include <typeinfo>
#include <boost/core/demangle.hpp>


template<typename T>
struct Tracer
{
  Tracer()
  {
    std::cout << '[' << *this << " constructed]" << std::endl;
  }

  ~Tracer()
  {
    std::cout << '[' << *this << " destructed]" << std::endl;
  }


  template<typename U>
  Tracer(Tracer<U> && other)
  {
    std::cout << '[' << *this << " move-constructed from " << other << ']' << std::endl;
  }

  Tracer(const Tracer & other)
  {
    std::cout << '[' << *this << " copy-constructed from " << other << ']' << std::endl;
  }

  template<typename U, typename = typename std::enable_if<not std::is_same<T, U>::value, void>::type>
  Tracer(const Tracer<U> & other)
  {
    std::cout << '[' << *this << " copy-constructed from " << other << ']' << std::endl;
  }


  template<typename U>
  Tracer & operator = (Tracer<U> && other)
  {
    std::cout << '[' << *this << " move-assigned from " << other << ']' << std::endl;
    return *this;
  }

  Tracer & operator = (const Tracer & other)
  {
    std::cout << '[' << *this << " copy-assigned from " << other << ']' << std::endl;
    return *this;
  }

  template<typename U, typename = typename std::enable_if<not std::is_same<T, U>::value, void>::type>
  Tracer & operator = (const Tracer<U> & other)
  {
    std::cout << '[' << *this << " copy-assigned from " << other << ']' << std::endl;
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
const std::string Tracer<T>::name = boost::core::demangle(typeid(T).name());

template<typename T>
std::size_t Tracer<T>::counter = 0;
