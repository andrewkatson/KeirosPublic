/*
 * Retrieve the pid for a given process by name. If multiple processes with the same name are found we grab all of them.
 */

#ifndef GET_PID_H
#define GET_PID_H

#include <string>
#include <vector>

#include "absl/strings/str_split.h"

#include "execute.h"

inline void getPid(const std::string& processRegex, std::vector<int>* pid) {

  std::string command = "/bin/ps -a | /bin/grep \"" + processRegex +  "\"";

  std::string result = exec(command.c_str());

  std::vector<std::string> split = absl::StrSplit(result, absl::ByChar(' '));

  // Remove all the empty spaces.
  auto it = std::remove_if(split.begin(), split.end(), [](std::string& str) {return str.empty();});
  split.erase(it, split.end());

  // Take each first value of every four.
  int count = 0;

  for (auto& str : split) {

    if (count % 4 == 0) {
      pid->emplace_back(std::stoi(str));
    }
    count++;
  }

}

#endif // GET_PID_H
