#pragma once

#include <google/protobuf/io/zero_copy_stream_impl_lite.h>
#include "wolfssl/options.h"
#include "wolfssl/ssl.h"
#include "wolfssl/wolfcrypt/settings.h"

using namespace google::protobuf::io;

class AsioInputStream : public CopyingInputStream {
public:
    AsioInputStream(WOLFSSL* ssl);

    int Read(void *buffer, int size);
private:
  WOLFSSL* mSSL = nullptr;
};


AsioInputStream::AsioInputStream(WOLFSSL* ssl) : mSSL(ssl){}


int
AsioInputStream::Read(void *buffer, int size) {
    return wolfSSL_read(mSSL, (char*) buffer, size);
}


class AsioOutputStream : public CopyingOutputStream {
public:
    AsioOutputStream(WOLFSSL *ssl);

    bool Write(const void *buffer, int size);
private:
  WOLFSSL* mSSL = nullptr;
};



AsioOutputStream::AsioOutputStream(WOLFSSL* ssl) : mSSL(ssl) {}


bool
AsioOutputStream::Write(const void *buffer, int size) {
  return wolfSSL_write(mSSL, (char*) buffer, size) == size;
}