#include "networking_client.h"

bool NetworkingClient::mSetup = false;

WOLFSSL_CTX* NetworkingClient::mCtx = nullptr;

absl::flat_hash_map<std::string, WOLFSSL*> NetworkingClient::mWolfssl;

void NetworkingClient::cleanupWolfssl() {
  wolfSSL_CTX_free(mCtx);

  wolfSSL_Cleanup();
}

void NetworkingClient::cleanupWolfssl(WOLFSSL* ssl) {
  wolfSSL_free(ssl);
  cleanupWolfssl();
}

void NetworkingClient::cleanupWolfssl(const common::network::IP &ip) {

  std::string fullyQualified;
  ipToString(ip, &fullyQualified);

  if (mWolfssl.contains(fullyQualified)) {
    cleanupWolfssl(mWolfssl.find(fullyQualified)->second);
  }

}

void NetworkingClient::ipToString(const common::network::IP &ip, std::string* str) {
  *str = absl::StrCat(ip.block(), ":", std::to_string(ip.port()));
}

bool NetworkingClient::setupAsServer(const std::string &caCertPath, const std::string &serverCertPath, const std::string &serverKeyPath) {

  if (mSetup) {
    return false;
  }

  wolfSSL_Init();  /* Initialize wolfSSL */

  /* Create the WOLFSSL_CTX */
  if ((mCtx = wolfSSL_CTX_new(wolfSSLv23_server_method())) == NULL) {
    std::cerr << "Wolfssl context setup failure" << std::endl;
    cleanupWolfssl();
    return false;
  }

  /* Load CA certificates into CYASSL_CTX */
  if (wolfSSL_CTX_load_verify_locations(mCtx, caCertPath.c_str(), 0) != SSL_SUCCESS) {
    std::cerr << "Wolfssl ca cert loading failure" << std::endl;
    cleanupWolfssl();
    return false;
  }



  /* Load server certificates into WOLFSSL_CTX */
  if (wolfSSL_CTX_use_certificate_file(mCtx, serverCertPath.c_str(), SSL_FILETYPE_PEM) != SSL_SUCCESS) {
    std::cerr << "Wolfssl server cert loading failure" << std::endl;
    cleanupWolfssl();
    return false;
  }

  /* Load keys */
  if (wolfSSL_CTX_use_PrivateKey_file(mCtx, serverKeyPath.c_str(), SSL_FILETYPE_PEM) != SSL_SUCCESS) {
    std::cerr << "Wolfssl server key loading failure" << std::endl;
    cleanupWolfssl();
    return false;
  }

  mSetup = true;
  return true;
}

bool NetworkingClient::setupAsClient(const std::string &caCertPath) {

  if (mSetup) {
    return false;
  }

  wolfSSL_Init();/* Initialize wolfSSL */

  /* Create the WOLFSSL_CTX */
  if ((mCtx = wolfSSL_CTX_new(wolfSSLv23_client_method())) == NULL) {
    std::cerr << "Wolfssl context setup failure" << std::endl;
    cleanupWolfssl();
    return false;
  }

  /* Load CA certificates into WOLFSSL_CTX */
  if (wolfSSL_CTX_load_verify_locations(mCtx, caCertPath.c_str(), 0) != SSL_SUCCESS) {
    std::cerr << "Wolfssl ca cert loading failure" << std::endl;
    cleanupWolfssl();
    return false;
  }

  mSetup = true;
  return true;
}

WOLFSSL* NetworkingClient::setupWolfsslForConnection(const common::network::IP &ip) {

  std::string fullyQualified;
  ipToString(ip, &fullyQualified);

  SOCKET_T sockfd = 0;

  if  (mWolfssl.contains(fullyQualified)) {
    return mWolfssl.find(fullyQualified)->second;
  } else {
    WOLFSSL* ssl;

    if( (ssl = wolfSSL_new(mCtx)) == NULL) {
      std::cerr << "Wolfssl setup ssl failure" << std::endl;
      return nullptr;
    }

    wolfSSL_set_fd(ssl, sockfd);

    mWolfssl.insert(std::make_pair(fullyQualified, ssl));

    return ssl;
  }
}

bool NetworkingClient::send(const common::network::IP &ip, const common::reactor::Event &event) {

  WOLFSSL *ssl = setupWolfsslForConnection(ip);

  AsioOutputStream aos (ssl);
  CopyingOutputStreamAdaptor cos_adp(&aos);

  google::protobuf::io::writeDelimitedTo(event, &cos_adp);
  // Now we have to flush, otherwise the write to the socket won't happen until enough bytes accumulate
  cos_adp.Flush();

  return true;
}

bool NetworkingClient::send(const common::network::IP &ip, const EventAndMessage &event) {

  event.event->set_stored_event(event.message->SerializeAsString());

  return send(ip, *event.event);
}

bool NetworkingClient::receive(const common::network::IP &ip, common::reactor::Event *event) {

  WOLFSSL *ssl = setupWolfsslForConnection(ip);

  AsioInputStream ais (ssl);
  CopyingInputStreamAdaptor cis_adp(&ais);

  google::protobuf::io::readDelimitedFrom(&cis_adp, event);

  return true;
}

bool NetworkingClient::receive(const common::network::IP &ip, EventAndMessage *event) {
  return receive(ip, event->event.get());
}
