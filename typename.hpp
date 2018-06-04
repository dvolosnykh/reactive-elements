#pragma once

#include <typeinfo>
#include <boost/core/demangle.hpp>


template<typename T>
inline
std::string typeName()
{
  return boost::core::demangle(typeid(T).name());
}
