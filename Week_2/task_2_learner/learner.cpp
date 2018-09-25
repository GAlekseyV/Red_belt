//#include "profile.h"
#include <algorithm>
#include <string>
#include <vector>
#include <set>

//#include <iostream>
//#include <fstream>
//#include <sstream>

using namespace std;

class Learner {
 private:
  set<string> dict;

 public:
  int Learn(const vector<string>& words) {
    size_t old_size = dict.size();
    for (const auto& word : words) {
      dict.insert(word);
    }
    return dict.size() - old_size;
  }

  vector<string> KnownWords() const {
    return {dict.begin(), dict.end()};
  }
};

//int main() {
//    Learner learner;
//    ifstream in("random_words.txt");
//    string line;
//    while (in) {
//        vector<string> words;
//        string word;
//        while (in >> word) {
//            words.push_back(word);
//        }
//        int count_word;
//        {
//            LOG_DURATION("Learn words")
//            count_word = learner.Learn(words);
//        }
//
//        cout << count_word << "\n";
//    }
//    cout << "=== known words ===\n";
//    {
//        LOG_DURATION("Check knownWords")
//        for (auto word : learner.KnownWords()) {
//            //cout << word << "\n";
//        }
//    }
//}
