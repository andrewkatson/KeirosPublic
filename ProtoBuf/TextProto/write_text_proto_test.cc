// tests for write_text_proto.h

#include "write_text_proto.h"

#include "gtest/gtest.h"
#include "src/google/protobuf/timestamp.pb.h"

#include "ProtoBuf/Matchers/equals_proto.h"
#include "ProtoBuf/TextProto/read_text_proto.h"
#include "Reactor/Proto/Event/Base/event.pb.h"

using common::reactor::Event;

namespace protobuf {
namespace keiros {

  class WriteTextProtoTest : public ::testing::Test {
    // No setup or persistence needed for the current tests. To see more on testing
    // https://github.com/google/googletest.
  };

  TEST_F(WriteTextProtoTest, TextProtoWrites) {
    Event toWrite;
    toWrite.set_checksum(1000);

    std::string path = "ProtoBuf/TextProto/TestData/other_event.textpb";

    // Write it.
    writeTextProto(path, &toWrite);

    // Then read it.
    Event eventFromFile;
    readTextProto(path, &eventFromFile);

    // Compare.
    ASSERT_TRUE(ASSERT_EQ_PROTO(toWrite, eventFromFile));
  }

}  // namespace keiros
}  // namespace protobuf

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}