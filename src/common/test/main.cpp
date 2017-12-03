#include "common/assert.h"
#include "gtest/gtest.h"

TEST(TestEnvironment, Working) {}

int main(int argc, char **argv) {
  {
    using namespace Assert;
    Handler::setHandler(Handler::availableHandlers::throwException);
  }
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
