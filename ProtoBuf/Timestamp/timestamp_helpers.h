#ifndef TIMESTAMP_HELPERS_H
#define TIMESTAMP_HELPERS_H

// TODO make this owned by TimeReactor.

/*
 * Helper functions to use with Google's timestamp protocol buffer.
 * See: https://github.com/protocolbuffers/protobuf/blob/master/src/google/protobuf/timestamp.proto
 */

#include <chrono>
#include <iomanip>
#include <math.h>
#include <sstream>
#include <vector>

#include "absl/time/clock.h"
#include "absl/strings/str_format.h"
#include "absl/strings/str_split.h"
#include "absl/time/time.h"
#include "src/google/protobuf/duration.pb.h"
#include "src/google/protobuf/timestamp.pb.h"

#include "Tools/General/Math/bits.h"

namespace protobuf {
namespace keiros {

  // Delimitters used in common time formatting.
  // See https://www.w3.org/TR/NOTE-datetime.
  constexpr char cBeforeSecondsFraction = '.';
  constexpr char cBeforeTimezone = '+';

  constexpr int64_t cNanosInSecond = 1000000000;

  // How many digits to save when we are storing the fractions of a second.
  constexpr int64_t cStoringFractionsOfSecondsPrecision = 9;

  // How many digits of precision to ensure with any conversion to and from timestamps and durations.
  // This is considered in terms of seconds.
  constexpr double cConversionPrecision = 0.0000001;

  enum class TimestampComparison {
    BEFORE = 0,
    AFTER = 1,
    EQUAL = 2,
  };

  // Formats a protobuf timestamp into a utc timezone based version.
  // See ParseTime https://github.com/abseil/abseil-cpp/blob/da3a87690c56f965705b6a233d25ba5a3294067c/absl/time/time.h
  void formatTimestamp(const google::protobuf::Timestamp& ts, std::string* formattedTime);

  // Retrieves an accurate nanos from an absl::Time by using its string representation to avoid over/underflow.
  int32_t getNanosFromAbslTime(const absl::Time& t);

  // Retrieves an accurate nanos from absl::Duration by using a fraction to avoid over/underflow.
  int32_t getNanosFromAbslDuration(const absl::Duration& d);

  // Convert a google::protobuf::Timestamp to an absl::Time
  void toAbslTime(const google::protobuf::Timestamp& ts, absl::Time* at);

  // Convert a google::protobuf::Duration to an absl::Duration
  void toAbslDuration(const google::protobuf::Duration& d, absl::Duration* ad);

  // Convert an absl::Time to a google::protobuf::Timestamp
  void toTimestamp(const absl::Time& at, google::protobuf::Timestamp* ts);

  // Convert an absl::Duration to google::protobuf::Duration
  void toDuration(const absl::Duration ad, google::protobuf::Duration* d);

  // Pack a protobuf timestamp with absl::Now. This helps deal with the weirdness of nanoseconds.
  void nowAsTimestamp(google::protobuf::Timestamp* now);

  // All returned values are T1 relative to T2
  TimestampComparison compareTimestamps(const google::protobuf::Timestamp& t1,
      const google::protobuf::Timestamp& t2);

  TimestampComparison compareTimeToTimestampAndDuration(const absl::Time& timestamp,
      const absl::Duration& duration);

  // Whether the time specified as t1 + d1 is before, equal, or after the current time.
  TimestampComparison compareTimeToTimestampAndDuration(const google::protobuf::Timestamp& t1,
      const google::protobuf::Duration& d1);
} // keiros
} // protobuf

#endif // TIMESTAMP_HELPERS_H