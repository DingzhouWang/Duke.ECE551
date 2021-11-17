#include "my_shell.hpp"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

bool shell::init_envpath() {
  path = getenv("PATH");
  if (setenv("ECE551PATH", path, 1) == 0) {
    return true;
  }
  std::cout << "init_envpath error!" << std::endl;
  std::cerr << "init_envpath error!" << std::endl;
  return false;
}

//save each env_path to the vector
void shell::set_path(const char * paths) {
  std::stringstream p(paths);
  std::string tmp;
  while (getline(p, tmp, ':')) {
    char * t = strdup(tmp.c_str());  //remember to free it
    env_paths.push_back(t);
  }
}

//concat a new path
void shell::process_cmd(const std::string & line) {
  init_envpath();
  path = getenv("ECE551PATH");
  if (line.find('/') == std::string::npos) {
    set_path(path);
  }
}

void shell::free_path() {
  for (size_t i = 0; i < env_paths.size(); i++) {
    free(env_paths[i]);
  }
}

//set argv[]
void shell::set_argv(const char * l) {  //also have to remember free
  int size_ = env_paths.size() + 2;
  argv = new char *[size_];
  argv[0] = strdup(l);
  for (int i = 1; i < size_ - 1; i++) {
    argv[i] = strdup(env_paths[i]);
  }
  argv[size_ - 1] = NULL;
}

void shell::free_argv() {
  for (size_t i = 0; i < env_paths.size(); i++) {
    free(argv[i]);
  }
  delete[] argv;
}

void shell::execute(const std::string & line) {
  pid_t p1 = -1;
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
    bool find_cmd = false;
    process_cmd(line);
    set_argv(line.c_str());
    for (size_t i = 0; i < env_paths.size(); i++) {
      //char * tmp_cmd = (char *)line.c_str();
      char * tmp_cmd = env_paths[i];
      //char * cmd[] = {tmp_cmd, NULL};
      int exe_num = execve(tmp_cmd, argv, NULL);
      if (exe_num != 0) {
        find_cmd = find_cmd || false;
      }
      else {
        find_cmd = true;
        exit(EXIT_SUCCESS);
      }
    }
    if (!find_cmd) {
      std::cout << "Command " << line << " not found" << std::endl;
      exit(EXIT_FAILURE);
    }
    free_argv();
    free_path();
  }
}
/*
//parse
void parse_input(std::string input) {
  std::string res;
  size_t i = 0;
  if(!input.find("\"")){
    while(input>>res){
      vec.push_back(res);
    }
    exit(EXIT_SUCCESS)
  }else{
    bool find_co = false;
    bool find_slash = false;
    int start = 0, end = 0;
    while (i < input.size()) {
      if (input[i] == ' ' && !find_co) //space
        continue;
      else if (input[i] == '"' && input[i+1]!='\' && !find_co){
        start = i+1;
      }else if(input[i] == '"' && find_co){
        res = input.substr(start, i-start);
	vec.push_back(res);
      }
    }
    if(find_co){
      std::cout<<"invalid argv input!"<<std::endl;
    }
  }
}

*/

//simplify path
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
