#include <stdlib.h>

#include <iostream>
#include <string>
#include <vector>

class shell {
 private:
  char * path;
  std::vector<char *> env_paths;
  char ** argv;

 public:
  shell();
  ~shell();
  bool init_envpath();
  std::string simplifyPath(std::string path);
  void process_cmd(const std::string & line);
  void set_path(const char * paths);
  void set_argv(const char * l);
  void execute(const std::string & line);
  void free_argv();
  void free_path();
};
