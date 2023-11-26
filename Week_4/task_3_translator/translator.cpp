#include "test_runner.h"
#include <deque>
#include <map>
#include <string>
#include <string_view>

using namespace std;

class Translator
{
public:
  void Add(string_view source, string_view target);
  [[nodiscard]] string_view TranslateForward(string_view source) const;
  [[nodiscard]] string_view TranslateBackward(string_view target) const;

private:
  string_view GetCopy(string_view str);
  // Дополнительный метод для устранения дублирования в методах TranslateForward и
  static string_view Translate(const map<string_view, string_view> &dict, string_view str);

  // String_view в качестве ключа словаря
  map<string_view, string_view> forward_dict;
  map<string_view, string_view> backward_dict;

  // Используетcя дек, так как у него неинвалидируются ссылки при релокации.
  deque<string> words;
};

void Translator::Add(string_view source, string_view target)
{
  // Так как переданный объект может быть уничтожен, его необходимо сохранить
  const string_view source_view = GetCopy(source);
  const string_view target_view = GetCopy(target);
  forward_dict[source_view] = target_view;
  backward_dict[target_view] = source_view;
}

string_view Translator::GetCopy(string_view str)
{
  // Копирования переданного string_view
  // 1. Поиск переданного string_view в словаре forward_dict, а затем backward_dict
  for (const auto *map_ptr : { &forward_dict, &backward_dict }) {
    const auto itr = map_ptr->find(str);
    if (itr != map_ptr->end()) {
      // Если нашли, то возвращаем string_view на него
      return itr->first;
    }
  }
  // Добавляем string_view в дек слов
  return words.emplace_back(str);
}

string_view Translator::Translate(const map<string_view, string_view> &dict, string_view str)
{
  if (const auto itr = dict.find(str); itr != dict.end()) {
    return itr->second;
  } else {
    return {};
  }
}

string_view Translator::TranslateForward(string_view source) const
{
  return Translate(forward_dict, source);
}

string_view Translator::TranslateBackward(string_view target) const
{
  return Translate(backward_dict, target);
}

void TestSimple()
{
  Translator translator;
  translator.Add(string("okno"), string("window"));
  translator.Add(string("stol"), string("table"));

  ASSERT_EQUAL(translator.TranslateForward("okno"), "window");
  ASSERT_EQUAL(translator.TranslateBackward("table"), "stol");
  ASSERT_EQUAL(translator.TranslateBackward("stol"), "");
}

int main()
{
  TestRunner trunner;
  RUN_TEST(trunner, TestSimple);
  return 0;
}
