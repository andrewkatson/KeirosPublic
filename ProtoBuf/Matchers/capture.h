/*
 * Matchers that simply capture a value passed to a function.
 */

#ifndef CAPTURE_H
#define CAPTURE_H

#include "gmock/gmock.h"

// Defines a matcher named Capture that captures the value of the proto passed to the function this matcher is used with
// Takes an EventAndMessage and assumes the function being called is passed an EventAndMessage
MATCHER_P(Capture, proto, "Capture the value of the argument passed") {

  if (proto->message != nullptr && arg.message != nullptr) {
    proto->message->CopyFrom(*arg.message);
  }
  proto->event->CopyFrom(*arg.event);
  return true;
}

#endif // CAPTURE_H
