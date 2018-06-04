#pragma once

#include "typename.hpp"
#include <tuple>


template<typename... Args>
class ArgumentsPrinter
{
  using Arguments = std::tuple<Args...>;

  template<std::size_t I>
  using Argument = typename std::tuple_element<I, Arguments>::type;

public:
  explicit ArgumentsPrinter(Args... args)
    : args(std::move(args)...)
  {}

  void operator()(std::ostream & out) const { print<>(out); }

private:
  template<std::size_t I>
  static constexpr
  bool isLastArgument()
  {
    return I >= std::tuple_size<Arguments>::value - 1;
  }

  template<std::size_t I = 0>
  typename std::enable_if<not isLastArgument<I>()>::type
  print(std::ostream & out) const
  {
    out << typeName<Argument<I>>() << " = " << std::get<I>(args) << ", ";
    print<I + 1>(out);
  }

  template<std::size_t I = 0>
  typename std::enable_if<isLastArgument<I>()>::type
  print(std::ostream & out) const
  {
    out << typeName<Argument<I>>() << " = " << std::get<I>(args);
  }

  friend
  std::ostream & operator<<(std::ostream & out, const ArgumentsPrinter & printer)
  {
    printer(out);
    return out;
  }

private:
  Arguments args;
};
