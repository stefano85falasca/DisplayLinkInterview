#include "testscreens/ramp.h"
#include "common/assert.h"
#include <array>
#include <boost/optional.hpp>
#include <iostream>
#include <limits>
#include <math.h>
#include <utility>

namespace {
/*
 * Given a range on the x axis and a range on the y axis that collectively
 * represent a line, gives an approximation of the y value for a given x.
 *
 * Ranges are specified as begin and end (past the last) elements.
 *
 * The function assumes that both ranges are increasing, i.e. xRange.first <
 * xRange.second and yRange.first < yRange.second
 */
int compute(const std::pair<int, int> xRange, const std::pair<int, int> yRange,
            const int x) {
  assert_opt(xRange.first <= x);
  assert_opt(x < xRange.second);
  assert_opt(yRange.first < yRange.second);

  auto y = round(yRange.first +
                 double(x - xRange.first) *
                     double(yRange.second - 1 - yRange.first) /
                     double(xRange.second - 1 - xRange.first));
  assert_opt(yRange.first <= y);
  assert_opt(y < yRange.second);
  return y;
}

template <typename C> C ramp(C first, C last, int total, int coordinate) {
  assert_opt(coordinate >= 0);
  assert_opt(coordinate < total);

  // lastpoints are trivial, return result
  if (0 == coordinate) {
    return first;
  }
  if (total - 1 == coordinate) {
    return last;
  }

  if (first.value > last.value) {
    // invert x axis for negative slope
    auto y = C(compute({0, total}, {last.value, first.value + 1},
                       total - 1 - coordinate));
    assert_normal((first.value <= y.value && y.value <= last.value) ||
                  (last.value <= y.value && y.value <= first.value));
    return y;
  }
  auto y = C(compute({0, total}, {first.value, last.value + 1}, coordinate));
  assert_normal((first.value <= y.value && y.value <= last.value) ||
                (last.value <= y.value && y.value <= first.value));
  return y;
}
} // namespace

namespace TestScreens {
RGB565 ramp(RGB565 first, RGB565 last, int total, int coordinate) {
  auto red = ::ramp(first.red(), last.red(), total, coordinate);
  auto green = ::ramp(first.green(), last.green(), total, coordinate);
  auto blue = ::ramp(first.blue(), last.blue(), total, coordinate);

  // make sure that colours move in the right direction
  assert_safe(
      0 == coordinate ||
      (abs(last.red().value - red.value) <=
       abs(last.red().value -
           ::ramp(first.red(), last.red(), total, coordinate - 1).value)));
  assert_safe(
      total - 1 == coordinate ||
      (abs(last.red().value - red.value) >=
       abs(last.red().value -
           ::ramp(first.red(), last.red(), total, coordinate + 1).value)));
  assert_safe(
      0 == coordinate ||
      (abs(first.red().value - red.value) >=
       abs(first.red().value -
           ::ramp(first.red(), last.red(), total, coordinate - 1).value)));
  assert_safe(
      total - 1 == coordinate ||
      (abs(first.red().value - red.value) <=
       abs(first.red().value -
           ::ramp(first.red(), last.red(), total, coordinate + 1).value)));

  assert_safe(
      0 == coordinate ||
      (abs(last.green().value - green.value) <=
       abs(last.green().value -
           ::ramp(first.green(), last.green(), total, coordinate - 1).value)));
  assert_safe(
      total - 1 == coordinate ||
      (abs(last.green().value - green.value) >=
       abs(last.green().value -
           ::ramp(first.green(), last.green(), total, coordinate + 1).value)));
  assert_safe(
      0 == coordinate ||
      (abs(first.green().value - green.value) >=
       abs(first.green().value -
           ::ramp(first.green(), last.green(), total, coordinate - 1).value)));
  assert_safe(
      total - 1 == coordinate ||
      (abs(first.green().value - green.value) <=
       abs(first.green().value -
           ::ramp(first.green(), last.green(), total, coordinate + 1).value)));

  assert_safe(
      0 == coordinate ||
      (abs(last.blue().value - blue.value) <=
       abs(last.blue().value -
           ::ramp(first.blue(), last.blue(), total, coordinate - 1).value)));
  assert_safe(
      total - 1 == coordinate ||
      (abs(last.blue().value - blue.value) >=
       abs(last.blue().value -
           ::ramp(first.blue(), last.blue(), total, coordinate + 1).value)));
  assert_safe(
      0 == coordinate ||
      (abs(first.blue().value - blue.value) >=
       abs(first.blue().value -
           ::ramp(first.blue(), last.blue(), total, coordinate - 1).value)));
  assert_safe(
      total - 1 == coordinate ||
      (abs(first.blue().value - blue.value) <=
       abs(first.blue().value -
           ::ramp(first.blue(), last.blue(), total, coordinate + 1).value)));

  // make sure that the last-point are satisfied
  assert_safe(coordinate != 0 || first.value == RGB565{red, green, blue}.value);
  assert_safe(coordinate != total - 1 || total == 1 ||
              last.value == RGB565{red, green, blue}.value);

  return {red, green, blue};
}
} // namespace TestScreens
