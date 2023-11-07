#include "test_runner.h"

#include <ostream>
#include <sstream>
using namespace std;

#define PRINT_VALUES(out, x, y) \
  (out) << (x) << '\n'          \
        << (y) << '\n'

int main()
{
  TestRunner tr;
  tr.RunTest([] {
    ostringstream output;
    PRINT_VALUES(output, 5, "red belt");
    ASSERT_EQUAL(output.str(), "5\nred belt\n");
  },
    "PRINT_VALUES usage example");
}
