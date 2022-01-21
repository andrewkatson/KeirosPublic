#include "timestamp_helpers.h"

namespace protobuf {
namespace keiros {

  void formatTimestamp(const google::protobuf::Timestamp& ts, std::string* formattedTime) {
    absl::TimeZone utc =  absl::UTCTimeZone();

    // First we take the seconds which we know won't cause an overflow and convert.
    absl::Time justSeconds = absl::FromUnixSeconds(ts.seconds());

    // Get the formatted version.
    std::string timeAsStr = absl::FormatTime(justSeconds, utc);

    // Split the string along the barrier between the date and time info and the timezone.
    std::vector<std::string> splitIntoBeforeTimezone = absl::StrSplit(timeAsStr, cBeforeTimezone);
    std::string beforeSecondsFraction = splitIntoBeforeTimezone.at(0);
    std::string afterSecondsFraction = splitIntoBeforeTimezone.at(1);

    // Calculate our seconds fraction using the nanos.
    long double realSecondsFraction = (double) ts.nanos() / (double) cNanosInSecond ;
    long double intPart;
    long double fractionalPart = modf(realSecondsFraction, &intPart);
    std::stringstream stream;
    stream << std::fixed << std::setprecision(cStoringFractionsOfSecondsPrecision) << fractionalPart;
    std::string fractionAsString = stream.str();
    std::vector<std::string> splitOnDecimal = absl::StrSplit(fractionAsString, ".");
    std::string secondsFractionAsStr = splitOnDecimal.at(1);

    // Complete the string.
    *(formattedTime) = absl::StrFormat("%s%c%s%c%s", beforeSecondsFraction, cBeforeSecondsFraction,
                                       secondsFractionAsStr, cBeforeTimezone, afterSecondsFraction);
  }

  int32_t getNanosFromAbslTime(const absl::Time& t) {
    absl::TimeZone utc =  absl::UTCTimeZone();

    // See https://abseil.io/docs/cpp/guides/time#formatting-absltime
    std::string timeAsStr = absl::FormatTime(t, utc);

    // First split off everything before the seconds fraction.
    std::vector<std::string> splitIntoBeforeSecondsFraction = absl::StrSplit(timeAsStr, cBeforeSecondsFraction);

    // If no nanos then just return 0.
    if (splitIntoBeforeSecondsFraction.size() < 2) {
      return 0;
    }
    std::string secondsFractionAndAfter = splitIntoBeforeSecondsFraction.at(1);

    // Then split off everything after the seconds fraction.
    std::vector<std::string> splitIntoBeforeTimezone = absl::StrSplit(secondsFractionAndAfter, cBeforeTimezone);
    std::string secondsFraction = splitIntoBeforeTimezone.at(0);

    // Now, convert to nanos.
    // TODO investigate if this causes weird issues when nanos is really large
    long double nanosAsSecondsFraction = std::stod("0." + secondsFraction);
    int32_t nanos = nanosAsSecondsFraction * cNanosInSecond;
    return nanos;
  }

  int32_t getNanosFromAbslDuration(const absl::Duration& d) {

    // We want the seconds as a fraction so we can use the fractional part as the nanos.
    long double secondsAsFraction = absl::ToDoubleSeconds(d);

    long double intPart;
    long double fractionalPart = modf(secondsAsFraction, &intPart);

    // TODO investigate if this causes weird issues when nanos is really large
    int32_t nanos = fractionalPart * cNanosInSecond;
    return nanos;
  }

  void toAbslTime(const google::protobuf::Timestamp& ts, absl::Time* at) {

    // We want to get each part of the timestamp.
    // And then combine.
    std::string formattedTime;
    formatTimestamp(ts, &formattedTime);

    std::string errorMessage;
    bool success = absl::ParseTime(absl::RFC3339_full, formattedTime, at, &errorMessage);

    if (success == false) {
      throw std::runtime_error(errorMessage);
    }
  }

  void toAbslDuration(const google::protobuf::Duration& d, absl::Duration* ad) {


    auto chronoNanos = std::chrono::nanoseconds(d.nanos());
    absl::Duration justNanos = absl::FromChrono(chronoNanos);


    auto chronoSeconds = std::chrono::seconds(d.seconds());
    absl::Duration justSeconds = absl::FromChrono(chronoSeconds);

    *(ad) = justNanos + justSeconds;
  }

  void toTimestamp(const absl::Time& at, google::protobuf::Timestamp* ts) {
    ts->set_seconds(absl::ToUnixSeconds(at));

    ts->set_nanos(getNanosFromAbslTime(at));
  }

  void toDuration(const absl::Duration ad, google::protobuf::Duration* d) {
    d->set_seconds(absl::ToInt64Seconds(ad));

    d->set_nanos(getNanosFromAbslDuration(ad));
  }

  void nowAsTimestamp(google::protobuf::Timestamp* now) {
    absl::Time abslNow = absl::Now();
    toTimestamp(abslNow, now);
  }

  TimestampComparison compareTimestamps(const google::protobuf::Timestamp& t1,
                                               const google::protobuf::Timestamp& t2) {

    TimestampComparison secondsComparison = TimestampComparison::EQUAL;
    TimestampComparison nanosComparison = TimestampComparison::EQUAL;

    if (t1.seconds() < t2.seconds()) {
      secondsComparison = TimestampComparison::BEFORE;
    } else if (t1.seconds() > t2.seconds()) {
      secondsComparison = TimestampComparison::AFTER;
    }

    if (secondsComparison == TimestampComparison::EQUAL) {
      if (t1.nanos() < t2.nanos()) {
        nanosComparison = TimestampComparison::BEFORE;
      } else if (t1.nanos() > t2.nanos()) {
        nanosComparison = TimestampComparison::AFTER;
      }
      return nanosComparison;
    } else {
      // If the two timestamps do not have equal seconds all we need is to return the seconds comparison.
      return secondsComparison;
    }
  }

  TimestampComparison compareTimeToTimestampAndDuration(const absl::Time& timestamp,
                                                               const absl::Duration& duration) {

    // Current time.
    absl::Time timeInQuestion = timestamp + duration;
    absl::Time currentTime = absl::Now();
    if (timeInQuestion < currentTime) {
      return TimestampComparison::BEFORE;
    }  else if(timeInQuestion > currentTime) {
      return TimestampComparison::AFTER;
    } else {
      return TimestampComparison::EQUAL;
    }
  }

  TimestampComparison compareTimeToTimestampAndDuration(const google::protobuf::Timestamp& t1,
                                                               const google::protobuf::Duration& d1) {

    // We want everything in an absl container.
    absl::Duration duration;
    toAbslDuration(d1, &duration);

    // We want timestamp as absl container.
    absl::Time timestamp;
    toAbslTime(t1, &timestamp);

    return compareTimeToTimestampAndDuration(timestamp, duration);
  }
} // keiros
} // protobuf