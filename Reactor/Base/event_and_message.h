#pragma once

#pragma once

#include <memory>

#include "absl/memory/memory.h"
#include "src/google/protobuf/message.h"

#include "Reactor/Proto/Event/Base/event.pb.h"

/**
 * An event and a generic message.
 */
class EventAndMessage {
public:
  std::unique_ptr<common::reactor::Event> event = absl::make_unique<common::reactor::Event>();
  std::unique_ptr<google::protobuf::Message> message = nullptr;
};