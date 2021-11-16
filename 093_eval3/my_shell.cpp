#include "my_shell.hpp"

#include <stdbool.h>

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

bool shell::init_envpath() {
  char * path_ = getenv("PATH");
  if (setenv("ECE551PATH", path_, 1) == 0) {
    return true;
  }
  std::cout << "init_envpath error!" << std::endl;
  std::cerr << "init_envpath error!" << std::endl;
  return false;
}

std::string shell::simplifyPath(std::string path) {
  std::string ans;
  std::string tmp;
  std::stringstream ss(path);
  std::vector<std::string> stk;
  while (getline(ss, tmp, '/')) {
    if (tmp == "" || tmp == ".")
      continue;
    if (tmp == ".." && !stk.empty()) {
      stk.pop_back();
    }
    else if (tmp != "..") {
      stk.push_back(tmp);
    }
  }
  for (auto & c : stk) {
    ans += "/" + c;
  }
  return ans.empty() ? "/" : ans;
}
