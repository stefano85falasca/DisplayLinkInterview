#pragma once

#include "common/pimplh.h"
#include <utility>

template <typename T>
pimpl<T>::pimpl(pimpl &&source) : m(std::move(source.m)) {}

template <typename T> pimpl<T> &pimpl<T>::operator=(pimpl &&source) {
  m = std::move(source.m);
  return *this;
}

template <typename T>
template <typename... Args>
pimpl<T>::pimpl(Args &&... args) : m{new T{std::forward<Args>(args)...}} {}

template <typename T> pimpl<T>::~pimpl() = default;

template <typename T> T *pimpl<T>::operator->() { return m.get(); }

template <typename T> T &pimpl<T>::operator*() { return *m; }

template <typename T> const T *pimpl<T>::operator->() const { return m.get(); }

template <typename T> const T &pimpl<T>::operator*() const { return *m.get(); }
