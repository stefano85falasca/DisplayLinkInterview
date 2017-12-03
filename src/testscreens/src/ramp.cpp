#include "common/assert.h"
#include "testscreens/testscreens.h"
#include <array>
#include <boost/optional.hpp>
#include <iostream>
#include <limits>
#include <utility>

namespace {
/*
 * divide a segment in 2 or 3 zones depending on whether it's made of an even or
 * odd number of points
 *
 * |head|tail|
 * |****|****|
 * a segment composed of an even number (8) of points gets split in two equal
 * zones named head and tail
 *
 * |head|m|tail|
 * |****|*|****|
 * a segment composed of an odd number of points (9) gets split in
 * * a head
 * * a middle point
 * * a tail having the same size (4) as the head
 */
struct Zones {
  explicit Zones(std::pair<int, int> range) {
    assert_opt_message(range.first < range.second,
                       "first: " << range.first << " second: " << range.second);

    const auto first = range.first;
    const auto last = range.second - 1;

    head = {first, first + 1 + (last - first - 1) / 2};
    tail = {head.second, range.second};
    if (0 == (last - first) % 2) {
      middle = head.second;
      ++(tail.first);
    }
  }
  std::pair<int, int> head;
  boost::optional<int> middle = boost::none;
  std::pair<int, int> tail;

  ~Zones() {
    assert_opt(head.first < head.second);
    assert_opt(boost::none == middle || head.second == *middle);
    assert_opt(head.second <= tail.first);
    assert_opt(tail.first < tail.second);
  }
};

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

  if (yRange.first + 1 == yRange.second) {
    // orizontal line, y does not depend on x
    return yRange.first;
  }

  const auto yZones = Zones(yRange);
  if (xRange.first + 1 == xRange.second) {
    // vertical line - return an approximation of the middle point on the y
    // range
    if (yZones.middle) {
      return *(yZones.middle);
    }
    return yZones.head.second - 1;
  }

  const auto xZones = Zones(xRange);
  if (x < xZones.head.second) {
    // x in the head of the x range corresponds to a value for y in the head of
    // the y range
    return compute(xZones.head, yZones.head, x);
  }
  if (xZones.middle && x == *(xZones.middle)) {
    // middle point in x correponds to the middle point in y (or an
    // approximation of it)
    if (boost::none == yZones.middle) {
      return yZones.head.second - 1;
    }
    return *(yZones.middle);
  }
  assert_opt(xZones.tail.first <= x);

  // x in the tail of the x range corresponds to a value for y in the tail of
  // the y range
  return compute(xZones.tail, yZones.tail, x);
}

template <typename C> C colour(C start, C end, int total, int coordinate) {
  assert_opt(coordinate >= 0);
  assert_opt(coordinate < total);

  // endpoints are trivial, return result
  if (0 == coordinate) {
    return start;
  }
  if (total - 1 == coordinate) {
    return end;
  }

  if (start.value > end.value) {
    return C(compute({0, total}, {end.value, start.value + 1},
                     total - 1 - coordinate));
  }
  return C(compute({0, total}, {start.value, end.value + 1}, coordinate));
}
}

namespace TestScreens {
DisplayNS::RGB565 colour(DisplayNS::RGB565 start, DisplayNS::RGB565 end,
                         int total, int coordinate) {
  using namespace DisplayNS;
  auto red = ::colour(start.red(), end.red(), total, coordinate);
  auto green = ::colour(start.green(), end.green(), total, coordinate);
  auto blue = ::colour(start.blue(), end.blue(), total, coordinate);

  // make sure that colours move in the right direction
  assert_safe(
      0 == coordinate ||
      (abs(end.red().value - red.value) <=
       abs(end.red().value -
           ::colour(start.red(), end.red(), total, coordinate - 1).value)));
  assert_safe(
      total - 1 == coordinate ||
      (abs(end.red().value - red.value) >=
       abs(end.red().value -
           ::colour(start.red(), end.red(), total, coordinate + 1).value)));
  assert_safe(
      0 == coordinate ||
      (abs(start.red().value - red.value) >=
       abs(start.red().value -
           ::colour(start.red(), end.red(), total, coordinate - 1).value)));
  assert_safe(
      total - 1 == coordinate ||
      (abs(start.red().value - red.value) <=
       abs(start.red().value -
           ::colour(start.red(), end.red(), total, coordinate + 1).value)));

  assert_safe(
      0 == coordinate ||
      (abs(end.green().value - green.value) <=
       abs(end.green().value -
           ::colour(start.green(), end.green(), total, coordinate - 1).value)));
  assert_safe(
      total - 1 == coordinate ||
      (abs(end.green().value - green.value) >=
       abs(end.green().value -
           ::colour(start.green(), end.green(), total, coordinate + 1).value)));
  assert_safe(
      0 == coordinate ||
      (abs(start.green().value - green.value) >=
       abs(start.green().value -
           ::colour(start.green(), end.green(), total, coordinate - 1).value)));
  assert_safe(
      total - 1 == coordinate ||
      (abs(start.green().value - green.value) <=
       abs(start.green().value -
           ::colour(start.green(), end.green(), total, coordinate + 1).value)));

  assert_safe(
      0 == coordinate ||
      (abs(end.blue().value - blue.value) <=
       abs(end.blue().value -
           ::colour(start.blue(), end.blue(), total, coordinate - 1).value)));
  assert_safe(
      total - 1 == coordinate ||
      (abs(end.blue().value - blue.value) >=
       abs(end.blue().value -
           ::colour(start.blue(), end.blue(), total, coordinate + 1).value)));
  assert_safe(
      0 == coordinate ||
      (abs(start.blue().value - blue.value) >=
       abs(start.blue().value -
           ::colour(start.blue(), end.blue(), total, coordinate - 1).value)));
  assert_safe(
      total - 1 == coordinate ||
      (abs(start.blue().value - blue.value) <=
       abs(start.blue().value -
           ::colour(start.blue(), end.blue(), total, coordinate + 1).value)));

  // make sure that the end-point are satisfied
  assert_safe(coordinate != 0 ||
              start.value == DisplayNS::RGB565{red, green, blue}.value);
  assert_safe(coordinate != total - 1 ||
              end.value == DisplayNS::RGB565{red, green, blue}.value);

  return {red, green, blue};
}
}
