#pragma once

#include <functional>
#include <vector>
#include <c10/util/complex.h>
namespace c10 {

// NOTE: hash_combine is based on implementation from Boost
//
// Boost Software License - Version 1.0 - August 17th, 2003
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
//
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.

inline size_t hash_combine(size_t seed, size_t value) {
  return seed ^ (value + 0x9e3779b9 + (seed << 6u) + (seed >> 2u));
}

////////////////////////////////////////////////////////////////////////////////
// c10::hash implementation
////////////////////////////////////////////////////////////////////////////////

namespace _hash_detail {

// Use template argument deduction to shorten calls to c10::hash
template <typename T>
size_t simple_get_hash(const T& o);

template <typename T, typename V>
using type_if_not_enum =
    typename std::enable_if<!std::is_enum<T>::value, V>::type;

// Use SFINAE to dispatch to std::hash if possible, cast enum types to int
// automatically, and fall back to T::hash otherwise. NOTE: C++14 added support
// for hashing enum types to the standard, and some compilers implement it even
// when C++14 flags aren't specified. This is why we have to disable this
// overload if T is an enum type (and use the one below in this case).
template <typename T>
auto dispatch_hash(const T& o)
    -> decltype(std::hash<T>()(o), type_if_not_enum<T, size_t>()) {
  return std::hash<T>()(o);
}

template <typename T>
typename std::enable_if<std::is_enum<T>::value, size_t>::type dispatch_hash(
    const T& o) {
  using R = typename std::underlying_type<T>::type;
  return std::hash<R>()(static_cast<R>(o));
}

template <typename T>
auto dispatch_hash(const T& o) -> decltype(T::hash(o), size_t()) {
  return T::hash(o);
}

} // namespace _hash_detail

// Hasher struct
template <typename T>
struct hash {
  size_t operator()(const T& o) const {
    return _hash_detail::dispatch_hash(o);
  };
};

// Specialization for std::tuple
template <typename... Types>
struct hash<std::tuple<Types...>> {
  template <size_t idx, typename... Ts>
  struct tuple_hash {
    size_t operator()(const std::tuple<Ts...>& t) const {
      return hash_combine(
          _hash_detail::simple_get_hash(std::get<idx>(t)),
          tuple_hash<idx - 1, Ts...>()(t));
    }
  };

  template <typename... Ts>
  struct tuple_hash<0, Ts...> {
    size_t operator()(const std::tuple<Ts...>& t) const {
      return _hash_detail::simple_get_hash(std::get<0>(t));
    }
  };

  size_t operator()(const std::tuple<Types...>& t) const {
    return tuple_hash<sizeof...(Types) - 1, Types...>()(t);
  }
};

// Specialization for std::vector
template <typename T>
struct hash<std::vector<T>> {
  size_t operator()(const std::vector<T>& v) const {
    size_t seed = 0;
    for (const auto& elem : v) {
      seed = hash_combine(seed, _hash_detail::simple_get_hash(elem));
    }
    return seed;
  }
};

namespace _hash_detail {

template <typename T>
size_t simple_get_hash(const T& o) {
  return c10::hash<T>()(o);
}

} // namespace _hash_detail

// Use this function to actually hash multiple things in one line.
// Dispatches to c10::hash, so it can hash containers.
// Example:
//
// static size_t hash(const MyStruct& s) {
//   return get_hash(s.member1, s.member2, s.member3);
// }
template <typename... Types>
size_t get_hash(const Types&... args) {
  return c10::hash<decltype(std::tie(args...))>()(std::tie(args...));
}

// Specialization for c10::complex
template <typename T>
struct hash<c10::complex<T>> {
  size_t operator()(const c10::complex<T>& c) const {
    return get_hash(c.real(), c.imag());
  }
};


} // namespace c10
