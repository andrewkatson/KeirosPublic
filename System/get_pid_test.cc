// tests for get_pid.h

#include "get_pid.h"

#include "gtest/gtest.h"

namespace common {
namespace file {

class GetPidTest : public ::testing::Test {
  // No setup or persistence needed for the current tests. To see more on testing
  // https://github.com/google/googletest.
};

}  // namespace file
}  // namespace common

// Google test works some weird magic and we cannot test this normally...
void getPidTest() {

  std::vector<int> vec;

  getPid("dbus", &vec);


  // This could fail if your computer is running a different number of processes so do not fret.
  ASSERT_EQ(vec.size(), 3);

}

int main(int argc, char **argv) {
  getPidTest();
}