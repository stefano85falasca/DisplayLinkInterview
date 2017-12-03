#include "testscreens/testscreens.h"

#include "gtest/gtest.h"

TEST(Ramp, OnePerPixel) {
  using namespace DisplayNS;
  {
    const auto first = RGB565{Red{0u}, Green{0u}, Blue{0u}};
    const auto last = RGB565{Red{0u}, Green{0u}, Blue{}};

    for(auto i = 0u; i < 32u; ++i) {
      EXPECT_EQ(i, TestScreens::colour(first, last, 32, i).blue().value);
    }

    for(auto i = 0u; i < 32u; i += 2) {
      EXPECT_EQ(i, TestScreens::colour(first, last, 16, i).blue().value);
    }
  }
}
