#include "testscreens/ramp.h"

#include "gtest/gtest.h"

#include <iostream>

// One colour per pixel
TEST(Ramp, OnePerPixel) {
  using namespace TestScreens;
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
  {
    const auto first = RGB565{Red{}, Green{0u}, Blue{0u}};
    const auto last = RGB565{Red{0u}, Green{0u}, Blue{0u}};

    for (auto i = 0u; i < 32u; ++i) {
      EXPECT_EQ(31u - i, TestScreens::colour(first, last, 32, i).red().value);
    }
  }
  {
    const auto first = RGB565{Red{0u}, Green{}, Blue{0u}};
    const auto last = RGB565{Red{0u}, Green{0u}, Blue{0u}};

    for (auto i = 0u; i < 64u; ++i) {
      EXPECT_EQ(63u - i, TestScreens::colour(first, last, 64, i).green().value);
    }
  }
  {
    const auto first = RGB565{Red{0u}, Green{0u}, Blue{}};
    const auto last = RGB565{Red{0u}, Green{0u}, Blue{0u}};

    for (auto i = 0u; i < 32u; ++i) {
      EXPECT_EQ(31u - i, TestScreens::colour(first, last, 32, i).blue().value);
    }
  }
}

// More colours than pixels
TEST(Ramp, Tall) {
  using namespace TestScreens;

  {
    const auto first = RGB565{Red{0u}, Green{0u}, Blue{0u}};
    const auto last = RGB565{Red{0u}, Green{0u}, Blue{}};

    EXPECT_EQ(0u, TestScreens::colour(first, last, 1, 0).blue().value);
  }
  {
    const auto first = RGB565{Red{0u}, Green{0u}, Blue{}};
    const auto last = RGB565{Red{0u}, Green{0u}, Blue{0u}};

    EXPECT_EQ(Blue{}.value, TestScreens::colour(first, last, 1, 0).blue().value);
  }
  {
    const auto first = RGB565{Red{0u}, Green{}, Blue{0u}};
    const auto last = RGB565{Red{0u}, Green{0u}, Blue{0u}};

    EXPECT_EQ(Green{}.value, TestScreens::colour(first, last, 3, 0).green().value);
    EXPECT_GE(Green{}.value, TestScreens::colour(first, last, 3, 1).green().value);
    EXPECT_EQ(Green{0u}.value, TestScreens::colour(first, last, 3, 2).green().value);
  }
  {
    const auto first = RGB565{Red{0u}, Green{}, Blue{0u}};
    const auto last = RGB565{Red{}, Green{0u}, Blue{0u}};

    EXPECT_EQ(Green{}.value, TestScreens::colour(first, last, 3, 0).green().value);
    EXPECT_EQ(Red{0u}.value, TestScreens::colour(first, last, 3, 0).red().value);

    EXPECT_GE(Green{}.value, TestScreens::colour(first, last, 3, 1).green().value);
    EXPECT_LE(Green{0u}.value, TestScreens::colour(first, last, 3, 1).green().value);
    EXPECT_LE(Red{0u}.value, TestScreens::colour(first, last, 3, 1).red().value);
    EXPECT_GE(Red{}.value, TestScreens::colour(first, last, 3, 1).red().value);

    EXPECT_EQ(Green{0u}.value, TestScreens::colour(first, last, 3, 2).green().value);
    EXPECT_EQ(Red{}.value, TestScreens::colour(first, last, 3, 2).red().value);
  }
  {
    const auto first = RGB565{Red{0u}, Green{}, Blue{0u}};
    const auto last = RGB565{Red{}, Green{0u}, Blue{0u}};

    EXPECT_EQ(Green{}.value, TestScreens::colour(first, last, 4, 0).green().value);
    EXPECT_EQ(Red{0u}.value, TestScreens::colour(first, last, 4, 0).red().value);

    EXPECT_GE(Green{}.value, TestScreens::colour(first, last, 4, 1).green().value);
    EXPECT_LE(Green{0u}.value, TestScreens::colour(first, last, 4, 1).green().value);
    EXPECT_LE(Red{0u}.value, TestScreens::colour(first, last, 4, 1).red().value);
    EXPECT_GE(Red{}.value, TestScreens::colour(first, last, 4, 1).red().value);

    EXPECT_GE(Green{}.value, TestScreens::colour(first, last, 4, 2).green().value);
    EXPECT_LE(Green{0u}.value, TestScreens::colour(first, last, 4, 2).green().value);
    EXPECT_LE(Red{0u}.value, TestScreens::colour(first, last, 4, 2).red().value);
    EXPECT_GE(Red{}.value, TestScreens::colour(first, last, 4, 2).red().value);

    EXPECT_EQ(Green{0u}.value, TestScreens::colour(first, last, 4, 3).green().value);
    EXPECT_EQ(Red{}.value, TestScreens::colour(first, last, 4, 3).red().value);
  }
}

// More pixels than colours
TEST(Ramp, Fat) {
  using namespace TestScreens;

  {
    const auto first = RGB565{Red{0u}, Green{}, Blue{0u}};
    const auto last = RGB565{Red{0u}, Green{}, Blue{0u}};

    for(auto i = 0u; i < 10u; ++i) {
      EXPECT_EQ(Red{0u}.value, TestScreens::colour(first, last, 10, i).red().value);
      EXPECT_EQ(Green{}.value, TestScreens::colour(first, last, 10, i).green().value);
      EXPECT_EQ(Blue{0u}.value, TestScreens::colour(first, last, 10, i).blue().value);
    }
  }
  {
    const auto first = RGB565{Red{0u}, Green{}, Blue{0u}};
    const auto last = RGB565{Red{0u}, Green{}, Blue{0u}};

    for(auto i = 0u; i < 9u; ++i) {
      EXPECT_EQ(Red{0u}.value, TestScreens::colour(first, last, 9, i).red().value);
      EXPECT_EQ(Green{}.value, TestScreens::colour(first, last, 9, i).green().value);
      EXPECT_EQ(Blue{0u}.value, TestScreens::colour(first, last, 9, i).blue().value);
    }
  }
}
