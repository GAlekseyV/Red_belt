#include "profile.h"
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include <fstream>
#include <iostream>

using namespace std;

class Learner
{
private:
  set<string> dict;

public:
  int Learn(const vector<string> &words)
  {
    size_t const old_size = dict.size();
    for (const auto &word : words) {
      dict.insert(word);
    }
    return dict.size() - old_size;
  }

  [[nodiscard]] vector<string> KnownWords() const
  {
    return { dict.begin(), dict.end() };
  }
};

int main()
{
  Learner learner;
  ifstream instr("random_words.txt");
  string const line;
  while (instr) {
    vector<string> words;
    string word;
    while (instr >> word) {
      words.push_back(word);
    }
    int cnt_word{ 0 };
    {
      LOG_DURATION("Learn words")
      cnt_word = learner.Learn(words);
    }

    cout << cnt_word << "\n";
  }
  cout << "=== known words ===\n";
  {
    LOG_DURATION("Check knownWords")
    for (auto word : learner.KnownWords()) {
      // cout << word << "\n";
    }
  }
}
