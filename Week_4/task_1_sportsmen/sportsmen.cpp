//
// Created by GAlekseyV on 29.09.2018.
//
#include "profile.h"

#include <algorithm>
#include <deque>
#include <iostream>
#include <map>
#include <list>
#include <fstream>
#include <vector>

using namespace std;


int main(){
    ifstream is("inputTask1.txt");
    ofstream os("outW4T1list.txt");

    int n, cur_n, next_n;
    list<int> q_l;
    {
        LOG_DURATION("Input and insert to list")
        is >> n;
        vector<list<int>::iterator> pos(n, q_l.end());
        for(int i = 0; i < n; ++i){
            is >> cur_n >> next_n;
            pos[cur_n] = q_l.insert(pos[next_n], cur_n);
        }
    }

    {
        LOG_DURATION("Output list")
        for(auto item : q_l){
            os << item << " ";
        }
    }

    ifstream isv("inputTask1.txt");
    ofstream osv("outW4T1v.txt");

    {
        LOG_DURATION("Input and insert to vector")
        isv >> n;
        vector<int> q_v;
        isv >> cur_n >> next_n;
        q_v.push_back(cur_n);
        for(int i = 1; i < n; ++i){
            isv >> cur_n >> next_n;
            auto prev_it = find(q_v.begin(), q_v.end(), next_n);
            q_v.insert(prev_it, cur_n);
        }

        {
            LOG_DURATION("Output vector")
            for(auto item : q_v){
                os << item << " ";
            }
        }
    }

    return 0;
}

int main1(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, cur_n, next_n;
    cin >> n;
    list<int> q_l;
    vector<list<int>::iterator> pos(n, q_l.end());
    for(int i = 0; i < n; ++i){
        cin >> cur_n >> next_n;
        pos[cur_n] = q_l.insert(pos[next_n], cur_n);
    }

    for(auto item : q_l){
        cout << item << " ";
    }
    return 0;
}
