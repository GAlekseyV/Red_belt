//
// Created by GAlekseyV on 24.09.2018.
//
#include "deque.h"
#include "test_runner.h"

#include <deque>
#include <stdexcept>
#include <string>


void TestDeque()
{
  {
    Deque<int> const my_deque;
    deque<int> const exp_d;
    ASSERT_EQUAL(my_deque.Size(), exp_d.size());
    ASSERT_EQUAL(my_deque.Empty(), exp_d.empty());
  }
  {
    Deque<int> my_deque;
    deque<int> expected_d;
    my_deque.PushFront(1);
    expected_d.push_front(1);
    ASSERT_EQUAL(my_deque.Size(), expected_d.size());
    ASSERT_EQUAL(my_deque.Front(), expected_d.front());
    ASSERT_EQUAL(my_deque.Back(), expected_d.back());
    ASSERT_EQUAL(my_deque.At(0), expected_d.at(0));
    ASSERT_EQUAL(my_deque[0], expected_d[0]);
  }
  {
    Deque<int> my_deque;
    deque<int> exp_d;
    my_deque.PushBack(1);
    exp_d.push_back(1);
    ASSERT_EQUAL(my_deque.Size(), exp_d.size());
    ASSERT_EQUAL(my_deque.Front(), exp_d.front());
    ASSERT_EQUAL(my_deque.Back(), exp_d.back());
    ASSERT_EQUAL(my_deque.At(0), exp_d.at(0));
    ASSERT_EQUAL(my_deque[0], exp_d[0]);
  }
  {
    Deque<string> my_deque;
    deque<string> exp_d;
    my_deque.PushFront("One");
    my_deque.PushBack("Two");
    exp_d.emplace_front("One");
    exp_d.emplace_back("Two");
    ASSERT_EQUAL(my_deque.Size(), exp_d.size());
    ASSERT_EQUAL(my_deque.Front(), exp_d.front());
    ASSERT_EQUAL(my_deque.Back(), exp_d.back());
    ASSERT_EQUAL(my_deque.At(0), exp_d.at(0));
    ASSERT_EQUAL(my_deque.At(1), exp_d.at(1));
    ASSERT_EQUAL(my_deque[0], exp_d[0]);
    ASSERT_EQUAL(my_deque[1], exp_d[1]);
  }
  {
    Deque<int> my_deque;
    my_deque.PushFront(1);
    ASSERT_EQUAL(my_deque.Size(), 1U);
    ASSERT_EQUAL(my_deque.Front(), 1);
    ASSERT_EQUAL(my_deque.Back(), 1);
    string error;
    try {
      my_deque.At(3);
    } catch (out_of_range &er) {
      error = "catch error";
    }
    ASSERT_EQUAL(error, "catch error");
  }
  {
    Deque<int> my_deque;
    deque<double> exp_d;
    my_deque.PushFront(1);
    my_deque.PushBack(2);
    exp_d.push_front(1);
    exp_d.push_back(2);
    my_deque[0] = 3;
    my_deque[1] = 4;
    exp_d[0] = 3;
    exp_d[1] = 4;
    ASSERT_EQUAL(my_deque.Size(), exp_d.size());
    ASSERT_EQUAL(my_deque.Front(), exp_d.front());
    ASSERT_EQUAL(my_deque.Back(), exp_d.back());
    ASSERT_EQUAL(my_deque.At(0), exp_d.at(0));
    ASSERT_EQUAL(my_deque.At(1), exp_d.at(1));
    ASSERT_EQUAL(my_deque[0], exp_d[0]);
    ASSERT_EQUAL(my_deque[1], exp_d[1]);
  }
  {
    Deque<int> my_deque;
    deque<int> exp_d;
    my_deque.PushFront(1);
    my_deque.PushFront(2);
    my_deque.PushBack(3);
    my_deque.PushBack(4);
    exp_d.push_front(1);
    exp_d.push_front(2);
    exp_d.push_back(3);
    exp_d.push_back(4);
    ASSERT_EQUAL(my_deque.Size(), exp_d.size());
    ASSERT_EQUAL(my_deque.Front(), exp_d.front());
    ASSERT_EQUAL(my_deque.Back(), exp_d.back());
    ASSERT_EQUAL(my_deque.At(0), exp_d.at(0));
    ASSERT_EQUAL(my_deque.At(3), exp_d.at(3));
    ASSERT_EQUAL(my_deque[1], exp_d[1]);
    ASSERT_EQUAL(my_deque[3], exp_d[3]);
  }
}

int main()
{
  TestRunner trunner;
  RUN_TEST(trunner, TestDeque);
  return 0;
}
