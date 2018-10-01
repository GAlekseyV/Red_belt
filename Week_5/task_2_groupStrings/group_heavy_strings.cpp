#include "test_runner.h"

#include <algorithm>
#include <deque>
#include <string>
#include <vector>
#include <list>

using namespace std;

// Объявляем Group<String> для произвольного типа String
// синонимом vector<String>.
// Благодаря этому в качестве возвращаемого значения
// функции можно указать не малопонятный вектор векторов,
// а вектор групп — vector<Group<String>>.
template <typename String>
using Group = vector<String>;

// Ещё один шаблонный синоним типа
// позволяет вместо громоздкого typename String::value_type
// использовать Char<String>
template <typename String>
using Char = typename String::value_type;

//
template <typename String>
class GroupChar{
public:
    explicit GroupChar(const String& string){
        for(auto& c : string){
            group.insert(c);
        }
    }
    bool isStrInGroup(const String& str){
        return group == GroupChar(str).group;
    }
private:
    set<typename String::value_type> group;
};

template <typename String>
vector<Group<String>> GroupHeavyStrings(vector<String> strings) {
  // Напишите реализацию функции,
  // использовав не более 1 копирования каждого символа
  list<String> strings_list;
  move(strings.begin(), strings.end(), back_inserter(strings_list));
  vector<vector<String>> result;
  //vector<String> words;

  while(!strings_list.empty())
  {
      GroupChar cur_group = GroupChar(strings_list.front());
      result.resize(result.size() + 1);
      for(auto it = strings_list.begin(); it != strings_list.end();){
          if(cur_group.isStrInGroup(*it)){
              result[result.size() - 1].push_back(move(*it));
              //words.push_back(move(*it));
              it = strings_list.erase(it);
          }else{
              it++;
          }
      }
      //result.push_back(move(words));
  }

  return result;
}


void TestGroupingABC() {
  vector<string> strings = {"caab", "abc", "cccc", "bacc", "c"};
  auto groups = GroupHeavyStrings(strings);
  ASSERT_EQUAL(groups.size(), 2);
  sort(begin(groups), end(groups));  // Порядок групп не имеет значения
  ASSERT_EQUAL(groups[0], vector<string>({"caab", "abc", "bacc"}));
  ASSERT_EQUAL(groups[1], vector<string>({"cccc", "c"}));
}

void TestGroupingReal() {
  vector<string> strings = {"law", "port", "top", "laptop", "pot", "paloalto", "wall", "awl"};
  auto groups = GroupHeavyStrings(strings);
  ASSERT_EQUAL(groups.size(), 4);
  sort(begin(groups), end(groups));  // Порядок групп не имеет значения
  ASSERT_EQUAL(groups[0], vector<string>({"laptop", "paloalto"}));
  ASSERT_EQUAL(groups[1], vector<string>({"law", "wall", "awl"}));
  ASSERT_EQUAL(groups[2], vector<string>({"port"}));
  ASSERT_EQUAL(groups[3], vector<string>({"top", "pot"}));
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestGroupingABC);
  RUN_TEST(tr, TestGroupingReal);
  return 0;
}
