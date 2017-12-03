#include "common/pimplcpp.h"

#include "pimpl_header.h"

struct PimpldClass::Impl {
  Impl() = default;
  Impl(bool b, int /*unused*/, char /*unused*/) : b(b) {}
  bool working() const { return b; }

private:
  bool b = true;
};

PimpldClass::PimpldClass() = default;
PimpldClass::PimpldClass(bool b, int i, char c) : impl(b, i, c) {}

PimpldClass::~PimpldClass() = default;

bool PimpldClass::working() const { return impl->working(); }
