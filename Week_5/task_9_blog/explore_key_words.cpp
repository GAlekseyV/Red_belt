#include "test_runner.h"
#include "profile.h"

#include <future>
#include <map>
#include <string>
using namespace std;

struct Stats {
  map<string, int> word_frequences;

  void operator += (const Stats& other);
};

void Stats::operator+=(const Stats& other){
    for(const auto& [key, value] : other.word_frequences){
        if(word_frequences.count(key) > 0){
            word_frequences.at(key) += value;
        }else{
            word_frequences[key] = value;
        }
    }
}

Stats ExploreLine(const set<string>& key_words, const string& line) {
    Stats result;
    string_view str = line;
    string word;
    while(true){
        size_t space = str.find(' ');
        word = str.substr(0, space);
        if(key_words.count(word) != 0){
            result.word_frequences[word] += 1;
        }
        if(space == str.npos){
            break;
        }else{
            str.remove_prefix(space + 1);
        }
    }
    return result;
}

Stats ExploreKeyWordsSingleThread(const set<string>& key_words, istream& input) {
  Stats result;
  for (string line; getline(input, line); ) {
    result += ExploreLine(key_words, line);
  }
  return result;
}

Stats ExploreKeyWordsMultiThread(const set<string>& key_words, istream& input) {
    Stats result;
    vector<future<Stats>> futures;
    vector<Stats> debugStats;
    for (string line; getline(input, line); ) {
        futures.push_back(async(ExploreLine, ref(key_words), line));
    }
    for(auto& f : futures){
        result += f.get();
    }
    return result;
}

Stats ExploreKeyWords(const set<string>& key_words, istream& input) {
  // Возвращает структуру Stats, в которой хранится, сколько раз каждое слово из
  // key_words суммарно встретилось в потоке input
  //return ExploreKeyWordsSingleThread(key_words, input);
  return ExploreKeyWordsMultiThread(key_words, input);
}

void TestOperatorStats(){
    {
        Stats stats, expStats;
        map<string, int> expected = {
                {"a", 1},
                {"b", 2},
                {"c", 3},
                {"d", 4},
                {"e", 5},
        };
        expStats.word_frequences = expected;
        stats += expStats;
        ASSERT_EQUAL(stats.word_frequences, expected);
    }
    {
        Stats stats, expStats;
        map<string, int> word_frequenced = {
                {"a", 2},
                {"c", 4},
                {"e", 6},
                {"f", 8},
                {"g", 10},
        };
        map<string, int> expected = {
                {"a", 4},
                {"c", 8},
                {"e", 12},
                {"f", 16},
                {"g", 20},
        };
        stats.word_frequences = word_frequenced;
        stats += stats;
        ASSERT_EQUAL(stats.word_frequences, expected);
    }
    {
        Stats stats, add;
        map<string, int> word_frequenced = {
                {"a", 2},
                {"c", 4},
                {"e", 6},
                {"f", 8},
                {"g", 10},
        };
        stats.word_frequences = word_frequenced;
        stats += add;
        ASSERT_EQUAL(stats.word_frequences, stats.word_frequences);
    }
    {
        Stats stats, add, expStats;
        map<string, int> word_frequenced = {
                {"a", 2},
                {"c", 4},
                {"e", 6},
                {"f", 8},
                {"g", 10},
        };
        map<string, int> add_word_frequenced = {
                {"b", 2},
                {"d", 4},
                {"e", 6},
                {"h", 8},
        };
        map<string, int> expected_word_frequenced = {
                {"a", 2},
                {"b", 2},
                {"c", 4},
                {"d", 4},
                {"e", 12},
                {"f", 8},
                {"g", 10},
                {"h", 8},
        };
        stats.word_frequences = word_frequenced;
        add.word_frequences = add_word_frequenced;
        stats += add;
        ASSERT_EQUAL(stats.word_frequences, expected_word_frequenced);
    }

}

void TestBasic() {
    {
        const set<string> key_words = {"yangle", "rocks", "sucks", "all"};

        stringstream ss;
        ss << "this new yangle service really rocks\n";
        ss << "It sucks when yangle isn't available\n";
        ss << "10 reasons why yangle is the best IT company\n";
        ss << "yangle rocks others suck\n";
        ss << "Goondex really sucks, but yangle rocks. Use yangle\n";

        const auto stats = ExploreKeyWords(key_words, ss);
        const map<string, int> expected = {
                {"yangle", 6},
                {"rocks", 2},
                {"sucks", 1}
        };
        ASSERT_EQUAL(stats.word_frequences, expected);
    }
    {
        const set<string> key_words = {"yangle", "rocks", "sucks", "all"};

        stringstream ss;
        ss << "this new yangle service really rocks\n";
        ss << "It  sucks when yangle isn't available\n";
        ss << " 10 reasons why yangle is the best IT company\n";
        ss << "yangle   rocks others suck\n";
        ss << " Goondex   really sucks, but yangle rocks.   Use yangle\n";

        const auto stats = ExploreKeyWords(key_words, ss);
        const map<string, int> expected = {
                {"yangle", 6},
                {"rocks", 2},
                {"sucks", 1}
        };
        ASSERT_EQUAL(stats.word_frequences, expected);
    }
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestOperatorStats);
  RUN_TEST(tr, TestBasic);
}
