#include "test_runner.h"

#include <cstddef>
#include <list>
#include <string>

using namespace std;

class Editor
{
public:
  // Реализуйте конструктор по умолчанию и объявленные методы
  Editor()
    : pos(text.end())
  {
  }

  void Left()
  {
    pos = Advance(pos, -1);
  }

  void Right()
  {
    pos = Advance(pos, 1);
  }

  void Insert(char token)
  {
    text.insert(pos, token);
  }

  void Cut(size_t tokens = 1)
  {
    auto pos2 = Advance(pos, tokens);
    buffer.assign(pos, pos2);
    pos = text.erase(pos, pos2);
  }

  void Copy(size_t tokens = 1)
  {
    auto pos2 = Advance(pos, tokens);
    buffer.assign(pos, pos2);
  }

  void Paste()
  {
    text.insert(pos, buffer.begin(), buffer.end());
  }

  [[nodiscard]] string GetText() const
  {
    return { text.begin(), text.end() };
  }

private:
  using Iterator = list<char>::iterator;
  list<char> text;
  list<char> buffer;
  Iterator pos;

  Iterator Advance(Iterator itr, int steps)
  {
    while (steps > 0 && itr != text.end()) {
      ++itr;
      --steps;
    }
    while (steps < 0 && itr != text.begin()) {
      --itr;
      ++steps;
    }
    return itr;
  }
};


void TypeText(Editor &editor, const string &text)
{
  for (const char c : text) {
    editor.Insert(c);
  }
}

void TestEditing()
{
  {
    Editor editor;

    const size_t text_len = 12;
    const size_t first_part_len = 7;
    TypeText(editor, "hello, world");
    for (size_t i = 0; i < text_len; ++i) {
      editor.Left();
    }
    editor.Cut(first_part_len);
    for (size_t i = 0; i < text_len - first_part_len; ++i) {
      editor.Right();
    }
    TypeText(editor, ", ");
    editor.Paste();
    editor.Left();
    editor.Left();
    editor.Cut(3);

    ASSERT_EQUAL(editor.GetText(), "world, hello");
  }
  {
    Editor editor;

    TypeText(editor, "misprnit");
    editor.Left();
    editor.Left();
    editor.Left();
    editor.Cut(1);
    editor.Right();
    editor.Paste();

    ASSERT_EQUAL(editor.GetText(), "misprint");
  }
}

void TestReverse()
{
  Editor editor;

  const string text = "esreveR";
  for (const char c : text) {
    editor.Insert(c);
    editor.Left();
  }

  ASSERT_EQUAL(editor.GetText(), "Reverse");
}

void TestNoText()
{
  Editor editor;
  ASSERT_EQUAL(editor.GetText(), "");

  editor.Left();
  editor.Left();
  editor.Right();
  editor.Right();
  editor.Copy(0);
  editor.Cut(0);
  editor.Paste();

  ASSERT_EQUAL(editor.GetText(), "");
}

void TestEmptyBuffer()
{
  Editor editor;

  editor.Paste();
  TypeText(editor, "example");
  editor.Left();
  editor.Left();
  editor.Paste();
  editor.Right();
  editor.Paste();
  editor.Copy(0);
  editor.Paste();
  editor.Left();
  editor.Cut(0);
  editor.Paste();

  ASSERT_EQUAL(editor.GetText(), "example");
}

int main()
{
  TestRunner trunner;
  RUN_TEST(trunner, TestEditing);
  RUN_TEST(trunner, TestReverse);
  RUN_TEST(trunner, TestNoText);
  RUN_TEST(trunner, TestEmptyBuffer);
  return 0;
}
