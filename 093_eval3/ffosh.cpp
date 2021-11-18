#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <iostream>
#include <string>
#include <vector>

#include "my_shell.hpp"

int main(int argc, char * argv[]) {
  std::string line;
  Shell My_Shell;
  size_t cnt = 1;
  size_t num = argc - 1;
  while (num) {
    std::string tmp_arg = argv[cnt];
    line = line + tmp_arg + " ";
    num--;
    cnt++;
  }
  //std::cout << line << "1" << std::endl;
  if (line.size()) {
    My_Shell.execute(line);
  }
  //std::cout << line << std::endl;
  std::cout << "ffosh$ ";
  while (getline(std::cin, line)) {
    if (line == "exit()") {
      std::cout << "break now!!!!" << std::endl;
      break;
    }
    //std::cout << line << std::endl;
    //process_cmd(line);
    //Shell My_Shell;
    My_Shell.execute(line);
    std::cout << "ffosh$ ";
  }
  return (EXIT_SUCCESS);
}
