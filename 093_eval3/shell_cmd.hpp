#include <iostream>
#include <unordered_map>
#include <vector>
using namespace std;
class Shell_cmd {
 private:
  unordered_map<string, string> m_;  //the map is used to save env virables
  string ans;

 public:
  Shell_cmd();
  void init_map();
  void update_map(string input);
  string print_map(string input);
  void set_val(vector<string> argument_p);
  void export_val(vector<string> argument_p);
  void rev_val(vector<string> argument_p);
  void print_env();
  void print_map();
};
