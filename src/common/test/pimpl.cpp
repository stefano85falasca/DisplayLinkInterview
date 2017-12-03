#undef PADS_ASSERT_ENABLED
#undef PADS_ASSERT_OPT_ENABLED
#undef PADS_ASSERT_SAFE_ENABLED

#include "pimpl_header.h"

#include "gtest/gtest.h"

TEST(Pimpl, NoArgs) {
  PimpldClass p;
  EXPECT_TRUE(p.working());
}

TEST(Pimpl, Args) {
  {
    PimpldClass p(false, 1, '1');
    EXPECT_FALSE(p.working());
  }
  {
    PimpldClass p(true, 1, '1');
    EXPECT_TRUE(p.working());
  }
}
