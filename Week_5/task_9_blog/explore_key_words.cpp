#include "profile.h"
#include "test_runner.h"

#include <cstddef>
#include <cstdlib>
#include <functional>
#include <future>
#include <istream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <string_view>
#include <utility>
#include <vector>
using namespace std;

struct Stats
{
  map<string, int> word_frequences;

  void operator+=(const Stats &other);
};

void Stats::operator+=(const Stats &other)
{
  for (const auto &[key, value] : other.word_frequences) {
    word_frequences[key] += value;
  }
}

Stats ExploreLineMy(const set<string> &key_words, const string &line)
{
  Stats result;
  string_view sv = line;
  string word;
  while (true) {
    size_t const space_pos = sv.find(' ');
    word = sv.substr(0, space_pos);
    if (key_words.count(word) != 0) {
      result.word_frequences[std::move(word)] += 1;
    }
    if (space_pos == sv.npos) {
      break;
    } else {
      sv.remove_prefix(space_pos + 1);
    }
  }
  return result;
}

Stats ExploreLine(const set<string> &key_words, const vector<string> &lines)
{
  Stats result;
  vector<string_view> words;
  for (auto &line : lines) {
    string_view line_string_view = line;
    while (true) {
      size_t pos = line_string_view.find(' ');
      words.push_back(line_string_view.substr(0, pos));
      if (pos == line_string_view.npos)
        break;
      else {
        line_string_view.remove_prefix(pos + 1);
        while (line_string_view[0] == ' ')
          line_string_view.remove_prefix(1);
      }
    }
  }
  for (auto &word : words) {
    if (key_words.find(string(word)) != key_words.end())
      ++result.word_frequences[string(word)];
  }
  return result;
}

Stats ExploreGroup(const set<string> &key_words, const vector<string> &group)
{
  Stats result;
  for (const auto &line : group) {
    result += ExploreLineMy(key_words, line);
  }
  return result;
}

Stats ExploreKeyWordsSingleThread(const set<string> &key_words, istream &input)
{
  Stats result;
  for (string line; getline(input, line);) {
    result += ExploreLineMy(key_words, line);
  }
  return result;
}

Stats ExploreKeyWordsMultiThread(const set<string> &key_words, istream &input)
{
  Stats result;
  vector<future<Stats>> futures;

  size_t const size_group = 10000;
  vector<string> group_of_blogs(size_group);

  string line;
  while (getline(input, line)) {
    group_of_blogs.push_back(std::move(line));
    if (group_of_blogs.size() >= size_group) {
      futures.push_back(async(ExploreGroup, ref(key_words), std::move(group_of_blogs)));
      group_of_blogs.reserve(size_group);
    }
  }

  futures.push_back(async(ExploreGroup, ref(key_words), std::move(group_of_blogs)));

  for (auto &f : futures) {
    result += f.get();
  }
  return result;
}

Stats ExploreKeyWordsMy(const set<string> &key_words, istream &input)
{
  // Возвращает структуру Stats, в которой хранится, сколько раз каждое слово из
  // key_words суммарно встретилось в потоке input
  // return ExploreKeyWordsSingleThread(key_words, input);
  return ExploreKeyWordsMultiThread(key_words, input);
}

Stats ExploreKeyWords(const set<string> &key_words, istream &input)
{
  vector<vector<string>> threads_packs(4);
  int i = 0;
  for (string line; getline(input, line);) {
    threads_packs[i++ % 4].push_back(move(line));
  }
  vector<future<Stats>> futures;
  for (auto &str : threads_packs) {
    futures.push_back(async(ExploreLine, ref(key_words), ref(str)));
  }
  Stats result;
  for (auto &f : futures) {
    result += f.get();
  }
  return result;
}

void GenerateBlogs(const vector<string> &words, stringstream &ss, size_t number_of_blogs, size_t lenght_of_blog)
{
  while (number_of_blogs--) {
    size_t cnt = lenght_of_blog;
    while (cnt--) {
      ss << words[rand() % words.size()] << ' ';
    }
    ss << "\n";
  }
}

