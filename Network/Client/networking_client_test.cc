
#include "gtest/gtest.h"

#include "networking_client.h"

class NetworkingClientTest : public ::testing::Test {
public:
   static void cleanupWolfssl(const common::network::IP& ip) {
    NetworkingClient::cleanupWolfssl(ip);
    NetworkingClient::cleanupWolfssl();
  }

};

TEST_F(NetworkingClientTest, SendAsServer) {
  common::network::IP ip;
  ip.set_block("127.0.0.1");
  ip.set_port(8080);

  ASSERT_TRUE(NetworkingClient::setupAsServer("Network/Client/ca.pem", "Network/Client/test-server.pem",
                                  "Network/Client/test.key.pem"));

  common::reactor::Event event;
  event.set_stored_event("some_event");

  // We expect the send to fail because we have no pending client connections.
  ASSERT_FALSE(NetworkingClient::send(ip, event));

  cleanupWolfssl(ip);
}

TEST_F(NetworkingClientTest, SendAsClient) {
  common::network::IP ip;
  ip.set_block("127.0.0.1");
  ip.set_port(8080);

  ASSERT_TRUE(NetworkingClient::setupAsClient("Network/Client/ca.pem"));

  common::reactor::Event event;
  event.set_stored_event("some_event");

  // We expect the send to fail because we have nothing to connect to.
  ASSERT_FALSE(NetworkingClient::send(ip, event));

  cleanupWolfssl(ip);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}