// tests for read_text_proto.h

#include "read_text_proto.h"

#include "gtest/gtest.h"
#include "src/google/protobuf/timestamp.pb.h"

#include "ProtoBuf/Matchers/equals_proto.h"
#include "Reactor/Proto/Event/Base/event.pb.h"

using common::reactor::Event;

namespace protobuf {
namespace keiros {

  class ReadTextProtoTest : public ::testing::Test {
    // No setup or persistence needed for the current tests. To see more on testing
    // https://github.com/google/googletest.
  };

  TEST_F(ReadTextProtoTest, EventReadIn) {

    Event event;

    std::string path = "ProtoBuf/TextProto/TestData/event_proto.textpb";

    ASSERT_TRUE(readTextProto(path, &event));

    Event expectedEvent;
    expectedEvent.mutable_timestamp()->set_seconds(6);

    ASSERT_TRUE(ASSERT_EQ_PROTO(event, expectedEvent));
  }

}  // namespace keiros
}  // namespace protobuf

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}