#include "test_runner.h"

#include <algorithm>
#include <cctype>
#include <cstddef>
#include <iostream>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

// Реализуйте шаблон класса Paginator
template<typename Iterator>
class IteratorRange
{
private:
  Iterator first, last;
  size_t size_;

public:
  IteratorRange(Iterator begin, Iterator end)
    : first(begin), last(end), size_(distance(first, last))
  {
  }
  [[nodiscard]] Iterator begin() const
  {
    return first;
  }
  [[nodiscard]] Iterator end() const
  {
    return last;
  }
  [[nodiscard]] size_t size() const
  {
    return size_;
  }
};

template<typename Iterator>
class Paginator
{
public:
  Paginator(Iterator begin, Iterator end, size_t page_size)
  {
    for (size_t left = distance(begin, end); left > 0;) {
      size_t const current_page_size = min(page_size, left);
      Iterator current_page_end = next(begin, current_page_size);
      pages.push_back({ begin, current_page_end });

      left -= current_page_size;
      begin = current_page_end;
    }
  }

  [[nodiscard]] size_t size() const
  {
    // Возвращает количество страниц, на которые был разбит переданный контейнер
    return pages.size();
  }

  [[nodiscard]] auto begin() const
  {
    return pages.begin();
  }

  [[nodiscard]] auto end() const
  {
    return pages.end();
  }

private:
  // Iterator first, last;
  vector<IteratorRange<Iterator>> pages;
  // const size_t page_size;
};

template<typename Container>
auto Paginate(Container &container, size_t page_size)
{
  // Реализуйте этот шаблон функции
  return Paginator(container.begin(), container.end(), page_size);
}

void TestPageCounts()
{
  const vector<int> vec(15);

  ASSERT_EQUAL(Paginate(vec, 1).size(), vec.size());
  ASSERT_EQUAL(Paginate(vec, 3).size(), 5U);
  ASSERT_EQUAL(Paginate(vec, 5).size(), 3U);
  ASSERT_EQUAL(Paginate(vec, 4).size(), 4U);
  ASSERT_EQUAL(Paginate(vec, 15).size(), 1U);
  ASSERT_EQUAL(Paginate(vec, 150).size(), 1U);
  ASSERT_EQUAL(Paginate(vec, 14).size(), 2U);

  vector<int> vec1;
  ASSERT_EQUAL(Paginate(vec1, 1).size(), 0U);
}

void TestLooping()
{
  vector<int> vec(15);
  iota(begin(vec), end(vec), 1);

  Paginator<vector<int>::iterator> const paginate_v(vec.begin(), vec.end(), 6);
  ostringstream os;
  for (const auto &page : paginate_v) {
    for (int const x : page) {
      os << x << ' ';
    }
    os << '\n';
  }

  ASSERT_EQUAL(os.str(), "1 2 3 4 5 6 \n7 8 9 10 11 12 \n13 14 15 \n");
}

void TestModification()
{
  vector<string> vstr = { "one", "two", "three", "four", "five" };
  for (auto page : Paginate(vstr, 2)) {
    for (auto &word : page) {
      word[0] = toupper(word[0]);
    }
  }

  const vector<string> expected = { "One", "Two", "Three", "Four", "Five" };
  ASSERT_EQUAL(vstr, expected);
}

void TestPageSizes()
{
  string letters(26, ' ');

  Paginator const letters_pagination(letters.begin(), letters.end(), 11);
  vector<size_t> page_sizes;
  for (const auto &page : letters_pagination) {
    page_sizes.push_back(page.size());
  }

  const vector<size_t> expected = { 11, 11, 4 };
  ASSERT_EQUAL(page_sizes, expected);
}

void TestConstContainer()
{
  const string letters = "abcdefghijklmnopqrstuvwxyz";

  vector<string> pages;
  for (const auto &page : Paginate(letters, 10)) {
    pages.emplace_back(page.begin(), page.end());
  }

  const vector<string> expected = { "abcdefghij", "klmnopqrst", "uvwxyz" };
  ASSERT_EQUAL(pages, expected);
}

void TestPagePagination()
{
  vector<int> vec(22);
  iota(begin(vec), end(vec), 1);

  vector<vector<int>> lines;
  for (const auto &split_by_9 : Paginate(vec, 9)) {
    for (const auto &split_by_4 : Paginate(split_by_9, 4)) {
      lines.emplace_back();
      for (int const item : split_by_4) {
        lines.back().push_back(item);
      }
    }
  }

  const vector<vector<int>> expected = {
    { 1, 2, 3, 4 },
    { 5, 6, 7, 8 },
    { 9 },
    { 10, 11, 12, 13 },
    { 14, 15, 16, 17 },
    { 18 },
    { 19, 20, 21, 22 }
  };
  ASSERT_EQUAL(lines, expected);
}

int main()
{
  TestRunner trunner;
  RUN_TEST(trunner, TestPageCounts);
  RUN_TEST(trunner, TestLooping);
  RUN_TEST(trunner, TestModification);
  RUN_TEST(trunner, TestPageSizes);
  RUN_TEST(trunner, TestConstContainer);
  RUN_TEST(trunner, TestPagePagination);
}
