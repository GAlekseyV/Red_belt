#include "stack_vector.h"

#include "profile.h"
#include "test_runner.h"

#include <cstddef>
#include <iostream>
#include <ostream>
#include <random>
#include <stdexcept>
#include <vector>
using namespace std;

void TestConstruction()
{
  const size_t array_size = 10;
  StackVector<int, array_size> st_vec_10;
  ASSERT_EQUAL(st_vec_10.Size(), 0U);
  ASSERT_EQUAL(st_vec_10.Capacity(), array_size);
  ASSERT_EQUAL(st_vec_10.begin(), st_vec_10.end());

  StackVector<int, 8> const st_vec_8(5);
  ASSERT_EQUAL(st_vec_8.Size(), 5U);
  ASSERT_EQUAL(st_vec_8.Capacity(), 8U);

  try {
    StackVector<int, 10> const st_vec(50);
    Assert(false, "Expect invalid_argument for too large size");
  } catch (invalid_argument &) {
  } catch (...) {
    Assert(false, "Unexpected exception for too large size");
  }
}

void TestPushBack()
{
  StackVector<size_t, 5> st_vec;
  for (size_t i = 0; i < st_vec.Capacity(); ++i) {
    st_vec.PushBack(i);
  }

  try {
    st_vec.PushBack(0);
    Assert(false, "Expect overflow_error for PushBack in full vector");
  } catch (overflow_error &) {
  } catch (...) {
    Assert(false, "Unexpected exception for PushBack in full vector");
  }
}

void TestPopBack()
{
  StackVector<size_t, 5> vec;
  for (size_t i = 1; i <= vec.Capacity(); ++i) {
    vec.PushBack(i);
  }
  for (size_t i = vec.Size(); i > 0; --i) {
    ASSERT_EQUAL(vec.PopBack(), i);
  }

  try {
    vec.PopBack();
    Assert(false, "Expect underflow_error for PopBack from empty vector");
  } catch (underflow_error &) {
  } catch (...) {
    Assert(false, "Unexpected exception for PopBack from empty vector");
  }
}

int main()
{
  {
    TestRunner trunner;
    RUN_TEST(trunner, TestConstruction);
    RUN_TEST(trunner, TestPushBack);
    RUN_TEST(trunner, TestPopBack);
  }

  cerr << "Running benchmark..." << '\n';
  const size_t max_size = 2500;

  default_random_engine rand_eng;
  uniform_int_distribution<int> value_gen(1, max_size);

  vector<vector<int>> test_data(50000);
  for (auto &cur_vec : test_data) {
    cur_vec.resize(value_gen(rand_eng));
    for (int &x : cur_vec) {
      x = value_gen(rand_eng);
    }
  }

  {
    LOG_DURATION("vector without reserve");
    for (auto &cur_vec : test_data) {
      vector<int> vec;
      for (int const x : cur_vec) {
        vec.push_back(x);
      }
    }
  }
  {
    LOG_DURATION("vector with reserve");
    for (auto &cur_vec : test_data) {
      vector<int> vec;
      vec.reserve(cur_vec.size());
      for (int const x : cur_vec) {
        vec.push_back(x);
      }
    }
  }
  {
    LOG_DURATION("StackVector");
    for (auto &cur_vec : test_data) {
      StackVector<int, max_size> vec;
      for (int const x : cur_vec) {
        vec.PushBack(x);
      }
    }
  }
  cerr << "Done" << '\n';
}
