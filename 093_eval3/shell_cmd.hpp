#include <iostream>
#include <unordered_map>
using namespace std;
class Shell_cmd {
 private:
  unordered_map<string, string> m_;
  string ans;

 public:
  Shell_cmd();
  void init_map();
  void update_map(string input);
  void print_map(string input);
};
