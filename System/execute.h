/*
 * Execute a command through the shell.
 */

#ifndef EXECUTE_H
#define EXECUTE_H

#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>

// Executes a command and returns the output as a string.
inline std::string exec(const char* cmd) {
  std::array<char, 128> buffer{};
  std::string result;
  std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
  if (!pipe) {
    std::cerr << strerror(errno) << std::endl;
    throw std::runtime_error("popen() failed!");
  }
  while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
    result += buffer.data();
  }
  return result;
}

#endif // EXECUTE_H
