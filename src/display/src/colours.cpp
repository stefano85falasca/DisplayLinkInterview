#include "display/colours.h"
#include "common/assert.h"
#include <limits>

namespace DisplayNS {
Red::Red(unsigned int v) : value{static_cast<decltype(value)>(v)} {}
Red::~Red() { assert_opt(value < 32u); }

Green::Green(unsigned int v) : value{static_cast<decltype(value)>(v)} {}
Green::~Green() { assert_opt(value < 64u); }

Blue::Blue(unsigned int v) : value{static_cast<decltype(value)>(v)} {}
Blue::~Blue() { assert_opt(value < 32u); }

RGB565::RGB565(unsigned int v) : value(static_cast<uint16_t>(v)) {
  assert_opt(v <= std::numeric_limits<uint16_t>::max());
}
RGB565::RGB565(Red red, Green green, Blue blue)
    : value(red.value << 11 | green.value << 5 | blue.value) {}

Red RGB565::red() const { return Red{static_cast<uint_fast8_t>(value >> 11)}; }
Green RGB565::green() const {
  return Green{static_cast<uint_fast8_t>((~(0xf800 | 0x001f) & value) >> 5)};
}
Blue RGB565::blue() const {
  return Blue{static_cast<uint_fast8_t>(~(0xf800 | 0x07e0) & value)};
}
std::ostream &operator<<(std::ostream &out, RGB565 colour) {
  auto r = ((colour.red().value * 255) / 31);
  auto g = ((colour.green().value * 255) / 63);
  auto b = ((colour.blue().value * 255) / 31);
  out << "\033[38;2;" << r << ';' << g << ';' << b << "mx";
  return out;
}
}
