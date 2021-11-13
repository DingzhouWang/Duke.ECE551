#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <iostream>
#include <string>
#include <vector>

void process_cmd(const std::string & line) {
  pid_t p1 = -1;
  //std::vector<std::string> cmd;
  p1 = fork();
  if (p1 < 0) {
    std::cerr << "cannot fork!" << std::endl;
    exit(EXIT_FAILURE);
  }
  else if (p1 > 0) {
    int status;
    //learn from linux man page
    pid_t wpid = waitpid(p1, &status, WUNTRACED);
    if (wpid == -1) {
      std::cerr << "waitpid error!" << std::endl;
      exit(EXIT_FAILURE);
    }
    if (WIFEXITED(status)) {
      if (WEXITSTATUS(status) == 0)
        std::cout << "Program was successful" << std::endl;
      else
        std::cout << "Program failed with code " << WEXITSTATUS(status) << std::endl;
    }
    if (WIFSIGNALED(status)) {
      std::cout << "Terminated by signal " << WTERMSIG(status) << std::endl;
    }
  }
  else if (p1 == 0) {
    char * tmp_cmd = (char *)line.c_str();
    char * cmd[] = {tmp_cmd, NULL};
    if (execve(tmp_cmd, cmd, NULL) < 0)
      exit(EXIT_FAILURE);
  }
}

int main() {
  std::string line;
  std::cout << "ffosh$ ";
  while (getline(std::cin, line)) {
    if (line == "exit()") {
      std::cout << "break now!!!!" << std::endl;
      break;
    }
    std::cout << line << std::endl;
    process_cmd(line);
    std::cout << "ffosh$ ";
  }
  return (EXIT_SUCCESS);
}
