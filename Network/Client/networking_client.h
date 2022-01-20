#pragma once
/*
 * A client that can be used to send generic protobufs encapsulated as Event or EventAndMessage
 */
#include <iostream>
#include <string>

#include "absl/container/flat_hash_map.h"
#include "absl/strings/str_cat.h"
#include "wolfssl/options.h"
#include "wolfssl/ssl.h"
#include "wolfssl/wolfcrypt/settings.h"

#include "Network/Identity/Proto/ip.pb.h"
#include "Network/Protobuf/asio_adapting.h"
#include "Network/Protobuf/protobuf_helpers.h"
#include "Reactor/Base/event_and_message.h"

class NetworkingClient {
public:
  // Setup as a client. Return true on success, false otherwise.
  static bool setupAsClient(const std::string &caCertPath);

  // Setup as a server. Return true on success, false otherwise.
  static bool
  setupAsServer(const std::string &caCertPath, const std::string &serverCertPath, const std::string &serverKeyPath);

  // Send some data.
  static bool send(const common::network::IP& ip, const common::reactor::Event& event);
  static bool send(const common::network::IP& ip, const EventAndMessage& event);

  // Receive some data.
  static bool receive(const common::network::IP& ip, common::reactor::Event* event);
  static bool receive(const common::network::IP& ip, EventAndMessage* event);

  // Cleanup a connection.
  static void cleanupWolfssl(const common::network::IP &ip);

  // Cleanup the client.
  static void cleanupWolfssl();
private:
  // Setup a Wolfssl object.
  static WOLFSSL* setupWolfsslForConnection(const common::network::IP &ip, bool isServer);

  // Whether we have setup already.
  static bool mSetup;

  // Whether we are the server or not.
  static bool mIsServer;

  // The wolfssl context object.
  static WOLFSSL_CTX *mCtx;

  // The wolfssl ssl objects used for connecting to different ip:port combinations.
  // Keyed by the ip:port string.
  static absl::flat_hash_map<std::string, WOLFSSL*> mWolfssl;

  // The socket connection object used for connecting to different ip:port combinations.
  // Keyed by the ip:port string.
  static absl::flat_hash_map<std::string, SOCKET_T> mSocket;

  // The socket connection object used for connecting to different ip:port combinations from the server.
  // This is used in addition to the socket above.
  // Keyed by the ip:port string.
  static absl::flat_hash_map<std::string, SOCKET_T> mConnSocket;

  // Cleanup any wolfssl stuff.
  static void cleanupWolfssl(WOLFSSL *ssl);

  // Turn an ip proto into a string.
  static void ipToString(const common::network::IP& ip, std::string* str);
};
