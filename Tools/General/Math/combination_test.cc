// Tests for combination.h

#include "combination.h"

#include <string>
#include <vector>

#include "gtest/gtest.h"

namespace common {
namespace tools {
namespace math {

  class CombinationTest : public ::testing::Test {
    // No setup or persistence needed for the current tests. To see more on testing
    // https://github.com/google/googletest.
  };

  TEST_F(CombinationTest, AllCombinationsMade) {
    std::vector<std::string> words;
    words.emplace_back("andrew");
    words.emplace_back("drew");
    words.emplace_back("nick");

    std::vector<std::vector<std::string>> combinations;
    allCombinations(words, &combinations);

    std::vector<std::vector<std::string>> expected;

    std::vector<std::string> expectedSizeThree;
    expectedSizeThree.emplace_back("andrew");
    expectedSizeThree.emplace_back("drew");
    expectedSizeThree.emplace_back("nick");

    std::vector<std::string> expectedSizeTwoFirst;
    expectedSizeTwoFirst.emplace_back("andrew");
    expectedSizeTwoFirst.emplace_back("drew");

    std::vector<std::string> expectedSizeTwoSecond;
    expectedSizeTwoSecond.emplace_back("andrew");
    expectedSizeTwoSecond.emplace_back("nick");

    std::vector<std::string> expectedSizeTwoThird;
    expectedSizeTwoThird.emplace_back("drew");
    expectedSizeTwoThird.emplace_back("nick");

    std::vector<std::string> expectedSizeOneFirst;
    expectedSizeOneFirst.emplace_back("andrew");

    std::vector<std::string> expectedSizeOneSecond;
    expectedSizeOneSecond.emplace_back("drew");

    std::vector<std::string> expectedSizeOneThird;
    expectedSizeOneThird.emplace_back("nick");

    expected.emplace_back(expectedSizeThree);
    expected.emplace_back(expectedSizeTwoFirst);
    expected.emplace_back(expectedSizeTwoSecond);
    expected.emplace_back(expectedSizeTwoThird);
    expected.emplace_back(expectedSizeOneFirst);
    expected.emplace_back(expectedSizeOneSecond);
    expected.emplace_back(expectedSizeOneThird);

    // Sort both vectors to get same order.
    std::sort(expected.begin(), expected.end());
    std::sort(combinations.begin(), combinations.end());
    ASSERT_EQ(expected, combinations);
  }

}  // namespace math
}  // namespace tools
}  // namespace common

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}