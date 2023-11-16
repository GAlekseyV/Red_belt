#include "test_runner.h"

#include <algorithm>
#include <cstddef>
#include <string>
#include <vector>
using namespace std;

template<typename T>
void Swap(T *first, T *second)
{
  T temp = *first;
  *first = *second;
  *second = temp;
}

template<typename T>
void SortPointers(vector<T *> &pointers)
{
  sort(pointers.begin(), pointers.end(), [](T *lhs, T *rhs) { return *lhs < *rhs; });
}

template<typename T>
void ReversedCopy(T *source, size_t count, T *destination)
{
  auto source_begin = source;
  auto source_end = source + count;
  auto dest_begin = destination;
  auto dest_end = destination + count;
  if (dest_begin >= source_end || dest_end <= source_begin) {
    reverse_copy(source_begin, source_end, dest_begin);
  } else if (dest_begin > source_begin) {
    /* Случай, когда целевая область лежит правее исходной
     * |   |   |   |d_6|d_5|d_4|d_3|d_2|d_1|
     * |s_1|s_2|s_3|s_4|s_5|s_6|   |   |   |
     * */
    for (size_t i = 0; source_begin + i < dest_begin; ++i) {
      *(dest_end - i - 1) = *(source_begin + i);
    }
    reverse(dest_begin, source_end);
  } else {
    /* Случай, когда целевая область лежит левее исходной
     * |d_6|d_5|d_4|d_3|d_2|d_1|   |   |   |
     * |   |   |   |s_1|s_2|s_3|s_4|s_5|s_6|
     * */
    for (size_t i = 0; source_end - i - 1 >= dest_end; ++i) {
      *(dest_begin + i) = *(source_end - i - 1);
    }
    reverse(source_begin, dest_end);
  }
}

void TestSwap()
{
  int num1 = 1;
  int num2 = 2;
  Swap(&num1, &num2);
  ASSERT_EQUAL(num1, 2);
  ASSERT_EQUAL(num2, 1);

  string str1 = "world";
  string str2 = "hello";
  Swap(&str1, &str2);
  ASSERT_EQUAL(str1, "hello");
  ASSERT_EQUAL(str2, "world");
}

void TestSortPointers()
{
  int one = 1;
  int two = 2;
  int three = 3;

  vector<int *> pointers;
  pointers.push_back(&two);
  pointers.push_back(&three);
  pointers.push_back(&one);

  SortPointers(pointers);

  ASSERT_EQUAL(pointers.size(), 3U);
  ASSERT_EQUAL(*pointers[0], 1);
  ASSERT_EQUAL(*pointers[1], 2);
  ASSERT_EQUAL(*pointers[2], 3);
}

void TestReverseCopy()
{
  const size_t count = 7;

  auto *source = new size_t[count];
  auto *dest = new size_t[count];

  for (size_t i = 0; i < count; ++i) {
    source[i] = i + 1;
  }
  ReversedCopy(source, count, dest);
  const vector<int> expected1 = { 7, 6, 5, 4, 3, 2, 1 };
  ASSERT_EQUAL(vector<int>(dest, dest + count), expected1);

  // Области памяти могут перекрываться
  ReversedCopy(source, count - 1, source + 1);
  const vector<int> expected2 = { 1, 6, 5, 4, 3, 2, 1 };
  ASSERT_EQUAL(vector<int>(source, source + count), expected2);

  delete[] dest;
  delete[] source;
}

int main()
{
  TestRunner trunner;
  RUN_TEST(trunner, TestSwap);
  RUN_TEST(trunner, TestSortPointers);
  RUN_TEST(trunner, TestReverseCopy);
  return 0;
}
