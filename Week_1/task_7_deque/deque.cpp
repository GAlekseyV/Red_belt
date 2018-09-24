//
// Created by GAlekseyV on 24.09.2018.
//
#include "test_runner.h"
#include "deque.h"

#include <deque>


void TestDeque() {
    {
        Deque<int> d;
        deque<int> exp_d;
        ASSERT_EQUAL(d.Size(), exp_d.size());
        ASSERT_EQUAL(d.Empty(), exp_d.empty());
    }
    {
        Deque<int> d;
        deque<int> expected_d;
        d.PushFront(1);
        expected_d.push_front(1);
        ASSERT_EQUAL(d.Size(), expected_d.size());
        ASSERT_EQUAL(d.Front(), expected_d.front());
        ASSERT_EQUAL(d.Back(), expected_d.back());
        ASSERT_EQUAL(d.At(0), expected_d.at(0));
        ASSERT_EQUAL(d[0], expected_d[0]);
    }
    {
        Deque<int> d;
        deque<int> exp_d;
        d.PushBack(1);
        exp_d.push_back(1);
        ASSERT_EQUAL(d.Size(),exp_d.size() );
        ASSERT_EQUAL(d.Front(), exp_d.front());
        ASSERT_EQUAL(d.Back(), exp_d.back());
        ASSERT_EQUAL(d.At(0), exp_d.at(0));
        ASSERT_EQUAL(d[0], exp_d[0]);
    }
    {
        Deque<string> d;
        deque<string> exp_d;
        d.PushFront("One");
        d.PushBack("Two");
        exp_d.push_front("One");
        exp_d.emplace_back("Two");
        ASSERT_EQUAL(d.Size(), exp_d.size());
        ASSERT_EQUAL(d.Front(), exp_d.front());
        ASSERT_EQUAL(d.Back(), exp_d.back());
        ASSERT_EQUAL(d.At(0), exp_d.at(0));
        ASSERT_EQUAL(d.At(1), exp_d.at(1));
        ASSERT_EQUAL(d[0], exp_d[0]);
        ASSERT_EQUAL(d[1], exp_d[1]);
    }
    {
        Deque<int> d;
        d.PushFront(1);
        ASSERT_EQUAL(d.Size(), 1u);
        ASSERT_EQUAL(d.Front(), 1);
        ASSERT_EQUAL(d.Back(), 1);
        string error;
        try{
            d.At(3);
        }catch (out_of_range& er){
            error = "catch error";
        }
        ASSERT_EQUAL(error, "catch error");
    }
    {
        Deque<int> d;
        deque<double> exp_d;
        d.PushFront(1);
        d.PushBack(2);
        exp_d.push_front(1);
        exp_d.push_back(2);
        d[0] = 3;
        d[1] = 4;
        exp_d[0] = 3;
        exp_d[1] = 4;
        ASSERT_EQUAL(d.Size(), exp_d.size());
        ASSERT_EQUAL(d.Front(), exp_d.front());
        ASSERT_EQUAL(d.Back(), exp_d.back());
        ASSERT_EQUAL(d.At(0), exp_d.at(0));
        ASSERT_EQUAL(d.At(1), exp_d.at(1));
        ASSERT_EQUAL(d[0], exp_d[0]);
        ASSERT_EQUAL(d[1], exp_d[1]);
    }
    {
        Deque<int> d;
        deque<int> exp_d;
        d.PushFront(1);
        d.PushFront(2);
        d.PushBack(3);
        d.PushBack(4);
        exp_d.push_front(1);
        exp_d.push_front(2);
        exp_d.push_back(3);
        exp_d.push_back(4);
        ASSERT_EQUAL(d.Size(), exp_d.size());
        ASSERT_EQUAL(d.Front(), exp_d.front());
        ASSERT_EQUAL(d.Back(), exp_d.back());
        ASSERT_EQUAL(d.At(0), exp_d.at(0));
        ASSERT_EQUAL(d.At(3), exp_d.at(3));
        ASSERT_EQUAL(d[1], exp_d[1]);
        ASSERT_EQUAL(d[3], exp_d[3]);
    }

}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestDeque);
    return 0;
}
