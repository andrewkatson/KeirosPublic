
#include "gtest/gtest.h"

#include "networking_client.h"

class NetworkingClientTest : public ::testing::Test {

};

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}