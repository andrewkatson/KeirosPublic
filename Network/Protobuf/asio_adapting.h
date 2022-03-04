#pragma once

#include <google/protobuf/io/zero_copy_stream_impl_lite.h>
#ifdef _WIN32
#include "IDE/WIN10/user_settings.h"
#include "wolfssl/wolfcrypt/settings.h"
#else
#include "wolfssl/options.h"
#endif
#include "wolfssl/ssl.h"

using namespace google::protobuf::io;

class AsioInputStream : public CopyingInputStream {
public:
    explicit AsioInputStream(WOLFSSL* ssl);

    int Read(void *buffer, int size) override;
private:
  WOLFSSL* mSSL = nullptr;
};




class AsioOutputStream : public CopyingOutputStream {
public:
    explicit AsioOutputStream(WOLFSSL *ssl);

    bool Write(const void *buffer, int size) override;
private:
  WOLFSSL* mSSL = nullptr;
};


