#include "test_runner.h"

#include <cstddef>
#include <utility>
#include <vector>

using namespace std;

// Реализуйте здесь шаблонный класс Table
template<typename T>
class Table
{
public:
  Table(size_t rows, size_t columns)
  {
    Resize(rows, columns);
  }
  vector<T> &operator[](size_t index)
  {
    return table[index];
  }
  const vector<T> &operator[](size_t index) const
  {
    return table[index];
  }
  void Resize(size_t new_rows, size_t new_columns)
  {
    table.resize(new_rows);
    for (auto &row : table) {
      row.resize(new_columns);
    }
  }
  [[nodiscard]] pair<size_t, size_t> Size() const
  {
    return make_pair(table.size(), (table.empty() ? 0 : table[0].size()));
  }

private:
  vector<vector<T>> table;
};


void TestTable()
{
  Table<int> tbl(1, 1);
  ASSERT_EQUAL(tbl.Size().first, 1U);
  ASSERT_EQUAL(tbl.Size().second, 1U);
  const int test_data = 42;
  tbl[0][0] = test_data;
  ASSERT_EQUAL(tbl[0][0], test_data);
  tbl.Resize(3, 4);
  ASSERT_EQUAL(tbl.Size().first, 3U);
  ASSERT_EQUAL(tbl.Size().second, 4U);
}

int main()
{
  TestRunner trunner;
  RUN_TEST(trunner, TestTable);
  return 0;
}
