#undef PADS_ASSERT_ENABLED
#undef PADS_ASSERT_OPT_ENABLED
#undef PADS_ASSERT_SAFE_ENABLED

#define PADS_ASSERT_OPT_ENABLED
#include "common/assert.h"
#include "string_compare.h"

#include "gtest/gtest.h"

TEST(OptimisedEnabled, Optimised) {
  using namespace Assert;
  EXPECT_NO_THROW(assert_opt(true));

  EXPECT_THROW(assert_opt(false), Handler::Failure);
  EXPECT_THROW(assert_opt_message(false,
                                  "the quick brown fox "
                                      << "jumps over " << 1 << "lazy dog"),
               Handler::Failure);

  try {
    assert_opt_message(false,
                       "the quick brown fox "
                           << "jumps over " << 1 << " lazy dog");
  } catch (Handler::Failure &e) {
    EXPECT_TRUE(
        begins_with(std::string{"the quick brown fox jumps over 1 lazy dog"},
                    std::string{e.what()}))
        << e.what();
  }
}

TEST(OptimisedEnabled, Normal) {
  using namespace Assert;
  EXPECT_NO_THROW(assert_normal(true));

  EXPECT_NO_THROW(assert_normal(false));
  EXPECT_NO_THROW(assert_normal_message(false,
                                        "the quick brown fox "
                                            << "jumps over " << 1
                                            << "lazy dog"));
}

TEST(OptimisedEnabled, Safe) {
  using namespace Assert;
  EXPECT_NO_THROW(assert_safe(true));

  EXPECT_NO_THROW(assert_safe(false));
  EXPECT_NO_THROW(assert_safe_message(false,
                                      "the quick brown fox "
                                          << "jumps over " << 1 << "lazy dog"));
}

TEST(OptimisedEnabled, CodeExecuted) {
  using namespace Assert;
  EXPECT_THROW(assert_opt(throw 1, true), int);
  EXPECT_NO_THROW(assert_normal(throw 1, true));
  EXPECT_NO_THROW(assert_safe(throw 1, true));
}
