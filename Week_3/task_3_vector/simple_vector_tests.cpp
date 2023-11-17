#include "simple_vector.h"
#include "test_runner.h"

#include <algorithm>
#include <cstddef>
#include <string>
#include <vector>
using namespace std;

void TestConstruction()
{
  SimpleVector<int> empty;
  ASSERT_EQUAL(empty.Size(), 0U);
  ASSERT_EQUAL(empty.Capacity(), 0U);
  ASSERT(empty.begin() == empty.end());

  size_t const str_cnt = 5;
  SimpleVector<string> five_strings(str_cnt);
  ASSERT_EQUAL(five_strings.Size(), str_cnt);
  ASSERT(five_strings.Size() <= five_strings.Capacity());
  for (auto &item : five_strings) {
    ASSERT(item.empty());
  }
  five_strings[2] = "Hello";
  ASSERT_EQUAL(five_strings[2], "Hello");
}

void TestPushBack()
{
  SimpleVector<int> vec;
  for (int i = 10; i >= 1; --i) {
    vec.PushBack(i);
    ASSERT(vec.Size() <= vec.Capacity());
  }
  sort(begin(vec), end(vec));

  const vector<int> expected = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
  ASSERT_EQUAL(vec.Size(), expected.size());
  ASSERT(equal(begin(vec), end(vec), begin(expected)));
}

int main()
{
  TestRunner trunner;
  RUN_TEST(trunner, TestConstruction);
  RUN_TEST(trunner, TestPushBack);
  return 0;
}
