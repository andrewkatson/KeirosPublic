#ifndef READ_TEXT_PROTO_H
#define READ_TEXT_PROTO_H
/*
 * Helper class that reads in a protobuf in text format and uses that to fill out a protocol buffer of a specific type.
 */

#include <fstream>
#include <iostream>
#include <string>

#include "src/google/protobuf/text_format.h"
#include "src/google/protobuf/io/zero_copy_stream_impl.h"

namespace protobuf {
namespace keiros {

  // Reads in the text proto at path into proto. Returns true if successful and false otherwise.
  template<typename T>
  bool readTextProto(const std::string& path, T* proto){

    bool ret = false;

    std::ifstream protoFile;

    protoFile.open(path, std::ifstream::in);

    if (protoFile.fail()) {
      ret = false;
      std::cerr << "Error: " << strerror(errno) << " " << path << std::endl;
    } else {
      google::protobuf::io::IstreamInputStream isStream(&protoFile);

      if (!google::protobuf::TextFormat::Parse(&isStream, proto))
      {
        std::cerr << std::endl << "Failed to parse file!" << std::endl;
      } else {
        ret = true;
      }

      protoFile.close();
    }

    return ret;
  }

  // Reads a text proto from its debug string.
  template <typename T>
  bool readTextProtoFromDebugString(const std::string& debugString, T* proto) {
    return google::protobuf::TextFormat::ParseFromString(debugString, proto);
  }

} // keiros
} // protobuf

#endif // READ_TEXT_PROTO_H


