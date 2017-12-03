#undef PADS_ASSERT_ENABLED
#undef PADS_ASSERT_OPT_ENABLED
#undef PADS_ASSERT_SAFE_ENABLED

#include "common/assert.h"

#include "gtest/gtest.h"

TEST(NoneEnabled, Optimised) {
  using namespace Assert;
  EXPECT_NO_THROW(assert_opt(true));

  EXPECT_NO_THROW(assert_opt(false));
  EXPECT_NO_THROW(assert_opt_message(false,
                                     "the quick brown fox "
                                         << "jumps over " << 1 << "lazy dog"));
}

TEST(NoneEnabled, Normal) {
  using namespace Assert;
  EXPECT_NO_THROW(assert_normal(true));

  EXPECT_NO_THROW(assert_normal(false));
  EXPECT_NO_THROW(assert_normal_message(false,
                                        "the quick brown fox "
                                            << "jumps over " << 1
                                            << "lazy dog"));
}

TEST(NoneEnabled, Safe) {
  using namespace Assert;
  EXPECT_NO_THROW(assert_safe(true));

  EXPECT_NO_THROW(assert_safe(false));
  EXPECT_NO_THROW(assert_safe_message(false,
                                      "the quick brown fox "
                                          << "jumps over " << 1 << "lazy dog"));
}

TEST(NoneEnabled, CodeNotExecuted) {
  using namespace Assert;
  EXPECT_NO_THROW(assert_opt(throw 1, true));
  EXPECT_NO_THROW(assert_normal(throw 1, true));
  EXPECT_NO_THROW(assert_safe(throw 1, true));
}
