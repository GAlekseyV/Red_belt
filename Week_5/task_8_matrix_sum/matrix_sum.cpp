//#include "paginator.h"
#include "test_runner.h"

#include <algorithm>
#include <future>
#include <numeric>
#include <vector>
using namespace std;

template <typename Iterator>
class IteratorRange {
private:
    Iterator first, last;
    size_t size_;
public:
    IteratorRange(Iterator begin, Iterator end)
            : first(begin)
            , last(end)
            , size_(distance(first, last)){
    }
    Iterator begin()const{
        return first;
    }
    Iterator end()const{
        return last;
    }
    size_t size() const{
        return size_;
    }
};

template <typename Iterator>
class Paginator {
public:
    Paginator(Iterator begin, Iterator end, size_t page_size) {
        for(size_t left = distance(begin, end); left > 0;){
            size_t current_page_size = min(page_size, left);
            Iterator current_page_end = next(begin, current_page_size);
            pages.push_back({begin, current_page_end});

            left -= current_page_size;
            begin = current_page_end;
        }
    }

    size_t size()const{
        // Возвращает количество страниц, на которые был разбит переданный контейнер
        return pages.size();
    }

    auto begin() const{
        return pages.begin();
    }

    auto end() const{
        return pages.end();
    }

private:
    //Iterator first, last;
    vector<IteratorRange<Iterator>> pages;
    //const size_t page_size;
};

template <typename C>
auto Paginate(C& c, size_t page_size) {
    // Реализуйте этот шаблон функции
    return Paginator(c.begin(), c.end(), page_size);
}

template <typename ContainerOfVectors>
int64_t CalculateSingleThreadMatrixSum(ContainerOfVectors& matrix){
    int64_t result = 0;
    for(const auto& row : matrix){
        result += accumulate(row.begin(), row.end(), 0);
    }
    return result;
}

int64_t CalculateMatrixSum(const vector<vector<int>>& matrix) {
  // Реализуйте эту функцию
  size_t page_size = 1500;
  int64_t result = 0;
  vector<future<int64_t >> futures;
  size_t first_row = 0;
  for(auto page : Paginate(matrix, page_size)){
    futures.push_back(
            async([page]{
                return CalculateSingleThreadMatrixSum(page);
            })
    );
  }
  for(auto& f : futures){
    result += f.get();
  }
  return result;
}

void TestCalculateMatrixSum() {
  const vector<vector<int>> matrix = {
    {1, 2, 3, 4},
    {5, 6, 7, 8},
    {9, 10, 11, 12},
    {13, 14, 15, 16}
  };
  ASSERT_EQUAL(CalculateMatrixSum(matrix), 136);
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestCalculateMatrixSum);
}
