#include "common/assert.h"
#include "testscreens/testscreens.h"
#include <array>
#include <boost/optional.hpp>
#include <iostream>
#include <limits>
#include <utility>

namespace {
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
    assert_opt(tail.first < tail.second);
  }
};

int compute(const std::pair<int, int> xRange, const std::pair<int, int> yRange,
            const int x) {
  assert_opt(xRange.first <= x);
  assert_opt(x < xRange.second);

  if (yRange.first + 1 == yRange.second) {
    return yRange.first;
  }

  const auto yZones = Zones(yRange);
  if (xRange.first + 1 == xRange.second) {
    if (yZones.middle) {
      return *(yZones.middle);
    }
    return yZones.head.second;
  }

  const auto xZones = Zones(xRange);
  if (x < xZones.head.second) {
    return compute(xZones.head, yZones.head, x);
  }
  if (xZones.middle && x == *(xZones.middle)) {
    if (boost::none == yZones.middle) {
      return compute({x, x + 1}, yZones.head, x);
    }
    return *(yZones.middle);
  }
  assert_opt(xZones.tail.first <= x);
  return compute(xZones.tail, yZones.tail, x);
}

template <typename C> C colour(C start, C end, int total, int coordinate) {
  assert_opt(coordinate >= 0);
  assert_opt(coordinate < total);

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

#if 0
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
#endif

  return {red, green, blue};
}
}
