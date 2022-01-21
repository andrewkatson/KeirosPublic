#ifndef BITS_H
#define BITS_H

/*
 * Some helpful math functions dealing with bits.
 */

#include <stdint-gcc.h>
#include <string>

namespace common {
namespace tools {
namespace math {

  // Pack two 32 bit integers into a 64 bit integer.
  inline void pack(uint32_t x, uint32_t y, uint64_t* toPack) {
    *(toPack) = ((uint64_t)x) << 32u | y;
  }

  // Pack a 64 bit integer into two 32 bit integers.
  inline void unpack(uint64_t unpack, uint32_t* toPackX, uint32_t* toPackY) {
    *(toPackX) = ((unpack & 0xFFFFFFFF00000000LLu) >> 32u);
    *(toPackY) = (unpack & 0xFFFFFFFFLLu);
  }

  // Keep the n digits specified as their own number.
  // For instance: 123456 with n = 3 --> 123
  inline void keepSignificantDigits(int64_t val, uint64_t digits, int64_t* truncatedVal) {

    std::string strRepr = std::to_string(val);

    if (strRepr.length() < digits) {
      *(truncatedVal) = val;
    }

    std::string truncatedStrRepr = strRepr.substr(0, digits);
    *(truncatedVal) = std::stoi(truncatedStrRepr);
  }

}  // math
}  // tools
}  // common

#endif // BITS_H