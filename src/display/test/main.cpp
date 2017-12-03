#include "gtest/gtest.h"

#include "common/assert.h"

TEST(TestEnvironment, Working) { EXPECT_TRUE(true); }

int main(int argc, char **argv) {
  {
    using namespace Assert;
    Handler::setHandler(Handler::availableHandlers::throwException);
  }
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
