#pragma once

#include <iostream>
#include <stdint.h>

namespace DisplayNS {
struct Red {
  explicit Red(unsigned int = 0xf800u);
  uint_fast8_t value;
  ~Red();
};
struct Green {
  explicit Green(unsigned int = 0x07e0u);
  uint_fast8_t value;
  ~Green();
};
struct Blue {
  explicit Blue(unsigned int = 0x001fu);
  uint_fast8_t value;
  ~Blue();
};
struct RGB565 {
  RGB565() = delete;
  explicit RGB565(unsigned int);
  RGB565(Red, Green, Blue);
  Red red() const;
  Green green() const;
  Blue blue() const;
  uint16_t value;
};

std::ostream &operator<<(std::ostream &, RGB565);
}
