#include "simple_vector_2.h"
#include "test_runner.h"

#include <algorithm>
#include <string>
#include <utility>
#include <vector>

using namespace std;

void TestConstruction() {
  SimpleVector<int> empty;
  ASSERT_EQUAL(empty.Size(), 0U);
  ASSERT_EQUAL(empty.Capacity(), 0U);
  ASSERT(empty.begin() == empty.end());

  SimpleVector<string> five_strings(5);
  ASSERT_EQUAL(five_strings.Size(), 5U);
  ASSERT(five_strings.Size() <= five_strings.Capacity());
  for (auto& item : five_strings) {
    ASSERT(item.empty());
  }
  five_strings[2] = "Hello";
  ASSERT_EQUAL(five_strings[2], "Hello");
}

void TestPushBack() {
  SimpleVector<int> simple_vector;
  for (int i = 10; i >= 1; --i) {
    simple_vector.PushBack(i);
    ASSERT(simple_vector.Size() <= simple_vector.Capacity());
  }
  sort(begin(simple_vector), end(simple_vector));

  const vector<int> expected = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  ASSERT(equal(begin(simple_vector), end(simple_vector), begin(expected)));
}

class StringNonCopyable : public string {
public:
  using string::string;
  explicit StringNonCopyable(string &&other) : string(std::move(other)) {}
  StringNonCopyable(const StringNonCopyable&) = delete;
  StringNonCopyable(StringNonCopyable&&) = default;
  StringNonCopyable& operator=(const StringNonCopyable&) = delete;
  StringNonCopyable& operator=(StringNonCopyable&&) = default;
};

void TestNoCopy() {
  SimpleVector<StringNonCopyable> strs;
  static const int SIZE = 10;
  for (int i = 0; i < SIZE; ++i) {
    strs.PushBack(StringNonCopyable(to_string(i)));
  }
  for (int i = 0; i < SIZE; ++i) {
    ASSERT_EQUAL(strs[i], to_string(i));
  }
}

int main() {
  TestRunner trunner;
  RUN_TEST(trunner, TestConstruction);
  RUN_TEST(trunner, TestPushBack);
  RUN_TEST(trunner, TestNoCopy);
  return 0;
}
