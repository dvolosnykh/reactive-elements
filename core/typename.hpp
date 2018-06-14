#pragma once

#include <typeinfo>
#ifdef USE_BOOST
#include <boost/core/demangle.hpp>
#endif


template<typename T>
inline
char const * nativeTypeName() { return typeid(T).name(); }

template<typename T>
inline
#ifdef USE_BOOST
std::string typeName() { return boost::core::demangle(nativeTypeName<T>()); }
#else
char const * typeName() { return nativeTypeName<T>(); }
#endif
