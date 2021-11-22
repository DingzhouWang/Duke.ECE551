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
  //Shell_cmd My_Cmd;
  //My_Cmd.init_map();

  //size_t cnt = 1;
  //size_t num = argc - 1;
  //My_Cmd.init_map();
  //My_Cmd.print_map("$USER-is-using-$TERM $");
  //My_Cmd.print_map("$USER-is-using-$TERM-");
  //My_Cmd.print_map("$USER-is-using-$TERM_");
  //My_Cmd.print_map("$USER-is-using-$TERM$USER");
  char buf[1000];
  getcwd(buf, sizeof(buf));
  //while (num) {
  //  std::string tmp_arg = argv[cnt];
  //  line = line + tmp_arg + " ";
  //  num--;
  //  cnt++;
  //}
  //std::cout << line << std::endl;
  //if (line.size()) {
  //  My_Shell.execute(line);
  //}
  //std::cout << line << std::endl;
  std::cout << "ffosh:" << buf << " $ ";
  while (getline(std::cin, line)) {
    if (line == "exit()") {
      std::cout << "break now!!!!" << std::endl;
      break;
    }
    if (line != "") {
      //if ((int)line.find("$") != -1) {
      //cout << "IN" << endl;
      //My_Cmd.print_map(line);
      //continue;
      //}

      //std::cout << line << std::endl;
      //process_cmd(line);
      //Shell My_Shell;
      My_Shell.execute(line);
      getcwd(buf, sizeof(buf));
      std::cout << "ffosh:" << buf << " $ ";
    }
    else {
      std::cout << "ffosh:" << buf << " $ ";
    }
  }
  return (EXIT_SUCCESS);
}
