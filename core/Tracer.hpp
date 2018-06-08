#pragma once

#include "FunctionArguments.hpp"


struct TracerBase {};

template<typename T>
class Tracer : public T, TracerBase
{
public:
  Tracer()
    : T()
  {
    std::cout << "[" << *this
      << " default-constructed]" << std::endl;
  }

  template<typename... Args>
  Tracer(Args... args)
    : T(args...)
  {
    std::cout << "[" << *this
      << " constructed with ("
      << arguments(args...)
      << ")]" << std::endl;
  }

  ~Tracer()
  {
    std::cout << "[" << *this << " destructed]" << std::endl;
  }


  // Support the same type wrapped by Tracer.

  Tracer(Tracer && other)
    : T(std::move(other))
  {
    std::cout << "[" << *this
      << " move-constructed from traced "
      << other << "]" << std::endl;
  }

  Tracer(Tracer const & other)
    : T(other)
  {
    std::cout << "[" << *this
      << " copy-constructed from traced "
      << other << "]" << std::endl;
  }

  Tracer & operator=(Tracer && other)
  {
    T::operator=(std::move(other));
    std::cout << "[" << *this
      << " move-assigned from traced "
      << other << "]" << std::endl;
    return *this;
  }

  Tracer & operator=(Tracer const & other)
  {
    T::operator=(other);
    std::cout << "[" << *this
      << " copy-assigned from traced "
      << other << "]" << std::endl;
    return *this;
  }


  // Support for different types wrapped by Tracer.

  template<typename U>
  Tracer(Tracer<U> && other)
    : T(std::move(other))
  {
    std::cout << "[" << *this
      << " move-constructed from traced "
      << other << "]" << std::endl;
  }

  template<typename U>
  Tracer(Tracer<U> const & other)
    : T(other)
  {
    std::cout << "[" << *this
      << " copy-constructed from traced "
      << other << "]" << std::endl;
  }

  template<typename U>
  Tracer & operator=(Tracer<U> && other)
  {
    T::operator=(std::move(other));
    std::cout << "[" << *this
      << " move-assigned from traced "
      << other << "]" << std::endl;
    return *this;
  }

  template<typename U>
  Tracer & operator=(Tracer<U> const & other)
  {
    T::operator=(other);
    std::cout << "[" << *this
      << " copy-assigned from traced "
      << other << "]" << std::endl;
    return *this;
  }


  // Support for custom types.

  template<typename U,
    typename = typename std::enable_if<
      not std::is_base_of<TracerBase, U>::value and
      not std::is_lvalue_reference<U>::value
    >::type>
  Tracer(U && other)
    : T(std::move(other))
  {
    std::cout << "[" << *this
      << " move-constructed from non-traced "
      << typeName<U>() << "]" << std::endl;
  }

  template<typename U,
    typename = typename std::enable_if<
      not std::is_base_of<TracerBase, U>::value
    >::type>
  Tracer(U const & other)
    : T(other)
  {
    std::cout << "[" << *this
      << " copy-constructed from non-traced "
      << typeName<U>() << "]" << std::endl;
  }

  template<typename U,
    typename = typename std::enable_if<
      not std::is_base_of<TracerBase, U>::value and
      not std::is_lvalue_reference<U>::value
    >::type>
  Tracer & operator=(U && other)
  {
    T::operator=(std::move(other));
    std::cout << "[" << *this
      << " move-assigned from non-traced "
      << typeName<U>() << "]" << std::endl;
    return *this;
  }

  template<typename U,
    typename = typename std::enable_if<
      not std::is_base_of<TracerBase, U>::value
    >::type>
  Tracer & operator=(U const & other)
  {
    T::operator=(other);
    std::cout << "[" << *this
      << " copy-assigned from non-traced "
      << typeName<U>() << "]" << std::endl;
    return *this;
  }

private:
  friend
  std::ostream & operator<<(std::ostream & out, Tracer const & tracer)
  {
    return out << tracer.name << "." << tracer.id;
  }

private:
  static std::string const name;
  static std::size_t counter;
  decltype(counter) const id{++counter};
};

template<typename T>
std::string const Tracer<T>::name = typeName<T>();

template<typename T>
std::size_t Tracer<T>::counter = 0;
