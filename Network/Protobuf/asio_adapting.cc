#include "asio_adapting.h"


AsioInputStream::AsioInputStream(WOLFSSL* ssl) : mSSL(ssl){}


int
AsioInputStream::Read(void *buffer, int size) {
  return wolfSSL_read(mSSL, (char*) buffer, size);
}


AsioOutputStream::AsioOutputStream(WOLFSSL* ssl) : mSSL(ssl) {}


bool
AsioOutputStream::Write(const void *buffer, int size) {
  return wolfSSL_write(mSSL, (char*) buffer, size) == size;
}