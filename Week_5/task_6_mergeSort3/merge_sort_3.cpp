#include "test_runner.h"

#include <algorithm>
#include <memory>
#include <vector>

using namespace std;

template <typename RandomIt>
void MergeSort(RandomIt range_begin, RandomIt range_end) {
  // Напишите реализацию функции,
  // не копируя сортируемые элементы

  // Если диапазон содержит меньше 2 элементов, выйти из функции.
  if(range_end - range_begin < 2){
    return;
  }
  // Создать вектор, содержащий все элементы текущего диапазона.
  vector<typename RandomIt::value_type> v = {make_move_iterator(range_begin), make_move_iterator(range_end)};
  // Разбить вектор на три равные части. (В этой задаче гарантируется, что длина передаваемого диапазона является
  // степенью тройки, так что вектор всегда можно разбить на три равные части.)
  RandomIt endPart1_it = v.begin() + v.size() / 3;
  RandomIt endPart2_it = endPart1_it + v.size() / 3;
  //Вызвать функцию MergeSort от каждой части вектора.
  MergeSort(v.begin(), endPart1_it);
  MergeSort(endPart1_it, endPart2_it);
  MergeSort(endPart2_it, v.end());
  // Слить первые две трети вектора с помощью алгоритма merge, сохранив результат во временный вектор с
  // помощью back_inserter.
  vector<typename RandomIt::value_type> tmp_v;
  merge(make_move_iterator(begin(v)), make_move_iterator(endPart1_it),
          make_move_iterator(endPart1_it), make_move_iterator(endPart2_it),
          back_inserter(tmp_v));
  //Слить временный вектор из предыдущего пункта с последней третью вектора из п. 2,
  // записав полученный отсортированный диапазон вместо исходного.
  merge(make_move_iterator(begin(tmp_v)), make_move_iterator(end(tmp_v)),
          make_move_iterator(endPart2_it), make_move_iterator(end(v)),
          range_begin);
}

void TestIntVector() {
  vector<int> numbers = {6, 1, 3, 9, 1, 9, 8, 12, 1};
  MergeSort(begin(numbers), end(numbers));
  ASSERT(is_sorted(begin(numbers), end(numbers)));
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestIntVector);
  return 0;
}
