#include "test_runner.h"

#include <vector>

using namespace std;

// Объявляем Sentence<Token> для произвольного типа Token
// синонимом vector<Token>.
// Благодаря этому в качестве возвращаемого значения
// функции можно указать не малопонятный вектор векторов,
// а вектор предложений — vector<Sentence<Token>>.
template <typename Token>
using Sentence = vector<Token>;

// Класс Token имеет метод bool IsEndSentencePunctuation() const
template <typename Token>
vector<Sentence<Token>> SplitIntoSentences(vector<Token> tokens) {
  // Напишите реализацию функции, не копируя объекты типа Token
  vector<Sentence<Token>> sentences;
  vector<Token> sentence;
  bool isPrevTokenStatus = false;
  auto startSentence = tokens.begin();
  for(auto it = tokens.begin(); it != tokens.end(); ++it){
    if(isPrevTokenStatus && !it->IsEndSentencePunctuation()){
      move(startSentence, it, back_inserter(sentence));
      sentences.push_back(move(sentence));
      startSentence = it;
    }
    isPrevTokenStatus = it->IsEndSentencePunctuation();
  }
  if(startSentence != tokens.end())
  {
    move(startSentence, tokens.end(), back_inserter(sentence));
    sentences.push_back(move(sentence));
  }
  return move(sentences);
}


struct TestToken {
  string data;
  bool is_end_sentence_punctuation = false;

  bool IsEndSentencePunctuation() const {
    return is_end_sentence_punctuation;
  }
  bool operator==(const TestToken& other) const {
    return data == other.data && is_end_sentence_punctuation == other.is_end_sentence_punctuation;
  }
};

ostream& operator<<(ostream& stream, const TestToken& token) {
  return stream << token.data;
}

struct TestTokenNoCopy {
    // Копирование объектов запрещено
    TestTokenNoCopy(string token, bool flag = false)
    :data(move(token)), is_end_sentence_punctuation(flag){
    }

    TestTokenNoCopy(const TestTokenNoCopy& other){
      cout << "Copy ctor " << endl;
      data = other.data;
      is_end_sentence_punctuation = other.is_end_sentence_punctuation;
    };

    TestTokenNoCopy(TestTokenNoCopy&& other)
    : data(move(other.data))
    , is_end_sentence_punctuation(other.is_end_sentence_punctuation)
    {
      cout << "Move ctor" << endl;
    }

    string data;
    bool is_end_sentence_punctuation = false;

    bool IsEndSentencePunctuation() const {
      return is_end_sentence_punctuation;
    }
    bool operator==(const TestTokenNoCopy& other) const {
      return data == other.data && is_end_sentence_punctuation == other.is_end_sentence_punctuation;
    }
};

ostream& operator<<(ostream& stream, const TestTokenNoCopy& token) {
  return stream << token.data;
}

// Тест содержит копирования объектов класса TestToken.
// Для проверки отсутствия копирований в функции SplitIntoSentences
// необходимо написать отдельный тест.
void TestSplitting() {
  ASSERT_EQUAL(
    SplitIntoSentences(vector<TestToken>({{"Split"}, {"into"}, {"sentences"}, {"!"}})),
    vector<Sentence<TestToken>>({
        {{"Split"}, {"into"}, {"sentences"}, {"!"}}
    })
  );

  ASSERT_EQUAL(
    SplitIntoSentences(vector<TestToken>({{"Split"}, {"into"}, {"sentences"}, {"!", true}})),
    vector<Sentence<TestToken>>({
        {{"Split"}, {"into"}, {"sentences"}, {"!", true}}
    })
  );

  ASSERT_EQUAL(
    SplitIntoSentences(vector<TestToken>({{"Split"}, {"into"}, {"sentences"}, {"!", true}, {"!", true}, {"Without"}, {"copies"}, {".", true}})),
    vector<Sentence<TestToken>>({
        {{"Split"}, {"into"}, {"sentences"}, {"!", true}, {"!", true}},
        {{"Without"}, {"copies"}, {".", true}},
    })
  );
}

void TestSplittingNoCopy(){
  ASSERT_EQUAL(
    SplitIntoSentences(vector<TestTokenNoCopy>({{"Split"}, {"into"}, {"sentences"}, {"!"}})),
    vector<Sentence<TestTokenNoCopy>>({{{"Split"}, {"into"}, {"sentences"}, {"!"}}
    })
  );

  ASSERT_EQUAL(
    SplitIntoSentences(vector<TestTokenNoCopy>({{"Split"}, {"into"}, {"sentences"}, {"!", true}})),
    vector<Sentence<TestTokenNoCopy>>({{{"Split"}, {"into"}, {"sentences"}, {"!", true}}
    })
  );

  ASSERT_EQUAL(
    SplitIntoSentences(vector<TestTokenNoCopy>({{"Split"}, {"into"}, {"sentences"}, {"!", true}, {"!", true}, {"Without"}, {"copies"}, {".", true}})),
    vector<Sentence<TestTokenNoCopy>>({
      {{"Split"}, {"into"}, {"sentences"}, {"!", true}, {"!", true}},
      {{"Without"}, {"copies"}, {".", true}},
      })
  );
};

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestSplitting);
  RUN_TEST(tr, TestSplittingNoCopy);
  return 0;
}
