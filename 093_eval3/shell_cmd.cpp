#include "shell_cmd.hpp"

#include <algorithm>
#include <iostream>
#include <vector>
extern char ** environ;  //learn from environ(7) — Linux manual page

using namespace std;

//use constructor to init the map
Shell_cmd::Shell_cmd() {
  init_map();
}

//function to init the map
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

//if we have set the virables, we can replace the word behind the $ by value that we set
string Shell_cmd::print_map(string input) {
  if ((int)input.find("$") == -1)
    return input;
  int cnt = 0;
  for (size_t i = 0; i < input.size(); i++) {
    if (input[i] == '$') {
      //cout << "head: " << input << endl;
      if (i == input.size() - 1) {
        cout << input << endl;
        ans = input;
        return ans;
      }
      int rec_pos = i;
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
      //cout << tmp_key << "!" << endl;
      input.erase(rec_pos, cnt + 1);
      //cout << input << endl;
      input.insert(rec_pos, m_[tmp_key]);
      //cout << "insert" << input << endl;
      //cout << input[i] << " " << i << endl;
      i = rec_pos;
      //cout << input[i] << " " << i << endl;
      cnt = 0;
    }
  }
  ans = input;
  //cout << ans << endl;
  return ans;
}

//function to set the value (add to the map)
void Shell_cmd::set_val(vector<string> argument_p) {
  for (size_t i = 0; i < argument_p[1].size(); i++) {
    if (isalnum(argument_p[1][i]) || argument_p[1][i] == '_') {
      continue;
    }
    else {
      cerr << "invalid key" << endl;
    }
  }
  m_[argument_p[1]] = argument_p[2];
  //print_map();
}

//function to export the value
void Shell_cmd::export_val(vector<string> argument_p) {
  if (m_.find(argument_p[1]) == m_.end()) {
    cout << "this key haven't been set!" << endl;
  }
  else {
    setenv(argument_p[1].c_str(), m_[argument_p[1]].c_str(), 1);
  }
}

//function to reverse the value
void Shell_cmd::rev_val(vector<string> argument_p) {
  if (m_.find(argument_p[1]) == m_.end()) {
    cout << "this key haven't been set!" << endl;
  }
  else {
    string tmp_val = m_[argument_p[1]];
    reverse(tmp_val.begin(), tmp_val.end());
    //cout << tmp_val << endl;
    m_[argument_p[1]] = tmp_val;
  }
}

//function to print the environment virables
void Shell_cmd::print_env() {
  char ** en_p;
  for (en_p = environ; *en_p != NULL; en_p++) {
    cout << *en_p << endl;
  }
}

//print key-value used for test
void Shell_cmd::print_map() {
  for (auto kv : m_) {
    cout << "key:" << kv.first << endl;
    cout << "value:" << kv.second << endl;
  }
}
