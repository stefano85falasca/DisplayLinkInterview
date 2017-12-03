#include "testscreens/testscreens.h"

#include "gtest/gtest.h"

#include <iostream>

TEST(Ramp, OnePerPixel) {
  using namespace DisplayNS;
  {
    const auto first = RGB565{Red{0u}, Green{0u}, Blue{0u}};
    const auto last = RGB565{Red{}, Green{0u}, Blue{0u}};

    for (auto i = 0u; i < 32u; ++i) {
      EXPECT_EQ(i, TestScreens::colour(first, last, 32, i).red().value);
    }
  }
  {
    const auto first = RGB565{Red{0u}, Green{0u}, Blue{0u}};
    const auto last = RGB565{Red{0u}, Green{}, Blue{0u}};

    for (auto i = 0u; i < 64u; ++i) {
      EXPECT_EQ(i, TestScreens::colour(first, last, 64, i).green().value);
    }
  }
  {
    const auto first = RGB565{Red{0u}, Green{0u}, Blue{0u}};
    const auto last = RGB565{Red{0u}, Green{0u}, Blue{}};

    for (auto i = 0u; i < 32u; ++i) {
      EXPECT_EQ(i, TestScreens::colour(first, last, 32, i).blue().value);
    }
  }
}
