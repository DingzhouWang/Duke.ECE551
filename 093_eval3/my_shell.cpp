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

bool Shell::init_envpath() {
  path = getenv("PATH");
  if (setenv("ECE551PATH", path, 1) == 0) {
    return true;
  }
  std::cout << "init_envpath error!" << std::endl;
  std::cerr << "init_envpath error!" << std::endl;
  return false;
}

//save each env_path to the vector
void Shell::set_path(const char * paths) {
  std::stringstream p(paths);
  std::string tmp;
  while (getline(p, tmp, ':')) {
    //std::cout << "set path " << tmp << std::endl;
    char * t = strdup(tmp.c_str());  //remember to free it
    env_paths.push_back(t);
  }
  //std::cout << env_paths[0] << " " << env_paths[1] << std::endl;
}

//set to new path
void Shell::process_cmd(const std::string & line) {
  init_envpath();
  path = getenv("ECE551PATH");
  if (line.find('/') == std::string::npos) {
    //std::cout << "process cmd" << std::endl;
    set_path(path);
  }
}

void Shell::free_path() {
  for (size_t i = 0; i < env_paths.size(); i++) {
    free(env_paths[i]);
  }
}

//set argv[] not used!!!!!!!
void Shell::set_argv() {  //also have to remember free
  int size_ = argument_p.size() + 1;
  argv = new char *[size_];
  //argv[0] = (char *)argument_p[0].c_str();
  for (int i = 0; i < size_ - 1; i++) {
    argv[i] = (char *)argument_p[i].c_str();
  }
  argv[size_ - 1] = NULL;
}

void Shell::free_argv() {
  for (size_t i = 0; i < env_paths.size(); i++) {
    //   free(argument_p[i]);
  }
  delete[] argv;
}

void Shell::execute(const std::string & line) {
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
    //std::cout << "child process in" << std::endl;
    bool find_cmd = false;
    process_cmd(line);  //init env_path
    parse_input(line);  //init argument_p
    //std::cout << argument_p[0] << std::endl;
    //std::cout << argument_p.size() << std::endl;
    if (argument_p[0].find('/') != std::string::npos) {
      char * tmp_cmd = (char *)argument_p[0].c_str();
      //char * cmd[] = {tmp_cmd, NULL};
      char ** cmd = new char *[argument_p.size()];
      for (size_t i = 0; i < argument_p.size(); i++) {
        cmd[i] = (char *)argument_p[i].c_str();
        //std::cout << cmd[i] << std::endl;
      }
      if (execve(tmp_cmd, cmd, NULL) < 0)
        exit(EXIT_FAILURE);
    }
    else {
      //std::cout << "child P11" << std::endl;
      for (size_t i = 0; i < env_paths.size(); i++) {
        //char * tmp_cmd = (char *)line.c_str();
        std::string tmp_env_path = env_paths[i];
        std::string tmp_cmd = tmp_env_path + "/" + argument_p[0];
        //std::cout << tmp_cmd << std::endl;
        char ** cmd = new char *[argument_p.size()];
        for (size_t i = 0; i < argument_p.size(); i++) {
          cmd[i] = (char *)argument_p[i].c_str();
          //std::cout << cmd[i] << std::endl;
        }
        int exe_num = execve(tmp_cmd.c_str(), cmd, NULL);
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
      free_path();
    }
  }
}

//parse
void Shell::parse_input(const std::string & input) {
  //std::cout << "parse input" << std::endl;
  std::string res;
  bool find_arg = false;
  bool find_quo = false;
  //bool find_slash = false;
  int start = 0;
  if (input.find('"') == std::string::npos) {
    for (size_t i = 0; i < input.size(); i++) {
      if (input[i] == ' ' && !find_arg)
        continue;
      else if (input[i] != ' ' && !find_arg) {
        //std::cout << "index: " << i << std::endl;
        start = i;
        find_arg = true;
      }
      else if (i == input.size() - 1 && find_arg && input[i] != ' ') {
        std::string tmp_arg = input.substr(start, i - start + 1);
        //std::cout << "tmp_arg: " << tmp_arg << std::endl;
        argument_p.push_back(tmp_arg);
        find_arg = false;
      }
      else if (input[i] == ' ' && find_arg) {
        std::string tmp_arg = input.substr(start, i - start);
        //std::cout << "tmp_arg: " << tmp_arg << std::endl;
        argument_p.push_back(tmp_arg);
        find_arg = false;
      }
      else {
        continue;
      }
    }
  }
  else if (input.find('"') != std::string::npos) {
    for (size_t i = 0; i < input.size(); i++) {
      if (input[i] == ' ' && !find_arg && !find_quo) {
        continue;
      }
      else if (input[i] == '"' && !find_quo) {
        find_quo = true;
        find_arg = true;
        //if (input[i + 1] != '\\')
        start = i + 1;
        // else {
        //if (input[i + 2] == '"' || input[i + 2] == '\\') {
        //  start = i + 2;
        //  i = i + 2;
        // }
        //}
      }
      else if (input[i] == '"' && find_quo) {
        std::string tmp_arg = input.substr(start, i - start);
        argument_p.push_back(tmp_arg);
        find_quo = false;
        find_arg = false;
      }
      else if (input[i] != '"' && !find_arg && !find_quo) {
        start = i;
        find_arg = true;
      }
      else if (i == input.size() - 1 && find_arg && input[i] != ' ') {
        std::string tmp_arg = input.substr(start, i - start + 1);
        //std::cout << "tmp_arg: " << tmp_arg << std::endl;
        argument_p.push_back(tmp_arg);
        find_arg = false;
      }
      else if (input[i] == ' ' && find_arg) {
        std::string tmp_arg = input.substr(start, i - start);
        argument_p.push_back(tmp_arg);
        find_arg = false;
      }
    }
    if (find_quo) {
      std::cerr << "unclosed quotation mark!" << std::endl;
      std::cout << "unclosed quotation mark!" << std::endl;
    }
    std::cout << argument_p.size() << std::endl;
  }
  //simplify();
}
/*
void Shell::simplify() {
  std::vector<std::string> tmp_vec(argument_p.begin(), argument_p.end());
  argument_p.clear();
  for (auto & s : tmp_vec) {
    size_t tmp = s.size();
    for (size_t i = 0; i < tmp; i++) {
      if (s[i] == '\\') {
        if (i + 1 < s.size() && (s[i + 1] == '\\' || s[i + 1] == '"')) {
          s.erase(i, 1);
          tmp--;
        }
      }
    }
    argument_p.push_back(s);
  }
}
*/
//simplify path
std::string Shell::simplifyPath(std::string path) {
  /*
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
  */
  return "";
}
