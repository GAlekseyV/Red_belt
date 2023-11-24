//
// Created by GAlekseyV on 28.09.2018.
//
#include "test_runner.h"
#include <iterator>
#include <set>
#include <string>
#include <vector>

using namespace std;

class StringSet
{
public:
  void Add(const string &s, int priority)
  {
    data.push_back(s);
    sorted_data.insert(StringItem{ s, priority });
  }
  [[nodiscard]] const string &FindLast() const
  {
    return data.back();
  }
  [[nodiscard]] const string &FindBest() const
  {
    return prev(sorted_data.end())->s;
  }

private:
  vector<string> data;

  struct StringItem
  {
    string s;
    int priority;
    bool operator<(const StringItem &other) const
    {
      return priority < other.priority;
    }
  };
  set<StringItem> sorted_data;
};

// Юнит-тесты

void TestSimple()
{
  StringSet strings;
  strings.Add("upper", 10);
  strings.Add("lower", 0);
  ASSERT_EQUAL(strings.FindLast(), "lower");
  ASSERT_EQUAL(strings.FindBest(), "upper");
}

void TestAnother()
{
  StringSet strings;

  strings.Add("apple", 1);
  strings.Add("orange", 3);
  ASSERT_EQUAL(strings.FindLast(), "orange");
  ASSERT_EQUAL(strings.FindBest(), "orange");

  strings.Add("plum", -5);
  ASSERT_EQUAL(strings.FindLast(), "plum");
  ASSERT_EQUAL(strings.FindBest(), "orange");

  strings.Add("lemon", 5);
  ASSERT_EQUAL(strings.FindLast(), "lemon");
  ASSERT_EQUAL(strings.FindBest(), "lemon");
}

int main()
{
  TestRunner trunner;
  RUN_TEST(trunner, TestSimple);
  RUN_TEST(trunner, TestAnother);
  return 0;
}
