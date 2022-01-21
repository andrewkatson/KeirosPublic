#ifndef EQUALS_PROTO_H
#define EQUALS_PROTO_H
/*
 * Matcher to use during a test to check the equality of two protocol buffers.
 * See: https://developers.google.com/protocol-buffers/docs/reference/cpp/google.protobuf.util.message_differencer#MessageDifferencer.Equals.details
 */

#include <typeinfo>

#include "gmock/gmock.h"
#include "src/google/protobuf/timestamp.pb.h"
#include "src/google/protobuf/util/message_differencer.h"

#include "Reactor/Proto/Event/Base/event.pb.h"

using google::protobuf::util::MessageDifferencer;

// Returns whether the two protos are equal and prints debug info.
template<typename P1, typename P2>
inline bool assertEqProto(const P1 &proto1, const P2 &proto2) {
  //std::cout << "First Proto: " << std::endl << proto1.DebugString() << std::endl;
  //std::cout << "Second Proto: " << std::endl << proto2.DebugString() << std::endl;

  MessageDifferencer messageDifferencer;
  // Ignore the seconds and nanos field for a timestamp.
  messageDifferencer.IgnoreField(google::protobuf::Timestamp().GetDescriptor()->FindFieldByLowercaseName("seconds"));
  messageDifferencer.IgnoreField(google::protobuf::Timestamp().GetDescriptor()->FindFieldByLowercaseName("nanos"));

  // Always ignore the timestamp field of an Event.
  messageDifferencer.IgnoreField(common::reactor::Event().GetDescriptor()->FindFieldByLowercaseName("timestamp"));

  bool equal = messageDifferencer.Compare(proto1, proto2);
  //std::cout << "These protos are " << std::boolalpha << equal << std::noboolalpha << std::endl;

  EXPECT_TRUE(equal);
  return equal;
}

// Returns whether the two protos are equal and prints debug info.
template<typename P1, typename P2>
inline bool assertNeProto(const P1 &proto1, const P2 &proto2) {
  //std::cout << "First Proto: " << std::endl << proto1.DebugString() << std::endl;
  //std::cout << "Second Proto: " << std::endl << proto2.DebugString() << std::endl;

  MessageDifferencer messageDifferencer;
  // Ignore the seconds and nanos field for a timestamp.
  messageDifferencer.IgnoreField(google::protobuf::Timestamp().GetDescriptor()->FindFieldByLowercaseName("seconds"));
  messageDifferencer.IgnoreField(google::protobuf::Timestamp().GetDescriptor()->FindFieldByLowercaseName("nanos"));

  // Always ignore the timestamp field of an Event.
  messageDifferencer.IgnoreField(common::reactor::Event().GetDescriptor()->FindFieldByLowercaseName("timestamp"));

  bool equal = messageDifferencer.Compare(proto1, proto2);
  //std::cout << "These protos are " << std::boolalpha << equal << std::noboolalpha << std::endl;

  EXPECT_FALSE(equal);
  return equal;
}


