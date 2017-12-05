#pragma once

#include <stdint.h>

namespace TestScreens {
struct Red {
  explicit Red(unsigned int = 31u);
  uint_fast8_t value;
  ~Red();
};
struct Green {
  explicit Green(unsigned int = 63u);
  uint_fast8_t value;
  ~Green();
};
struct Blue {
  explicit Blue(unsigned int = 31u);
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
}
