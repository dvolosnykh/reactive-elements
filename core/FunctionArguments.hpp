#pragma once

#include "typename.hpp"
#include <functional>
#include <iostream>


template<typename... Args>
class FunctionArguments
{
  using Arguments = std::tuple<std::reference_wrapper<Args const>...>;

  template<std::size_t I>
  using Argument = typename std::tuple_element<I, Arguments>::type::type;

public:
  explicit FunctionArguments(Args const &... args)
    : args(std::cref(args)...)
  {}

  void operator()(std::ostream & out) const { print<>(out); }

private:
  // TODO: All the print-related template stuff below may be simplified
  // by fold-expressions once C++17 is available.
  template<std::size_t I>
  static constexpr
  bool isEmpty() { return std::tuple_size<Arguments>::value == 0; }

  template<std::size_t I>
  static constexpr
  bool isLastArgument() { return I >= std::tuple_size<Arguments>::value - 1; }

  template<std::size_t I = 0>
  typename std::enable_if<not isEmpty<I>() and not isLastArgument<I>()>::type
  print(std::ostream & out) const
  {
    // TODO: Detect if i-th argument is printable.
    out << typeName<Argument<I>>() << " = " << std::get<I>(args).get() << ", ";
    print<I + 1>(out);
  }

  template<std::size_t I = 0>
  typename std::enable_if<not isEmpty<I>() and isLastArgument<I>()>::type
  print(std::ostream & out) const
  {
    // TODO: Detect if i-th argument is printable.
    out << typeName<Argument<I>>() << " = " << std::get<I>(args).get();
  }

  template<std::size_t I = 0>
  typename std::enable_if<isEmpty<I>()>::type
  print(std::ostream &) const
  {}

  friend
  std::ostream & operator<<(std::ostream & out, FunctionArguments const & printer)
  {
    printer(out);
    return out;
  }

private:
  Arguments args;
};

template<typename... Args>
inline
FunctionArguments<Args...> arguments(Args const &... args)
{
  return FunctionArguments<Args...>(args...);
}
