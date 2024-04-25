#include "test_runner.h"
#include <algorithm>
#include <cstddef>
#include <iterator>
#include <set>
#include <string>
#include <utility>
#include <vector>

using namespace std;

template<typename T>
class PriorityCollection
{
public:
  /* тип, используемый для идентификаторов */
  using Id = size_t;

  // Добавить объект с нулевым приоритетом
  // с помощью перемещения и вернуть его идентификатор
  Id Add(T object)
  {
    objects.emplace_back(move(object), 0);
    sorted_objects.emplace(0, last_id);
    return Id(last_id++);
  }

  // Добавить все элементы диапазона [range_begin, range_end)
  // с помощью перемещения, записав выданные им идентификаторы
  // в диапазон [ids_begin, ...)
  template<typename ObjInputIt, typename IdOutputIt>
  void Add(ObjInputIt range_begin, ObjInputIt range_end, IdOutputIt ids_begin)
  {
    while (range_begin != range_end) {
      *ids_begin++ = Add(move(*range_begin++));
    }
  }

  // Определить, принадлежит ли идентификатор какому-либо
  // хранящемуся в контейнере объекту
  [[nodiscard]] bool IsValid(Id id) const
  {
    return objects[id].second >= 0;
  }

  // Получить объект по идентификатору
  [[nodiscard]] const T &Get(Id id) const
  {
    return objects[id].first;
  }

  // Увеличить приоритет объекта на 1
  void Promote(Id id)
  {
    auto old_priority = objects[id].second++;
    auto new_priority = old_priority + 1;
    sorted_objects.erase({ old_priority, id });
    sorted_objects.emplace(new_priority, id);
  }

  // Получить объект с максимальным приоритетом и его приоритет
  pair<const T &, int> GetMax() const
  {
    return objects[prev(sorted_objects.end())->second];
  };

  // Аналогично GetMax, но удаляет элемент из контейнера
  pair<T, int> PopMax()
  {
    auto id = prev(sorted_objects.end())->second;
    auto result = move(objects[id]);
    sorted_objects.erase({ result.second, id });
    objects[id] = make_pair(T(), -1);
    return result;
  };

private:
  // Приватные поля и методы
  set<pair<int, Id>> sorted_objects;
  vector<std::pair<T, int>> objects;
  size_t last_id = 0;
};


class StringNonCopyable : public string
{
public:
  using string::string;// Позволяет использовать конструкторы строки
  StringNonCopyable(const StringNonCopyable &) = delete;
  StringNonCopyable(StringNonCopyable &&) = default;
  StringNonCopyable &operator=(const StringNonCopyable &) = delete;
  StringNonCopyable &operator=(StringNonCopyable &&) = default;
};

void TestNoCopy()
{
  PriorityCollection<StringNonCopyable> strings;
  const auto white_id = strings.Add("white");
  const auto yellow_id = strings.Add("yellow");
  const auto red_id = strings.Add("red");

  strings.Promote(yellow_id);
  for (int i = 0; i < 2; ++i) {
    strings.Promote(red_id);
  }
  strings.Promote(yellow_id);
  {
    const auto item = strings.PopMax();
    ASSERT_EQUAL(item.first, "red");
    ASSERT_EQUAL(item.second, 2);
  }
  {
    const auto item = strings.PopMax();
    ASSERT_EQUAL(item.first, "yellow");
    ASSERT_EQUAL(item.second, 2);
  }
  {
    const auto item = strings.PopMax();
    ASSERT_EQUAL(item.first, "white");
    ASSERT_EQUAL(item.second, 0);
  }
}

int main()
{
  TestRunner tr;
  RUN_TEST(tr, TestNoCopy);
  return 0;
}
