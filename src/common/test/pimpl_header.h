#pragma once

#include "common/pimplh.h"

struct PimpldClass {
  PimpldClass(bool, int, char);
  PimpldClass();
  bool working() const;
  ~PimpldClass();

private:
  struct Impl;
  pimpl<Impl> impl;
};
