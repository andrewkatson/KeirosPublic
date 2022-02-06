#include "networking_client.h"

bool NetworkingClient::mSetup = false;

bool NetworkingClient::mIsServer = false;

WOLFSSL_CTX *NetworkingClient::mCtx = nullptr;

std::unordered_map<std::string, WOLFSSL *> NetworkingClient::mWolfssl;


#ifdef _WIN32
std::unordered_map<std::string, SOCKET> NetworkingClient::mSocket;

std::unordered_map<std::string, SOCKET> NetworkingClient::mConnSocket;
#else
std::unordered_map<std::string, SOCKET_T> NetworkingClient::mSocket;

std::unordered_map<std::string, SOCKET_T> NetworkingClient::mConnSocket;
#endif

void NetworkingClient::cleanupWolfssl() {
  wolfSSL_CTX_free(mCtx);
  wolfSSL_Cleanup();
#ifdef _WIN32
  WSACleanup();
#endif
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
  if (mWolfssl.find(fullyQualified) != mWolfssl.end()) {
    cleanupWolfssl(mWolfssl.find(fullyQualified)->second);
  }

  mWolfssl.erase(fullyQualified);

  // Cleanup the Sockets
  if (mSocket.find(fullyQualified) != mSocket.end()) {
#ifdef _WIN32
    shutdown(mSocket.find(fullyQualified)->second, SD_BOTH);
    closesocket(mSocket.find(fullyQualified)->second);
#else
    close(mSocket.find(fullyQualified)->second);
#endif
  }

  mSocket.erase(fullyQualified);

  if (mConnSocket.find(fullyQualified) != mConnSocket.end()) {
#ifdef _WIN32
    shutdown(mConnSocket.find(fullyQualified)->second, SD_BOTH);
    closesocket(mConnSocket.find(fullyQualified)->second);
#else
    close(mConnSocket.find(fullyQualified)->second);
#endif
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

  int result = SSL_SUCCESS;

  /* Create the WOLFSSL_CTX */
  if ((mCtx = wolfSSL_CTX_new(wolfSSLv23_server_method())) == nullptr) {
    std::cerr << "Wolfssl context setup failure" << std::endl;
    cleanupWolfssl();
    return false;
  }

  /* Load CA certificates into CYASSL_CTX */
  result = wolfSSL_CTX_load_verify_locations(mCtx, caCertPath.c_str(), 0);
  if (result != SSL_SUCCESS) {
    std::cerr << "Wolfssl ca cert loading failure " << result << std::endl;
    cleanupWolfssl();
    return false;
  }



  /* Load server certificates into WOLFSSL_CTX */
  result = wolfSSL_CTX_use_certificate_file(mCtx, serverCertPath.c_str(), SSL_FILETYPE_PEM);
  if (result != SSL_SUCCESS) {
    std::cerr << "Wolfssl server cert loading failure " << result << std::endl;
    cleanupWolfssl();
    return false;
  }

  /* Load keys */
  result = wolfSSL_CTX_use_PrivateKey_file(mCtx, serverKeyPath.c_str(), SSL_FILETYPE_PEM);
  if (result != SSL_SUCCESS) {
    std::cerr << "Wolfssl server key loading failure " << result << std::endl;
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
  int result = wolfSSL_CTX_load_verify_locations(mCtx, caCertPath.c_str(), 0);

  if (result != SSL_SUCCESS) {
    std::cerr << "Wolfssl ca cert loading failure " << result << std::endl;
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

  if (mWolfssl.find(fullyQualified) != mWolfssl.end()) {
    return mWolfssl.find(fullyQualified)->second;
  } else {

#ifdef _WIN32
    SOCKET ConnectSocket = INVALID_SOCKET;
    SOCKET ListenSocket = INVALID_SOCKET;
    SOCKET ClientSocket = INVALID_SOCKET;

    if (!mIsServer) {
      WSADATA wsaData;
      struct addrinfo *result = NULL,
                      *ptr = NULL,
                      hints;

      int iResult;

      // Initialize Winsock
      iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
      if (iResult != 0) {
          std::cerr << "WSAStartup failed with error " << iResult << std::endl;
          return nullptr;
      }

      ZeroMemory( &hints, sizeof(hints) );
      hints.ai_family = AF_UNSPEC;
      hints.ai_socktype = SOCK_STREAM;
      hints.ai_protocol = IPPROTO_TCP;

      // Resolve the server address and port
      iResult = getaddrinfo(ip.block().c_str(), std::to_string(ip.port()).c_str(), &hints, &result);
      if ( iResult != 0 ) {
          std::cerr << "getaddrinfo failure with error " << iResult << std::endl;
          WSACleanup();
          return nullptr;
      }

      // Attempt to connect to an address until one succeeds
      for(ptr=result; ptr != NULL ;ptr=ptr->ai_next) {

          // Create a SOCKET for connecting to server
          ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
              ptr->ai_protocol);
          if (ConnectSocket == INVALID_SOCKET) {
              std::cerr << "Socket failed with error " << WSAGetLastError() << std::endl;
              WSACleanup();
              return nullptr;
          }

          // Connect to server.
          iResult = connect( ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
          if (iResult == SOCKET_ERROR) {
              closesocket(ConnectSocket);
              ConnectSocket = INVALID_SOCKET;
              continue;
          }
          break;
      }

      freeaddrinfo(result);

      if (ConnectSocket == INVALID_SOCKET) {
          std::cerr << "Unable to connect to server " << std::endl;
          WSACleanup();
          return nullptr;
      }
    } else {
      WSADATA wsaData;
      int iResult;

      struct addrinfo *result = NULL;
      struct addrinfo hints;

      // Initialize Winsock
      iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
      if (iResult != 0) {
          std::cerr << "WSAStartup failed with error " << iResult << std::endl;
          return nullptr;
      }

      ZeroMemory(&hints, sizeof(hints));
      hints.ai_family = AF_INET;
      hints.ai_socktype = SOCK_STREAM;
      hints.ai_protocol = IPPROTO_TCP;
      hints.ai_flags = AI_PASSIVE;

      // Resolve the server address and port
      iResult = getaddrinfo(NULL, std::to_string(ip.port()).c_str(), &hints, &result);
      if ( iResult != 0 ) {
          std::cerr << "getaddrinfo failure with error " << iResult << std::endl;
          WSACleanup();
          return nullptr;
      }

      // Create a SOCKET for connecting to server
      ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
      if (ListenSocket == INVALID_SOCKET) {
          std::cerr << "socket failed with error " << WSAGetLastError() << std::endl;
          freeaddrinfo(result);
          WSACleanup();
          return nullptr;
      }

      // Setup the TCP listening socket
      iResult = bind( ListenSocket, result->ai_addr, (int)result->ai_addrlen);
      if (iResult == SOCKET_ERROR) {
          std::cerr << "bind failed with error " << WSAGetLastError() << std::endl;
          freeaddrinfo(result);
          closesocket(ListenSocket);
          WSACleanup();
          return nullptr;
      }

      freeaddrinfo(result);

      iResult = listen(ListenSocket, SOMAXCONN);
      if (iResult == SOCKET_ERROR) {
          std::cerr << "listen failed with error " << WSAGetLastError() << std::endl;
          closesocket(ListenSocket);
          WSACleanup();
          return nullptr;
      }

      // Accept a client socket
      ClientSocket = accept(ListenSocket, NULL, NULL);
      if (ClientSocket == INVALID_SOCKET) {
          std::cerr << "accept failed with error " << WSAGetLastError() << std::endl;
          closesocket(ListenSocket);
          WSACleanup();
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
    if ((ret = wolfSSL_set_fd(ssl, isServer ? ClientSocket : ConnectSocket)) != WOLFSSL_SUCCESS) {
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

    mSocket.insert(std::make_pair(fullyQualified, isServer ? ListenSocket : ConnectSocket));

    mConnSocket.insert(std::make_pair(fullyQualified, ClientSocket));
    return ssl;

#else
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
      socklen_t size = sizeof(clientAddr);

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
#endif
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

JNIEXPORT jboolean

JNICALL Java_com_keiros_client_network_NetworkingClient_setupAsClient(JNIEnv *env, jobject obj, jstring caCertPath) {
  jboolean isCopy;
  const char *convertedValue = (env)->GetStringUTFChars(caCertPath, &isCopy);
  std::string caCertPathCopy = std::string(convertedValue);

  bool success = NetworkingClient::setupAsClient(hex_to_string(caCertPathCopy));

  env->ReleaseStringUTFChars(caCertPath, convertedValue);

  return success;
}

JNIEXPORT jboolean

JNICALL
Java_com_keiros_client_network_NetworkingClient_setupAsServer(JNIEnv *env, jobject obj, jstring caCertPath, jstring serverCertPath, jstring serverKeyPath) {
  const char *convertedValue = (env)->GetStringUTFChars(caCertPath, nullptr);
  std::string caCertPathCopy = std::string(convertedValue);

  const char *convertedValueTwo = (env)->GetStringUTFChars(serverCertPath, nullptr);
  std::string serverCertPathCopy = std::string(convertedValueTwo);

  const char *convertedValueThree = (env)->GetStringUTFChars(serverKeyPath, nullptr);
  std::string serverKeyPathCopy = std::string(convertedValueThree);

  bool success =  NetworkingClient::setupAsServer(hex_to_string(caCertPathCopy), hex_to_string(serverCertPathCopy), hex_to_string(serverKeyPathCopy));

  env->ReleaseStringUTFChars(caCertPath, convertedValue);
  env->ReleaseStringUTFChars(serverCertPath, convertedValueTwo);
  env->ReleaseStringUTFChars(serverKeyPath, convertedValueThree);

  return success;
}

JNIEXPORT jboolean

JNICALL Java_com_keiros_client_network_NetworkingClient_send(JNIEnv *env, jobject obj, jstring ip, jstring event) {

  const char *convertedValue = (env)->GetStringUTFChars(ip, nullptr);
  std::string ipCopy = std::string(convertedValue);

  const char *convertedValueTwo = (env)->GetStringUTFChars(event, nullptr);
  std::string eventCopy = std::string(convertedValueTwo);

  common::network::IP ipObj;
  ipObj.ParseFromString(hex_to_string(ipCopy));

  common::reactor::Event eventObj;
  eventObj.ParseFromString(hex_to_string(eventCopy));

  bool success = NetworkingClient::send(ipObj, eventObj);

  env->ReleaseStringUTFChars(ip, convertedValue);
  env->ReleaseStringUTFChars(event, convertedValueTwo);

  return success;
}

JNIEXPORT jboolean

JNICALL Java_com_keiros_client_network_NetworkingClient_receive(JNIEnv *env, jobject obj, jstring ip, jstring event) {
  const char *convertedValue = (env)->GetStringUTFChars(ip, nullptr);
  std::string ipCopy = std::string(convertedValue);

  const char *convertedValueTwo = (env)->GetStringUTFChars(event, nullptr);
  std::string eventCopy = std::string(convertedValueTwo);

  common::network::IP ipObj;
  ipObj.ParseFromString(hex_to_string(ipCopy));

  common::reactor::Event eventObj;
  eventObj.ParseFromString(hex_to_string(eventCopy));


  jboolean success = NetworkingClient::receive(ipObj, &eventObj);

  std::string eventAsString = string_to_hex(eventObj.SerializeAsString());

  const char *convertedBack = eventAsString.c_str();
  *event = *env->NewStringUTF(convertedBack);

  env->ReleaseStringUTFChars(ip, convertedValue);
  env->ReleaseStringUTFChars(event, convertedValueTwo);

  return success;
}

JNIEXPORT void JNICALL Java_com_keiros_client_network_NetworkingClient_cleanupWolfsslWithIp(JNIEnv * env, jobject obj, jstring ip) {
  const char *convertedValue = (env)->GetStringUTFChars(ip, nullptr);

  std::string ipCopy = std::string(convertedValue);

  common::network::IP ipObj;
  ipObj.ParseFromString(hex_to_string(ipCopy));

  NetworkingClient::cleanupWolfssl(ipObj);

  env->ReleaseStringUTFChars(ip, convertedValue);
}

JNIEXPORT void JNICALL Java_com_keiros_client_network_NetworkingClient_cleanupWolfssl(JNIEnv * env, jobject obj) {
  NetworkingClient::cleanupWolfssl();
}