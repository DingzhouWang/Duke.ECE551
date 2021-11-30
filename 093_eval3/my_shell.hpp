#include <stdlib.h>

#include <iostream>
#include <string>
#include <vector>

#include "shell_cmd.hpp"

//using namespace std;
class Shell {
 private:
  char * path;
  std::vector<char *> env_paths;
  std::vector<std::string> parseline;
  std::vector<std::string> argument_p;
  Shell_cmd My_Cmd;
  std::vector<std::string> output_file;
  std::vector<std::string> input_file;
  std::vector<std::string> error_file;

 public:
  Shell(){};
  ~Shell(){};
  bool init_envpath();
  std::string simplifyPath(std::string path);
  void process_cmd(const std::string & line);
  void set_path(const char * paths);
  void set_argv();
  void execute(const std::string & line);
  void free_argv();
  void free_path();
  void parse_input(const std::string & input);
  void simplify();
  void new_cmd();
  void ParseLine(const std::string & line);
  string redirect_output(std::string line);
  string redirect_input(std::string line);
  string redirect_error(std::string line);
};