void TestOperatorStats()
{
  {
    Stats stats;
    Stats expStats;
    map<string, int> const expected = {
      { "a", 1 },
      { "b", 2 },
      { "c", 3 },
      { "d", 4 },
      { "e", 5 },
    };
    expStats.word_frequences = expected;
    stats += expStats;
    ASSERT_EQUAL(stats.word_frequences, expected);
  }
  {
    Stats stats;
    Stats const expStats;
    map<string, int> const word_frequenced = {
      { "a", 2 },
      { "c", 4 },
      { "e", 6 },
      { "f", 8 },
      { "g", 10 },
    };
    map<string, int> const expected = {
      { "a", 4 },
      { "c", 8 },
      { "e", 12 },
      { "f", 16 },
      { "g", 20 },
    };
    stats.word_frequences = word_frequenced;
    stats += stats;
    ASSERT_EQUAL(stats.word_frequences, expected);
  }
  {
    Stats stats;
    Stats const add;
    map<string, int> const word_frequenced = {
      { "a", 2 },
      { "c", 4 },
      { "e", 6 },
      { "f", 8 },
      { "g", 10 },
    };
    stats.word_frequences = word_frequenced;
    stats += add;
    ASSERT_EQUAL(stats.word_frequences, stats.word_frequences);
  }
  {
    Stats stats;
    Stats add;
    Stats const expStats;
    map<string, int> const word_frequenced = {
      { "a", 2 },
      { "c", 4 },
      { "e", 6 },
      { "f", 8 },
      { "g", 10 },
    };
    map<string, int> const add_word_frequenced = {
      { "b", 2 },
      { "d", 4 },
      { "e", 6 },
      { "h", 8 },
    };
    map<string, int> const expected_word_frequenced = {
      { "a", 2 },
      { "b", 2 },
      { "c", 4 },
      { "d", 4 },
      { "e", 12 },
      { "f", 8 },
      { "g", 10 },
      { "h", 8 },
    };
    stats.word_frequences = word_frequenced;
    add.word_frequences = add_word_frequenced;
    stats += add;
    ASSERT_EQUAL(stats.word_frequences, expected_word_frequenced);
  }
}

void TestBasic()
{
  {
    const set<string> key_words = { "yangle", "rocks", "sucks", "all" };

    stringstream ss;
    ss << "this new yangle service really rocks\n";
    ss << "It sucks when yangle isn't available\n";
    ss << "10 reasons why yangle is the best IT company\n";
    ss << "yangle rocks others suck\n";
    ss << "Goondex really sucks, but yangle rocks. Use yangle\n";

    const auto stats = ExploreKeyWordsMy(key_words, ss);
    const map<string, int> expected = {
      { "yangle", 6 },
      { "rocks", 2 },
      { "sucks", 1 }
    };
    ASSERT_EQUAL(stats.word_frequences, expected);
  }
  {
    const set<string> key_words = { "yangle", "rocks", "sucks", "all" };

    stringstream ss;
    ss << "this new yangle service really rocks\n";
    ss << "It  sucks when yangle isn't available\n";
    ss << " 10 reasons why yangle is the best IT company\n";
    ss << "yangle   rocks others suck\n";
    ss << " Goondex   really sucks, but yangle rocks.   Use yangle\n";

    const auto stats = ExploreKeyWordsMy(key_words, ss);
    const map<string, int> expected = {
      { "yangle", 6 },
      { "rocks", 2 },
      { "sucks", 1 }
    };
    ASSERT_EQUAL(stats.word_frequences, expected);
  }
}

void TestSpeed()
{
  stringstream ss;
  vector<string> const words{
    "the",
    "а",
    "her",
    "for",
    "on",
    "at",
    "were",
    "would",
    "like",
    "be",
    "them",
    "about",
    "down",
    "can",
    "know",
    "yangle",
    "over",
    "more",
    "an",
    "other",
    "little",
    "see",
    "here",
    "thing",
    "hand",
    "by",
    "will",
    "way",
    "again",
    "right",
    "rocks",
    "only",
    "eye",
    "well",
    "two",
    "sucks"
  };
  {
    LOG_DURATION("Generate blogs");
    GenerateBlogs(words, ss, 50000, 200);
  }
  Stats const st;
  const set<string> key_words = { "yangle", "rocks", "sucks", "all" };
  {
    LOG_DURATION("Single Thread");
    const auto stats = ExploreKeyWordsSingleThread(key_words, ss);
  }
  stringstream ss1;
  GenerateBlogs(words, ss1, 50000, 200);
  {
    LOG_DURATION("Multi Thread My ExploreKeyWords");
    const auto stats = ExploreKeyWordsMultiThread(key_words, ss1);
  }
  stringstream ss2;
  GenerateBlogs(words, ss2, 50000, 200);
  {
    LOG_DURATION("Multi Thread");
    const auto stats = ExploreKeyWordsMultiThread(key_words, ss2);
  }
}

int main()
{
  TestRunner trunner;
  RUN_TEST(trunner, TestOperatorStats);
  RUN_TEST(trunner, TestBasic);
  RUN_TEST(trunner, TestSpeed);
}
