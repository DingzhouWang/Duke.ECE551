#include "my_shell.hpp"

#include <fcntl.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

//this function is used to init the environment path
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
  while (getline(p, tmp, ':')) {     //filter the string by ":"
    char * t = strdup(tmp.c_str());  //remember to free it
    env_paths.push_back(t);
  }
}

//set to new path, split by :
void Shell::process_cmd(const std::string & line) {
  init_envpath();
  path = getenv("ECE551PATH");
  if (line.find('/') == std::string::npos) {
    //std::cout << "process cmd" << std::endl;
    set_path(path);
  }
}

//because we use strdup, so we have to free it
void Shell::free_path() {
  for (size_t i = 0; i < env_paths.size(); i++) {
    free(env_paths[i]);
  }
}

//the mainly execute function, which handle arguments like
// set, rev, export, env, ls, cd, etc.
void Shell::execute(const std::string & line) {
  //first we need to make input format
  parse_input(line);
  string line_;
  //if the input is "cd"
  if (argument_p[0] == "cd") {
    new_cmd();
    std::vector<std::string>().swap(argument_p);
  }
  else if (argument_p[0] == "set") {  //if the input is "set"
    if (argument_p.size() < 3) {
      cout << "please use: set var val!" << endl;
    }
    else {
      bool valid = true;
      for (size_t i = 0; i < argument_p[1].size(); i++) {
        if (argument_p[1][i] != '_' && isalnum(argument_p[1][i]) == 0) {
          valid = false;
          cout << "Invalid Key Name!" << endl;
          break;
        }
      }
      if (valid) {
        ParseLine(line);            //parse set, var, val
        My_Cmd.set_val(parseline);  //set to the map
      }
      valid = true;
    }
    std::vector<std::string>().swap(argument_p);
    std::vector<std::string>().swap(parseline);
  }
  else if (argument_p[0] == "export") {  //if the input is export
    if (argument_p.size() != 2) {
      cout << "please use: export var!" << endl;
    }
    else {  //add variable to environment
      My_Cmd.export_val(argument_p);
    }
    std::vector<std::string>().swap(argument_p);
  }
  else if (argument_p[0] == "rev") {  //&& argument_p.size() == 2
    if (argument_p.size() != 2) {
      cout << "please use: rev var!" << endl;
    }
    else {  // reverse the value
      My_Cmd.rev_val(argument_p);
    }
    std::vector<std::string>().swap(argument_p);
  }
  else if (argument_p[0] == "env") {  //if the input is "env"
    //&& argument_p.size() == 1
    if (argument_p.size() != 1) {
      cout << "please use: env!" << endl;
    }
    else {
      My_Cmd.print_env();  //print all the env variables
    }
    std::vector<std::string>().swap(argument_p);
  }
  else {
    if (line.find("$") != string::npos) {
      line_ = My_Cmd.print_map(line);
      if (line_.find(" ") != string::npos) {
        cout << line_ << endl;
        return;
      }
    }
    else {
      line_ = line;
    }
    std::vector<std::string>().swap(argument_p);
    pid_t p1 = -1;
    p1 = fork();
    if (p1 < 0) {
      std::cerr << "cannot fork!" << std::endl;
      exit(EXIT_FAILURE);
    }
    else if (p1 > 0) {  //parent process
      int status;
      //learn from linux man page
      pid_t wpid = waitpid(p1, &status, WUNTRACED);
      if (wpid == -1) {
        std::cerr << "waitpid error!" << std::endl;
        exit(EXIT_FAILURE);
      }
      if (WIFEXITED(status)) {
        if (WEXITSTATUS(status) == 0) {
          std::cout << "Program was successful" << std::endl;
        }
        else
          std::cout << "Program failed with code " << WEXITSTATUS(status) << std::endl;
      }
      if (WIFSIGNALED(status)) {
        std::cout << "Terminated by signal " << WTERMSIG(status) << std::endl;
      }
    }
    else if (p1 == 0) {  //child process
      //std::cout << "child process in" << std::endl;
      bool find_cmd = false;
      string l_ = redirect_error(line_);
      l_ = redirect_output(l_);
      l_ = redirect_input(l_);
      process_cmd(l_);  //init env_path
      parse_input(l_);  //init argument_p
      //std::cout << "1: " << argument_p[0] << std::endl;
      for (size_t i = 0; i < output_file.size(); i++) {
        int fd_out = open(output_file[i].c_str(),
                          O_CREAT | O_RDWR,
                          S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
        if (fd_out > 0) {
          // redirect STDOUT for this process
          dup2(fd_out, 1);
          close(fd_out);
        }
        else {
          cout << "error fd_out!" << endl;
        }
      }
      for (size_t j = 0; j < input_file.size(); j++) {
        int fd_in = open(input_file[j].c_str(), O_RDONLY);
        if (fd_in > 0) {
          // redirect STDIN for this process
          dup2(fd_in, 0);
          close(fd_in);
        }
        else {
          cout << "error fd_in!" << endl;
        }
      }
      for (size_t k = 0; k < error_file.size(); k++) {
        int fd_er = open(error_file[k].c_str(),
                         O_CREAT | O_RDWR,
                         S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
        if (fd_er > 0) {
          // redirect STDERR for this process
          dup2(fd_er, 2);
          close(fd_er);
        }
        else {
          cout << "error fd_er!" << endl;
        }
      }
      //if the input commend include '/'
      if (argument_p[0].find('/') != std::string::npos) {
        char * tmp_cmd = (char *)argument_p[0].c_str();
        //char * cmd[] = {tmp_cmd, NULL};
        char ** cmd = new char *[argument_p.size() + 1];
        for (size_t i = 0; i < argument_p.size(); i++) {
          cmd[i] = (char *)argument_p[i].c_str();
          //std::cout << cmd[i] << std::endl;
        }
        cmd[argument_p.size()] = NULL;
        if (execve(tmp_cmd, cmd, NULL) < 0) {
          cout << "Command " << argument_p[0] << " not found" << endl;
          exit(EXIT_FAILURE);
        }
        delete[] cmd;
      }
      else {  //if the input commend exclude '/'
        for (size_t i = 0; i < env_paths.size(); i++) {
          std::string tmp_env_path = env_paths[i];
          //here we need update the path
          std::string tmp_cmd = tmp_env_path + "/" + argument_p[0];
          //std::cout << tmp_cmd << std::endl;
          char ** cmd = new char *[argument_p.size() + 1];
          for (size_t i = 0; i < argument_p.size(); i++) {
            cmd[i] = (char *)argument_p[i].c_str();
            //cout << cmd[i] << endl;
          }
          cmd[argument_p.size()] = NULL;
          int exe_num = execve(tmp_cmd.c_str(), cmd, NULL);
          //cout << exe_num << endl;
          if (exe_num != 0) {
            find_cmd = find_cmd || false;
            //cout << tmp_cmd << ":" << exe_num << endl;
          }
          else {
            find_cmd = true;
            //cout << tmp_cmd << ":" << exe_num << endl;
            exit(EXIT_SUCCESS);
          }
          delete[] cmd;
        }
        //cout << "11111" << endl;
        if (!find_cmd) {
          std::cout << "Command " << argument_p[0] << " not found" << std::endl;
          exit(EXIT_FAILURE);
        }
      }
      free_path();
    }
  }
}

//parse the input to meet the format
//include to match '"' '\'
void Shell::parse_input(const std::string & input) {
  //std::cout << input << std::endl;
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
      else if (input[i] == ' ' && find_arg) {
        std::string tmp_arg = input.substr(start, i - start);
        //std::cout << "tmp_arg: " << tmp_arg << std::endl;
        argument_p.push_back(tmp_arg);
        find_arg = false;
      }
      else {
      }
      if (i == input.size() - 1 && find_arg && input[i] != ' ') {
        std::string tmp_arg = input.substr(start, i - start + 1);
        //std::cout << "tmp_arg: " << tmp_arg << std::endl;
        argument_p.push_back(tmp_arg);
        find_arg = false;
      }
    }
  }
  else if (input.find('"') != std::string::npos) {
    for (size_t i = 0; i < input.size(); i++) {
      if (input[i] == ' ' && !find_arg && !find_quo) {
        continue;
      }
      else if (input[i] == '"' && !find_quo && !find_arg) {
        find_quo = true;
        find_arg = true;
        start = i + 1;
      }
      else if (input[i] == '"' && find_quo) {
        if (i > 0 && input[i - 1] == '\\') {
          continue;
        }
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
      //std::cerr << "unclosed quotation mark!" << std::endl;
      std::cout << "unclosed quotation mark!" << std::endl;
    }
    //std::cout << argument_p.size() << std::endl;
  }
  simplify();
  //std::cout << argument_p[0] << std::endl;
}

//simplify the elements in argument_p
//remove "\\" and "\""
void Shell::simplify() {
  for (auto & s_ : argument_p) {
    //std::cout << s_ << std::endl;
    for (size_t i = 0; i < s_.size(); i++) {
      if (s_[i] == '\\') {
        if (i + 1 < s_.size() && (s_[i + 1] == '\\' || s_[i + 1] == '"')) {
          s_.erase(i, 1);
        }
      }
    }
    //argument_p.push_back(s_);
  }
}

//cd commend
void Shell::new_cmd() {
  int cd_num = 0;
  if (argument_p.size() == 1) {
    std::string path_ = "/home";
    cd_num = chdir(path_.c_str());
  }
  else if (argument_p.size() == 2) {
    //std::cout << argument_p[1] << std::endl;
    cd_num = chdir(argument_p[1].c_str());
  }
  else {
    std::cerr << "ERROR INPUT!" << std::endl;
  }
  if (cd_num < 0) {
    std::cerr << "No such file or dictionary!" << std::endl;
  }

  return;
}

//ParseLine is used to parse the "set var value" commend
void Shell::ParseLine(const std::string & line) {
  int pos1 = line.find(" ");
  string cmd = line.substr(0, pos1);
  parseline.push_back(cmd);
  int pos2 = line.substr(pos1 + 1).find(" ");
  string var = line.substr(pos1 + 1, pos2);
  parseline.push_back(var);
  string value = line.substr(pos1 + pos2 + 2);
  parseline.push_back(value);
}

//redirect output
string Shell::redirect_output(std::string l) {
  while (l.find('>') != string::npos) {
    int i = l.find('>');
    int j = i + 1;

    while (j <= (int)l.size() - 1 && l[j] != '>' && l[j] != '<') {
      j++;
    }
    string str = l.substr(i + 1, j - i - 1);
    str.erase(std::remove(str.begin(), str.end(), ' '), str.end());
    if (!str.empty()) {
      output_file.push_back(str);
    }
    else {
      cerr << "error redirect output!" << endl;
    }
    l.erase(i, j - i);
  }
  return l;
}

//redirect input
string Shell::redirect_input(std::string l) {
  while (l.find('<') != string::npos) {
    int i = l.find('<');
    int j = i + 1;

    while (j <= (int)l.size() - 1 && l[j] != '<' && l[j] != '>') {
      j++;
    }
    string str = l.substr(i + 1, j - i - 1);
    str.erase(std::remove(str.begin(), str.end(), ' '), str.end());
    if (!str.empty()) {
      input_file.push_back(str);
    }
    else {
      cerr << "error redirect input!" << endl;
    }
    l.erase(i, j - i);
  }
  return l;
}

//redirect error
string Shell::redirect_error(std::string l) {
  if (l.find("2>") != string::npos) {
    int i = l.find("2>");
    int j = i + 1;

    while (j <= (int)l.size() - 1) {
      j++;
    }
    string str = l.substr(i + 2, j - i - 1);
    str.erase(std::remove(str.begin(), str.end(), ' '), str.end());
    if (!str.empty()) {
      error_file.push_back(str);
    }
    else {
      cerr << "error redirect error!" << endl;
    }
    l.erase(i, j - i);
  }
  return l;
}

void Shell::go_exe(string line) {
  //if only have one command
  if (line.find('|') == string::npos) {
    execute(line);
    return;
  }
  //if we have many command, store it in pipe_v
  std::stringstream p_v(line);
  string cmd_p;
  while (getline(p_v, cmd_p, '|')) {
    pipe_v.push_back(cmd_p);
  }
  /*
  int s_ = pipe_v.size() * 2;
  int pipes[s_];
  auto i = pipes;
  int status;
  while (i < pipes + s_) {
    pipe(i);
    i = i + 2;
  }
  exe_pipe(0);
  for (auto j = 0; j < (int)pipe_v.size(); j++) {
    wait(&status);
  }
  */
  int fd[2];
  int pid1, pid2;
  int status;
  pipe(fd);
  if ((pid1 = fork()) < 0) {
    cout << "fork error" << endl;
  }
  else if (pid1 == 0) {
    dup2(fd[1], STDOUT_FILENO);
    dup2(fd[1], STDERR_FILENO);
    close(fd[0]);
    close(fd[1]);
    execute(pipe_v[0]);
    exit(EXIT_SUCCESS);
  }
  if ((pid2 = fork()) < 0) {
    cout << "fork error" << endl;
  }
  else if (pid2 == 0) {
    dup2(fd[0], STDIN_FILENO);
    close(fd[0]);
    close(fd[1]);
    execute(pipe_v[1]);
    exit(EXIT_SUCCESS);
  }
  close(fd[0]);
  close(fd[1]);
  for (int i = 0; i < 2; i++) {
    wait(&status);
  }
  return;
}

/*
void Shell::exe_pipe() {
  //do each command
  //cout << "pipe_v: " << pipe_v.size() << endl;

  int fd[2];
  if (pipe(fd) == -1) {
    cerr << "We got error PIPE!" << endl;
    exit(EXIT_FAILURE);
  }
  pid_t pid = fork();
  if (pid == -1) {
    std::cerr << "cannot fork!" << std::endl;
    exit(EXIT_FAILURE);
  }
  else if (pid == 0) {  //child process
    cout << "in child(pipe)" << endl;
    close(fd[0]);
    int tmp = dup2(fd[1], STDOUT_FILENO);  //std_out redirect to fd[1]
    //close(fd[1]);
    cout << "tmp:" << tmp << endl;
    string cmd = pipe_v.front();
    cout << cmd << endl;
    pipe_v.pop();
    execute(cmd);
    //do we need to exit?
    cout << "exit 1" << endl;
  }
  else if (pid > 0) {
    //cout << "parent" << endl;
    int status;
    cout << "1111" << endl;
    pid_t wpid = waitpid(pid, &status, 0);
    cout << wpid << endl;
    if (WEXITSTATUS(status)) {  //child process exit success
      cout << "status" << endl;
      if (pipe_v.size()) {
        cout << "pipe_v size: " << pipe_v.size() << endl;
        close(fd[1]);
        dup2(fd[0], STDIN_FILENO);  //std_in redirect tp fd[0]
        close(fd[0]);
        exe_pipe();
      }
    }
    else {
      cout << "child exit failed!" << endl;
      cerr << "child exit failed!" << endl;
    }
  }
}
*/
