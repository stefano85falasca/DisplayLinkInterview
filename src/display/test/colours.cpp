#include "display/colours.h"

#include "gtest/gtest.h"

TEST(RGB565, Gets) {
  using namespace DisplayNS;

  for (auto r = 0u; r < 32u; ++r) {
    for (auto g = 0u; g < 64u; ++g) {
      for (auto b = 0u; b < 32u; ++b) {
        auto rgb565 = RGB565{Red{r}, Green{g}, Blue{b}};
        EXPECT_EQ(r, rgb565.red().value);
        EXPECT_EQ(g, rgb565.green().value);
        EXPECT_EQ(b, rgb565.blue().value);
      }
    }
  }
}
