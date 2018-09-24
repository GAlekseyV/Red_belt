#include "test_runner.h"
#include <sstream>
#include <string>
using namespace std;

class Logger {
public:
  explicit Logger(ostream& output_stream)
  : os(output_stream), line(0) {
  }

  void SetLogLine(bool value) { log_line = value; }
  void SetLogFile(bool value) { log_file= value; }
  void SetFileName(const string& f) {filename = f;}
  void SetLine(int l){line = l;}

  void Log(const string& m){
      if(log_file && log_line){
          os << filename << ":" << line << " " + m << endl;
      }else if(log_line){
          os << "Line " << line << " " << m << endl;
      }else if(log_file){
          os << filename << " " << m << endl;
      }else{
          os << m << endl;
      }
  }

private:
  ostream& os;
  bool log_line = false;
  bool log_file = false;
  string filename;
  int line;
};

#define LOG(logger, message)     \
    logger.SetFileName(__FILE__);\
    logger.SetLine(__LINE__);\
    logger.Log(string(message));

void TestLog() {
/* Для написания юнит-тестов в этой задаче нам нужно фиксировать конкретные
 * номера строк в ожидаемом значении (см. переменную expected ниже). Если
 * мы добавляем какой-то код выше функции TestLog, то эти номера строк меняются,
 * и наш тест начинает падать. Это неудобно.
 *
 * Чтобы этого избежать, мы используем специальный макрос #line
 * (http://en.cppreference.com/w/cpp/preprocessor/line), который позволяет
 * переопределить номер строки, а также имя файла. Благодаря ему, номера
 * строк внутри функции TestLog будут фиксированы независимо от того, какой
 * код мы добавляем перед ней*/
#line 1 "logger.cpp"

  ostringstream logs;
  Logger l(logs);
  LOG(l, "hello");

  l.SetLogFile(true);
  LOG(l, "hello");

  l.SetLogLine(true);
  LOG(l, "hello");

  l.SetLogFile(false);
  LOG(l, "hello");

  string expected = "hello\n";
  expected += "logger.cpp hello\n";
  expected += "logger.cpp:10 hello\n";
  expected += "Line 13 hello\n";
  ASSERT_EQUAL(logs.str(), expected);
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestLog);
}
