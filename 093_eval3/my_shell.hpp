#include <stdlib.h>

#include <iostream>
#include <string>
#include <vector>

class Shell {
 private:
  char * path;
  std::vector<char *> env_paths;
  char ** argv;  //not used
  std::vector<std::string> argument_p;

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
};
