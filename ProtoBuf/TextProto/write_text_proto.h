#ifndef WRITE_TEXT_PROTO_H
#define WRITE_TEXT_PROTO_H
/*
 * Helper class that writes a protobuf to a file using its text format.
 */

#include <fstream>
#include <iostream>
#include <string>

#include "src/google/protobuf/text_format.h"
#include "src/google/protobuf/io/zero_copy_stream_impl.h"

namespace protobuf {
namespace keiros {

  // Writes the proto to the path specified. Returns true if successful and false otherwise.
  template<typename T>
  bool writeTextProto(const std::string& path, T* proto){

    bool ret = false;

    std::ofstream protoFile(path, std::ofstream::out);

    if (protoFile.fail()) {
      ret = false;
      std::cerr << "Write Error: " << strerror(errno) << std::endl;
    } else {

      std::string debugStringProto = proto->DebugString();

      protoFile << debugStringProto;

      protoFile.close();
    }

    return ret;
  }

} // keiros
} // protobuf

#endif // WRITE_TEXT_PROTO_H


