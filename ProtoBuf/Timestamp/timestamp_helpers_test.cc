// Tests for timestamp_helpers.h

#include "timestamp_helpers.h"

#include "absl/time/clock.h"
#include "absl/time/time.h"
#include "gtest/gtest.h"
#include "src/google/protobuf/duration.pb.h"
#include "src/google/protobuf/timestamp.pb.h"


namespace protobuf {
namespace keiros {

  class TimestampHelpersTest : public ::testing::Test {
    // No setup or persistence needed for the current tests. To see more on testing
    // https://github.com/google/googletest.
  };

  TEST_F(TimestampHelpersTest, EarlyTimestampBySecondsIsLessThanLaterOne) {
    absl::Time firstTime = absl::Now();

    google::protobuf::Timestamp t1;

    t1.set_seconds(absl::ToUnixSeconds(firstTime));

    absl::Time secondTime = absl::InfiniteFuture();

    google::protobuf::Timestamp t2;

    t2.set_seconds(absl::ToUnixSeconds(secondTime));

    TimestampComparison comparison = compareTimestamps(t1, t2);

    ASSERT_EQ(comparison, TimestampComparison::BEFORE);
  }

  TEST_F(TimestampHelpersTest, LaterTimestampByNanosIsLessThanLaterOne) {
    absl::Duration nanosToChange = absl::FromChrono(std::chrono::nanoseconds (3123456));
    absl::Time firstTime = absl::UniversalEpoch() + nanosToChange;

    google::protobuf::Timestamp t1;
    toTimestamp(firstTime, &t1);

    absl::Time secondTime = absl::UniversalEpoch();

    google::protobuf::Timestamp t2;
    toTimestamp(secondTime, &t2);

    TimestampComparison comparison = compareTimestamps(t1, t2);

    ASSERT_EQ(comparison, TimestampComparison::AFTER);
  }

  TEST_F(TimestampHelpersTest, CurrentTimeAfterTimestampAndDuration) {

    // This represents a start time or something along those lines.
    absl::Duration fiveMinutesInPast = absl::FromChrono(std::chrono::seconds(300));
    absl::Time time = absl::Now() - fiveMinutesInPast;

    // This represents a duration that should be considered with the time. Like a limiting length of time.
    absl::Duration duration = absl::FromChrono(std::chrono::seconds(10));

    // Expect that time plus duration is BEFORE the current time.
    TimestampComparison expected = TimestampComparison::BEFORE;

    ASSERT_EQ(expected, compareTimeToTimestampAndDuration(time, duration));
  }

  TEST_F(TimestampHelpersTest, TimestampConversions) {
    absl::Duration nanosToChange = absl::FromChrono(std::chrono::nanoseconds (3123456));
    absl::Time firstTime = absl::UniversalEpoch() + nanosToChange;

    google::protobuf::Timestamp t1;
    toTimestamp(firstTime, &t1);

    absl::Time backFromTime;
    toAbslTime(t1, &backFromTime);

    ASSERT_EQ(firstTime, backFromTime);
  }

  TEST_F(TimestampHelpersTest, DurationConversions) {
    absl::Duration d1 = absl::FromChrono(std::chrono::nanoseconds(53558909809));

    google::protobuf::Duration duration;

    toDuration(d1, &duration);

    absl::Duration convertedBack;

    toAbslDuration(duration, &convertedBack);

    // We expect some smearing.
    absl::Duration durationDifference = d1 - convertedBack;
    int64_t difference = absl::ToInt64Seconds(durationDifference);
    ASSERT_TRUE(difference <= cConversionPrecision && difference >= cConversionPrecision * -1.0);
  }

}  // namespace keiros
}  // namespace protobuf

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}