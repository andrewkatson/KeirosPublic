// tests for capture.h

#include "capture.h"

#include "gtest/gtest.h"

#include "Reactor/Base/event_and_message.h"
#include "Reactor/Proto/Event/Base/event.pb.h"

using google::protobuf::Timestamp;

using common::reactor::Event;

namespace protobuf {
namespace keiros {

// Simple class to run expectations on to check the matcher.
class TestingClass {
public:

  // Just takes an event.
  MOCK_METHOD(void, passProto, (const EventAndMessage& event));

  // Calls the method to take an event.
  inline void callPassProto(const EventAndMessage& event) {
    passProto(event);
  }

};

class CaptureTest : public ::testing::Test {
  // No setup or persistence needed for the current tests. To see more on testing
  // https://github.com/google/googletest.
};

TEST_F(CaptureTest, EventCaptured) {

  EventAndMessage eventAndMessage;
  eventAndMessage.message = absl::make_unique<Event>();
  auto* event = (Event*) eventAndMessage.message.get();
  event->set_checksum(10);

  TestingClass testingClass;

  EventAndMessage capturedEvent;
  capturedEvent.message = absl::make_unique<Event>();
  EXPECT_CALL(testingClass, passProto(Capture(&capturedEvent)));

  testingClass.callPassProto(eventAndMessage);

  auto* captured = (Event*) capturedEvent.message.get();
  ASSERT_EQ(captured->checksum(), event->checksum());
}

}  // namespace keiros
}  // namespace protobuf

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}