#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <iostream>
#include <string>
#include <vector>

#include "my_shell.hpp"

int main() {
  std::string line;
  Shell My_Shell;
  //use getcwd commend to get the path
  char buf[1000];
  getcwd(buf, sizeof(buf));
  std::cout << "ffosh:" << buf << " $ ";
  while (getline(std::cin, line)) {
    //exit the program
    if (line == "exit()") {
      std::cout << "break now!!!!" << std::endl;
      break;
    }
    if (line != "") {
      //regular execute function
      My_Shell.execute(line);
      //My_Shell.go_exe(line);
      getcwd(buf, sizeof(buf));
      std::cout << "ffosh:" << buf << " $ ";
    }
    else {
      //if we pass enter as commend
      std::cout << "ffosh:" << buf << " $ ";
    }
  }
  return (EXIT_SUCCESS);
}
