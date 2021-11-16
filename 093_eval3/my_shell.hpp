#include <stdlib.h>

#include <iostream>
#include <string>

class shell {
 private:
  char * path;

 public:
  shell();
  ~shell();
  bool init_envpath();
  std::string simplifyPath(std::string path);
};
