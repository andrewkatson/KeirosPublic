// Tests for bits.h

#include "bits.h"

#include "gtest/gtest.h"

namespace common {
namespace tools {
namespace math {

    class BitsTest : public ::testing::Test {
      // No setup or persistence needed for the current tests. To see more on testing
      // https://github.com/google/googletest.
    };

    TEST_F(BitsTest, PackAndUnpack) {

      uint32_t x = 31233;
      uint32_t y = 23458;
      uint64_t packed;

      pack(x, y, &packed);

      uint32_t z;
      uint32_t t;

      unpack(packed, &z, &t);
    }

    TEST_F(BitsTest, KeepDigitsEqualToLength) {

      int64_t val = 123456;
      int digits = 3;

      int64_t truncated;
      keepSignificantDigits(val, digits, &truncated);
      ASSERT_EQ(truncated, 123);
    }

    TEST_F(BitsTest, KeepDigitsGreaterThanLength) {
      int64_t val = 123456;
      int digits = 8;

      int64_t truncated;
      keepSignificantDigits(val, digits, &truncated);
      ASSERT_EQ(truncated, val);
    }

}  // namespace math
}  // namespace tools
}  // namespace common

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}