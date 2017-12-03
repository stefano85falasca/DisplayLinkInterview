#pragma once

#include <boost/operators.hpp>
#include <memory>

template <typename T>
struct pimpl : boost::equality_comparable<pimpl<T>, std::nullptr_t> {
  pimpl(pimpl &&source);
  pimpl &operator=(pimpl &&source);
  template <typename... Args> explicit pimpl(Args &&... args);
  ~pimpl();
  T *operator->();
  T &operator*();
  const T *operator->() const;
  const T &operator*() const;

  friend bool operator==(const pimpl<T> &lhs, std::nullptr_t) {
    return lhs.m == nullptr;
  }

private:
  std::unique_ptr<T> m;
};
