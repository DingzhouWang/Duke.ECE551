#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

int main(int argc, char ** argv) {
  if (argc == 1) {
    string s;
    vector<string> ans;
    while (getline(cin, s)) {
      ans.push_back(s);
    }
    sort(ans.begin(), ans.end());
    for (auto & s : ans) {
      cout << s << endl;
    }
  }
  else if (argc > 1) {
    for (int i = 1; i < argc; i++) {
      string s;
      vector<string> ans;
      ifstream f(argv[i]);
      if (!f.is_open()) {
        std::cerr << "Can not open file " << argv[i] << std::endl;
        exit(EXIT_FAILURE);
      }
      while (getline(f, s)) {
        ans.push_back(s);
      }
      sort(ans.begin(), ans.end());
      for (auto & s : ans) {
        cout << s << endl;
      }
      f.close();
    }
    return EXIT_SUCCESS;
  }
}
