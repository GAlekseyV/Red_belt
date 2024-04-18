#include "test_runner.h"
#include <algorithm>
#include <iostream>
#include <iterator>
#include <memory>
#include <set>
#include <utility>
#include <vector>
#include <list>

using namespace std;

template <typename T>
class PriorityCollection {
public:
  /* тип, используемый для идентификаторов */
  using Id = typename set<T>::iterator;

  // Добавить объект с нулевым приоритетом
  // с помощью перемещения и вернуть его идентификатор
  Id Add(T object) {
    auto id = objects.insert(std::move(object)).first;
    if (priority.size() == 0) {
      priority.push_back(list<Id>());
    }
      priority[0].push_back(id);
    return id;
  }

  // Добавить все элементы диапазона [range_begin, range_end)
  // с помощью перемещения, записав выданные им идентификаторы
  // в диапазон [ids_begin, ...)
  template <typename ObjInputIt, typename IdOutputIt>
  void Add(ObjInputIt range_begin, ObjInputIt range_end,
           IdOutputIt ids_begin);

  // Определить, принадлежит ли идентификатор какому-либо
  // хранящемуся в контейнере объекту
  bool IsValid(Id id) const {
    return objects.count(*id) > 0;
  }

  // Получить объект по идентификатору
  const T& Get(Id id) const {
      if (IsValid(id)) {
      return (*id);
    }
      return {};
  }

  // Увеличить приоритет объекта на 1
  void Promote(Id id) {
    pair<Id, int> p = find(id);
    if (p.second < (priority.size() - 1)) {
      priority[p.second + 1].push_back(p.first);
    } else {
      priority.push_back(list<Id>());
      priority.back().push_back(id);
    }
    priority[p.second].remove(p.first);
  }

  // Получить объект с максимальным приоритетом и его приоритет
  pair<const T &, int> GetMax() const
  {
    int p = priority.size() - 1;
    auto it = priority.back().back();
    return make_pair(move(*it, p);
  };

  // Аналогично GetMax, но удаляет элемент из контейнера
  pair<T, int> PopMax(){
    int p = priority.size() - 1;
    while (priority[p].size() < 1) {
      p--;
    }
    auto it = priority[p].back();
    priority[p].pop_back();
    return make_pair(move(objects.extract(it).value()), p);
  };

private:
    pair<Id, int> find(Id id) {
      //assert(IsValid(id));
      int i = 0;
      typename std::list<Id>::iterator it;
      for (; i < priority.size(); i++) {
        it = std::find(priority[i].begin(), priority[i].end(), id);
        
        if (it != priority[i].end()) {
          break;
        }
      }
      return { *it, i };
  }
  // Приватные поля и методы
  set<T> objects;
  vector<list<Id>> priority;
};


class StringNonCopyable : public string {
public:
  using string::string;  // Позволяет использовать конструкторы строки
  StringNonCopyable(const StringNonCopyable&) = delete;
  StringNonCopyable(StringNonCopyable&&) = default;
  StringNonCopyable& operator=(const StringNonCopyable&) = delete;
  StringNonCopyable& operator=(StringNonCopyable&&) = default;
};

void TestNoCopy() {
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

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestNoCopy);
  return 0;
}
