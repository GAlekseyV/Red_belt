#include "profile.h"
#include "student.h"
#include "test_runner.h"

#include <algorithm>
#include <vector>

using namespace std;

bool Compare(const Student &first, const Student &second)
{
  return first.Less(second);
}

void TestComparison()
{
  Student const newbie{
    "Ivan", "Ivanov", { { "c++", 1.0 }, { "algorithms", 3.0 } }, 2.0
  };

  Student const cpp_expert{
    "Petr", "Petrov", { { "c++", 9.5 }, { "algorithms", 6.0 } }, 7.75
  };

  Student const guru{
    "Sidor", "Sidorov", { { "c++", 10.0 }, { "algorithms", 10.0 } }, 10.0
  };
  ASSERT(Compare(guru, newbie));
  ASSERT(Compare(guru, cpp_expert));
  ASSERT(!Compare(newbie, cpp_expert));
}

void TestSorting()
{
  vector<Student> students(1000);
  for (auto &student : students) {
    student = Student{ "Sidor", "Sidorov", { { "maths", 2. } }, 2. };
  };
  {
    LOG_DURATION("Sort with compare")
    sort(students.begin(), students.end(), Compare);
  }

  ASSERT(is_sorted(students.begin(), students.end(), [](const Student &first, const Student &second) {
    return first.Less(second);
  }));
}

int main()
{
  TestRunner trunner;
  RUN_TEST(trunner, TestComparison);
  RUN_TEST(trunner, TestSorting);
  return 0;
}