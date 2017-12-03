#include "common/assert.h"
#include "testscreens/testscreens.h"
#include <iostream>
#include <limits>

namespace {
int drawline(int x0, int y0, const int x1, const int y1, const int current) {
  assert_opt(x0 <= current);
  assert_opt(current <= x1);

  auto dx = abs(x1 - x0);
  auto sx = x0 < x1 ? 1 : -1;
  auto dy = abs(y1 - y0);
  auto sy = y0 < y1 ? 1 : -1;
  int err = (dx > dy ? dx : -dy) / 2, e2;

  for (;;) {
    if (x0 == x1 && y0 == y1)
      return y1;
    e2 = err;
    if (e2 > -dx) {
      if (current == x0) {
        return y0;
      }
      err -= dy;
      x0 += sx;
    }
    if (e2 < dy) {
      err += dx;
      y0 += sy;
    }
  }
  return y1;
}

template <typename C> C colour(C start, C end, int total, int coordinate) {
  assert_opt(coordinate < total);
  assert_opt(total > 0);
  assert_opt(coordinate > 0);

  return C(drawline(0, start.value, total, end.value, coordinate));
}
}

namespace TestScreens {
DisplayNS::RGB565 colour(DisplayNS::RGB565 start, DisplayNS::RGB565 end,
                         int total, int coordinate) {
  using namespace DisplayNS;
  auto red = ::colour(start.red(), end.red(), total, coordinate);
  auto green = ::colour(start.green(), end.green(), total, coordinate);
  auto blue = ::colour(start.blue(), end.blue(), total, coordinate);
  return {red, green, blue};
}
}
