#include "shell_cmd.hpp"

#include <iostream>
#include <vector>
extern char ** environ;  //learn from environ(7) — Linux manual page

using namespace std;

Shell_cmd::Shell_cmd() {
}
/*
void Shell_cmd::update_map(string input) {
  int start = 0;
  //bool find_var
  for (size_t i = 0; i < input.size(); i++) {
    if (i < input.size() - 1 && input[i] == '$' &&
        (isalnum(input[i + 1]) || input[i + 1] == '_')) {
      start = i + 1;
    }
  }
}
*/
void Shell_cmd::init_map() {
  char ** en_p;  //= environ;
  for (en_p = environ; *en_p != NULL; en_p++) {
    //cout << *en_p << endl;
    string env = *en_p;
    int pos = env.find("=");
    if (pos != -1) {
      string key = env.substr(0, pos);
      string value = env.substr(pos + 1);
      m_[key] = value;
    }
  }
}

void Shell_cmd::print_map(string input) {
  if ((int)input.find("$") == -1)
    return;
  //int start = 0;
  //int end = 0;
  int cnt = 0;
  //bool find_var;
  //string ans = "";
  for (size_t i = 0; i < input.size(); i++) {
    if (input[i] == '$') {
      if (i == input.size() - 1)
        ans = input;
      int rec_pos = i;
      //&& i + 1 < input.size() &&
      //  (isalnum(input[i + 1]) || input[i + 1] == '_')) {
      while (i < input.size() - 1) {
        i++;
        if (isalnum(input[i]) || input[i] == '_') {
          cnt++;
        }
        else {
          break;
        }
      }
      string tmp_key = input.substr(rec_pos + 1, cnt);
      cout << tmp_key << "!" << endl;
      input.erase(rec_pos, cnt + 1);
      input.insert(rec_pos, m_[tmp_key]);
      i = rec_pos;
    }
  }
  ans = input;
  // for (auto kv : m_) {
  //  cout << "key: " << kv.first << endl;
  //  cout << "value: " << kv.second << endl;
  //}
  cout << ans << endl;
}