namespace protobuf {
namespace keiros {

// Defines a flavor of EqualsProto that checks that an arg matches proto passed
// Expects an EventAndMessage is the argument being passed.
// You pass an Event, an expected Message, and a default message
MATCHER_P3(EqualsStoredEvent, event, internalProtoToExpect, defaultInternalProto, "Casts the stored event to the same type as internal proto and then compares") {

  std::cout << "Actual External Proto: " << std::endl << arg.event->DebugString() << std::endl;
  std::cout << "Expected External Proto: " << std::endl << event.DebugString() << std::endl;

  MessageDifferencer messageDifferencer;
  // Ignore the seconds and nanos field for a timestamp.
  messageDifferencer.IgnoreField(google::protobuf::Timestamp().GetDescriptor()->FindFieldByLowercaseName("seconds"));
  messageDifferencer.IgnoreField(google::protobuf::Timestamp().GetDescriptor()->FindFieldByLowercaseName("nanos"));

  // Always ignore the timestamp field of an Event.
  messageDifferencer.IgnoreField(common::reactor::Event().GetDescriptor()->FindFieldByLowercaseName("timestamp"));

  // Now extract the stored proto.
  if (arg.message != nullptr) {
    auto *defaultInternal = (defaultInternalProto_type*) arg.message.get();

    std::cout << "Actual Internal Proto: " << std::endl << defaultInternal->DebugString() << std::endl;
    std::cout << "Expected Internal Proto: " << std::endl << internalProtoToExpect.DebugString() << std::endl;

    if (internalProtoToExpect.GetDescriptor() != arg.message->GetDescriptor()) {
      return false;
    }

    bool equal = messageDifferencer.Compare(internalProtoToExpect, *defaultInternal);
    //std::cout << "These protos are " << std::boolalpha << equal << std::noboolalpha << std::endl;

    return equal;
  }

  return false;
}

// Defines a matcher named EqualsProto that checks that an arg matches proto passed
// Expects the argument to be an EventAndMessage
// You pass a Message
MATCHER_P(EqualsProto, proto, "Checks that all fields are set to the same value") {
  std::cout << "Actual Event: " << std::endl << arg.event->DebugString() << std::endl;
  if (arg.message != nullptr) {
    std::cout << "Actual Message: " << std::endl << arg.message->DebugString() << std::endl;
  }
  std::cout << "Expected Proto: " << std::endl << proto.DebugString() << std::endl;

  MessageDifferencer messageDifferencer;
  // Ignore the seconds and nanos field for a timestamp.
  messageDifferencer.IgnoreField(google::protobuf::Timestamp().GetDescriptor()->FindFieldByLowercaseName("seconds"));
  messageDifferencer.IgnoreField(google::protobuf::Timestamp().GetDescriptor()->FindFieldByLowercaseName("nanos"));

  // Always ignore the timestamp field of an Event.
  messageDifferencer.IgnoreField(common::reactor::Event().GetDescriptor()->FindFieldByLowercaseName("timestamp"));

  bool equal = false;
  if (arg.message != nullptr) {
    if (arg.message->GetDescriptor() != proto.GetDescriptor()) {
      equal = false;
    } else {
      equal = messageDifferencer.Compare(*arg.message, *((google::protobuf::Message*) &proto));
    }
  }
  bool otherEqual = false;
  if (arg.event->GetDescriptor() != proto.GetDescriptor()) {
    otherEqual = false;
  } else {
    otherEqual = messageDifferencer.Compare(*((google::protobuf::Message*) arg.event.get()), proto);
  }
  //std::cout << "These protos are " << std::boolalpha << equal << std::noboolalpha << std::endl;
  return equal || otherEqual;
}

// Defines a matcher named EqualsRawProto that checks that an arg matches proto passed
// Expects the argument to be an Messaage
// You pass a Message
MATCHER_P(EqualsRawProto, proto, "Checks that all fields are set to the same value") {
  std::cout << "Actual Event: " << std::endl << arg.DebugString() << std::endl;
  std::cout << "Expected Proto: " << std::endl << proto.DebugString() << std::endl;

  MessageDifferencer messageDifferencer;
  // Ignore the seconds and nanos field for a timestamp.
  messageDifferencer.IgnoreField(google::protobuf::Timestamp().GetDescriptor()->FindFieldByLowercaseName("seconds"));
  messageDifferencer.IgnoreField(google::protobuf::Timestamp().GetDescriptor()->FindFieldByLowercaseName("nanos"));

  // Always ignore the timestamp field of an Event.
  messageDifferencer.IgnoreField(common::reactor::Event().GetDescriptor()->FindFieldByLowercaseName("timestamp"));

  return messageDifferencer.Compare(arg, proto);
}

// Defines a matcher named EqualsPtrProto that checks that an arg matches ptr to proto passed
// Expects the argument to be an EventAndMessage
// You pass a Message
MATCHER_P(EqualsPtrProto, proto, "Checks that all fields are set to the same value") {
  MessageDifferencer messageDifferencer;
  // Ignore the seconds and nanos field for a timestamp.
  messageDifferencer.IgnoreField(google::protobuf::Timestamp().GetDescriptor()->FindFieldByLowercaseName("seconds"));
  messageDifferencer.IgnoreField(google::protobuf::Timestamp().GetDescriptor()->FindFieldByLowercaseName("nanos"));

  // Always ignore the timestamp field of an Event.
  messageDifferencer.IgnoreField(common::reactor::Event().GetDescriptor()->FindFieldByLowercaseName("timestamp"));

  bool equal = false;
  if (arg->message != nullptr) {
    if (arg->message->GetDescriptor() == proto->GetDescriptor()) {
      equal = messageDifferencer.Compare(*arg->message, *proto);
    }
  }
  bool otherEqual = false;

  if (arg->event->GetDescriptor() == proto->GetDescriptor()) {
    otherEqual = messageDifferencer.Compare(*arg->event, *proto);
  }

  std::cout << "Actual Event: \n " + arg->event->DebugString();
  if (arg->message != nullptr) {
    std::cout << "Actual Message: \n" + arg->message->DebugString();
  }
  std::cout << "\n Expected: \n" + proto->DebugString();

  return equal || otherEqual;
}

// Defines a matcher that checks to see if the event received is the same type as the event passed.
// Expects the argument to be an EventAndMessage
// You pass an Event
MATCHER_P(IsEventOfType, proto, "Checks that the received event is of the same type as the one passed") {

  std::cout << "PROTO " << proto.DebugString() << std::endl;
  std::cout << "ARG " << arg.event->DebugString() << std::endl;

  bool equal = proto.type_id().DebugString() == arg.event->type_id().DebugString();
  std::cout << "These protos are " << std::boolalpha << equal << std::noboolalpha << std::endl;
  return equal;
}

// Defines a testing macro similar to ASSERT_EQ but just for protos.
#define ASSERT_EQ_PROTO(proto1, proto2) assertEqProto(proto1, proto2)

// Defines a testing macro similar to ASSERT_NE but just for protos.
#define ASSERT_NE_PROTO(proto1, proto2) assertNeProto(proto1, proto2)

} // keiros
} // protobuf
#endif // EQUALS_PROTO_H
