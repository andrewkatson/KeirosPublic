// tests for equals_proto.h

#include "equals_proto.h"

#include "gtest/gtest.h"
#include "src/google/protobuf/timestamp.pb.h"

#include "Reactor/Base/event_and_message.h"
#include "Reactor/Proto/Event/Base/event.pb.h"
#include "Security/Proto/identifier.pb.h"

using google::protobuf::Timestamp;

using common::reactor::Event;

namespace protobuf {
namespace keiros {

// Simple class to run expectations on to check the matcher.
class TestingClass {
public:

  // Just takes an event.
  MOCK_METHOD(void, passProto, (const EventAndMessage& event));

  // Just takes an event in a ptr
  MOCK_METHOD(void, passPtrProto, (EventAndMessage * event));

  // Calls the method to take an event.
  inline void callPassProto(const EventAndMessage &event) {
    passProto(event);
  }

  // Calls the method to take a ptr.
  inline void callPassPtrProto(EventAndMessage *event) {
    passPtrProto(event);
  }
};

class EqualsProtoTest : public ::testing::Test {
  // No setup or persistence needed for the current tests. To see more on testing
  // https://github.com/google/googletest.
};

TEST_F(EqualsProtoTest, EqualsProtoTopLevelFieldsMatch) {
  Event e1;

  e1.set_stored_event("blah");

  e1.set_communication_part(Event::REQUEST);

  EventAndMessage e2;

  e2.event->set_stored_event("blah");

  e2.event->set_communication_part(Event::REQUEST);

  TestingClass testingClass;

  EXPECT_CALL(testingClass, passProto(EqualsProto(e1))).Times(1);

  testingClass.callPassProto(e2);
}

TEST_F(EqualsProtoTest, EqualsProtoNestedFieldsMatch) {
  Event e1;

  Timestamp *t1 = e1.mutable_timestamp();

  // We expect timestamps to be ignored during comparison
  t1->set_seconds(13);

  EventAndMessage e2;

  Timestamp *t2 = e2.event->mutable_timestamp();

  t2->set_seconds(1);

  TestingClass testingClass;

  EXPECT_CALL(testingClass, passProto(EqualsProto(e1))).Times(1);

  testingClass.callPassProto(e2);
}

TEST_F(EqualsProtoTest, EqualsProtoWithPtrTopLevelFieldsMatch) {
  Event e1;

  e1.set_stored_event("blah");

  e1.set_communication_part(Event::REQUEST);

  EventAndMessage e2;

  e2.event->set_stored_event("blah");

  e2.event->set_communication_part(Event::REQUEST);

  TestingClass testingClass;

  EXPECT_CALL(testingClass, passPtrProto(EqualsPtrProto(&e1))).Times(1);

  testingClass.callPassPtrProto(&e2);
}

TEST_F(EqualsProtoTest, EqualsProtoWithPtrNestedFieldsMatch) {
  Event e1;

  Timestamp *t1 = e1.mutable_timestamp();

  // We expect timestamps to be ignored during comparison
  t1->set_seconds(10);

  EventAndMessage e2;

  Timestamp *t2 = e2.event->mutable_timestamp();

  t2->set_seconds(1);

  TestingClass testingClass;

  EXPECT_CALL(testingClass, passPtrProto(EqualsPtrProto(&e1))).Times(1);

  testingClass.callPassPtrProto(&e2);
}

TEST_F(EqualsProtoTest, AssertEqProtoTopLevelFieldsMatch) {
  Event e1;

  e1.set_stored_event("blah");

  e1.set_communication_part(Event::REQUEST);

  Event e2;

  e2.set_stored_event("blah");

  e2.set_communication_part(Event::REQUEST);

  TestingClass testingClass;

  ASSERT_TRUE(ASSERT_EQ_PROTO(e1, e2));
}

TEST_F(EqualsProtoTest, AssertEqProtoNestedFieldsMatch) {
  Event e1;

  Timestamp *t1 = e1.mutable_timestamp();

  // We expect timestamps to be ignored during comparison
  t1->set_seconds(1);

  Event e2;

  Timestamp *t2 = e2.mutable_timestamp();

  t2->set_seconds(10);

  TestingClass testingClass;

  ASSERT_TRUE(ASSERT_EQ_PROTO(e1, e2));
}

TEST_F(EqualsProtoTest, EqualsStoredEventTest) {
  EventAndMessage e1;

  e1.message = absl::make_unique<common::security::Identifier>();
  auto* identifier = (common::security::Identifier*) e1.message.get();
  identifier->set_type(common::security::Identifier::EVENT_TYPE_ID);

  Event e2;

  common::security::Identifier expected;
  expected.set_type(common::security::Identifier::EVENT_TYPE_ID);

  e2.set_stored_event(expected.SerializeAsString());

  TestingClass testingClass;

  EXPECT_CALL(testingClass, passProto(EqualsStoredEvent(e2, expected,
                                                        common::security::Identifier::default_instance()))).Times(1);

  testingClass.callPassProto(e1);
}

TEST_F(EqualsProtoTest, IsProtoOfType) {

  EventAndMessage e1;
  e1.event->mutable_type_id()->set_type(common::security::Identifier::EVENT_TYPE_ID);
  e1.event->mutable_type_id()->add_string_part("TypeA");

  Event expected;
  expected.mutable_type_id()->set_type(common::security::Identifier::EVENT_TYPE_ID);
  expected.mutable_type_id()->add_string_part("TypeA");

  TestingClass testingClass;

  EXPECT_CALL(testingClass, passProto(IsEventOfType(expected)));

  testingClass.callPassProto(e1);
}
}  // namespace keiros
}  // namespace protobuf

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}