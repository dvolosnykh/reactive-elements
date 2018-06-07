#pragma once

#include "typename.hpp"
#include <functional>
#include <iostream>


template<typename... Args>
class FunctionArguments
{
  using Arguments = std::tuple<std::reference_wrapper<const Args>...>;

  template<std::size_t I>
  using Argument = typename std::tuple_element<I, Arguments>::type::type;

public:
  explicit FunctionArguments(const Args &... args)
    : args(std::cref(args)...)
  {}

  void operator()(std::ostream & out) const { print<>(out); }

private:
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
    out << typeName<Argument<I>>() << " = " << std::get<I>(args).get() << ", ";
    print<I + 1>(out);
  }

  template<std::size_t I = 0>
  typename std::enable_if<not isEmpty<I>() and isLastArgument<I>()>::type
  print(std::ostream & out) const
  {
    out << typeName<Argument<I>>() << " = " << std::get<I>(args).get();
  }

  template<std::size_t I = 0>
  typename std::enable_if<isEmpty<I>()>::type
  print(std::ostream &) const
  {}

  friend
  std::ostream & operator<<(std::ostream & out, const FunctionArguments & printer)
  {
    printer(out);
    return out;
  }

private:
  Arguments args;
};

template<typename... Args>
FunctionArguments<Args...> arguments(const Args &... args)
{
  return FunctionArguments<Args...>(args...);
}
