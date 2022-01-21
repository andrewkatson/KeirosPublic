/*
 * Matchers that simply capture a value passed to a function.
 */
#pragma once
#include <iostream>

#include "gmock/gmock.h"

// Define a matcher that just prints out whatever goes through it
MATCHER(Print, "Print the value of the argument passed") {
  std::cout << "The event \n" <<  arg.event->DebugString() << std::endl;
  if (arg.message != nullptr) {
    std::cout << "The message \n " << arg.message->DebugString() << std::endl;
  }
  return true;
}
