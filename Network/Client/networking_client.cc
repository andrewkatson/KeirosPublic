#include "networking_client.h"

bool NetworkingClient::mSetup = false;

bool NetworkingClient::mIsServer = false;

WOLFSSL_CTX *NetworkingClient::mCtx = nullptr;

absl::flat_hash_map<std::string, WOLFSSL *> NetworkingClient::mWolfssl;

absl::flat_hash_map <std::string, SOCKET_T> NetworkingClient::mSocket;

absl::flat_hash_map <std::string, SOCKET_T> NetworkingClient::mConnSocket;

void NetworkingClient::cleanupWolfssl() {
  wolfSSL_CTX_free(mCtx);
  wolfSSL_Cleanup();
  mSetup = false;
}

void NetworkingClient::cleanupWolfssl(WOLFSSL *ssl) {
  wolfSSL_shutdown(ssl);
  wolfSSL_free(ssl);
}

void NetworkingClient::cleanupWolfssl(const common::network::IP &ip) {

  std::string fullyQualified;
  ipToString(ip, &fullyQualified);

  // Cleanup the SSL object
  if (mWolfssl.contains(fullyQualified)) {
    cleanupWolfssl(mWolfssl.find(fullyQualified)->second);
  }

  mWolfssl.erase(fullyQualified);

  // Cleanup the Sockets
  if (mSocket.contains(fullyQualified)) {
    close(mSocket.find(fullyQualified)->second);
  }

  mSocket.erase(fullyQualified);

  if (mConnSocket.contains(fullyQualified)) {
    close(mConnSocket.find(fullyQualified)->second);
  }

  mConnSocket.erase(fullyQualified);
}

void NetworkingClient::ipToString(const common::network::IP &ip, std::string *str) {
  *str = absl::StrCat(ip.block(), ":", std::to_string(ip.port()));
}

bool NetworkingClient::setupAsServer(const std::string &caCertPath, const std::string &serverCertPath,
                                     const std::string &serverKeyPath) {

  if (mSetup) {
    return false;
  }

  wolfSSL_Init();  /* Initialize wolfSSL */

  /* Create the WOLFSSL_CTX */
  if ((mCtx = wolfSSL_CTX_new(wolfSSLv23_server_method())) == nullptr) {
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
  mIsServer = true;
  return true;
}

bool NetworkingClient::setupAsClient(const std::string &caCertPath) {

  if (mSetup) {
    return false;
  }

  wolfSSL_Init();/* Initialize wolfSSL */

  /* Create the WOLFSSL_CTX */
  if ((mCtx = wolfSSL_CTX_new(wolfSSLv23_client_method())) == nullptr) {
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
  mIsServer = false;
  return true;
}

WOLFSSL *NetworkingClient::setupWolfsslForConnection(const common::network::IP &ip, bool isServer) {

  std::string fullyQualified;
  ipToString(ip, &fullyQualified);

  if (mWolfssl.contains(fullyQualified)) {
    return mWolfssl.find(fullyQualified)->second;
  } else {
    SOCKET_T sockfd = SOCKET_INVALID;
    SOCKET_T connd = SOCKET_INVALID;
    /*
    * Create a socket that uses an internet IPv4 address,
    * Sets the socket to be stream based (TCP),
    * 0 means choose the default protocol.
    */
    if ((sockfd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0)) == -1) {
      std::cerr << "Connecting to socket failed" << std::endl;
      return nullptr;
    }

    struct sockaddr_in servAddr;

    /* Initialize the server address struct with zeros */
    memset(&servAddr, 0, sizeof(servAddr));

    if (isServer) {
      /* Fill in the server address */
      servAddr.sin_family = AF_INET;             /* using IPv4      */
      servAddr.sin_port = ip.port();           /* on chosen port */
      servAddr.sin_addr.s_addr = INADDR_ANY;          /* from any address   */

      /* Bind the server socket to our port */
      if (bind(sockfd, (struct sockaddr *) &servAddr, sizeof(servAddr)) == -1) {
        std::cerr << "Binding to socket failed" << std::endl;
        return nullptr;
      }

      /* Listen for a new connection, allow 5 pending connections */
      if (listen(sockfd, 5) == -1) {
        std::cerr << "Listening to socket failed" << std::endl;
        return nullptr;
      }

      struct sockaddr_in clientAddr;
      socklen_t          size = sizeof(clientAddr);

      /* Accept client connections */
      if ((connd = accept4(sockfd, (struct sockaddr *) &clientAddr, &size, SOCK_NONBLOCK))
          == -1) {
        std::cerr << "Failed to accept connection" << std::endl;
        return nullptr;
      }

    } else {

      /* Fill in the server address */
      servAddr.sin_family = AF_INET;             /* using IPv4      */
      servAddr.sin_port = ip.port(); /* on set port */

      /* Get the server IPv4 address from the command line call */
      if (inet_pton(AF_INET, ip.block().c_str(), &servAddr.sin_addr) != 1) {
        std::cerr << "Invalid address" << std::endl;
        return nullptr;
      }

      /* Connect to the server */
      if (connect(sockfd, (struct sockaddr *) &servAddr, sizeof(servAddr))
          == -1) {
        std::cerr << "Failed to connect" << std::endl;
        return nullptr;
      }

    }

    WOLFSSL *ssl;

    if ((ssl = wolfSSL_new(mCtx)) == nullptr) {
      std::cerr << "Wolfssl setup ssl failure" << std::endl;
      cleanupWolfssl(ssl);
      return nullptr;
    }

    int ret;

    /* Attach wolfSSL to the socket */
    if ((ret = wolfSSL_set_fd(ssl, isServer ? connd : sockfd)) != WOLFSSL_SUCCESS) {
      std::cerr << "Wolfssl set fd failure" << std::endl;
      cleanupWolfssl(ssl);
      return nullptr;
    }

    if (isServer) {
      /* Establish TLS connection */
      ret = wolfSSL_accept(ssl);
      if (ret != WOLFSSL_SUCCESS) {
        std::cerr << "Failed to accept wolfssl connection" << std::endl;
        cleanupWolfssl(ssl);
        return nullptr;
      }
    } else {
      /* Connect to wolfSSL on the server side */
      if ((ret = wolfSSL_connect(ssl)) != SSL_SUCCESS) {
        std::cerr << "Wolfssl failed to connect" << std::endl;
        cleanupWolfssl(ssl);
        return nullptr;
      }
    }


    mWolfssl.insert(std::make_pair(fullyQualified, ssl));

    mSocket.insert(std::make_pair(fullyQualified, sockfd));

    mConnSocket.insert(std::make_pair(fullyQualified, connd));
    return ssl;
  }
}

bool NetworkingClient::send(const common::network::IP &ip, const common::reactor::Event &event) {

  WOLFSSL *ssl = setupWolfsslForConnection(ip, mIsServer);

  if (ssl == nullptr) {
    return false;
  }

  AsioOutputStream aos(ssl);
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

  WOLFSSL *ssl = setupWolfsslForConnection(ip, mIsServer);

  if (ssl == nullptr) {
    return false;
  }

  AsioInputStream ais(ssl);
  CopyingInputStreamAdaptor cis_adp(&ais);

  google::protobuf::io::readDelimitedFrom(&cis_adp, event);

  return true;
}

bool NetworkingClient::receive(const common::network::IP &ip, EventAndMessage *event) {
  return receive(ip, event->event.get());
}
