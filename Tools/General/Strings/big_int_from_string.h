
#pragma once

#include <string>

#include "src/bigint.h"

namespace common {
namespace tools {

inline Dodecahedron::Bigint fromString(const std::string& str) {

  Dodecahedron::Bigint value = 0;
  for (int i = 0; i < str.length(); i++) {

    int digit = str.at(i);

    Dodecahedron::Bigint raise = digit;
    raise.pow(i);

    value = value + raise;
  }

  return value;
}

}  // tools
}  // common