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
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n = 0;
    int cur_n = 0;
    int next_n = 0;
    cin >> n;
    list<int> q_l;
    map<int, list<int>::iterator> num_to_it;

    for(int i = 0; i < n; ++i){
        cin >> cur_n >> next_n;
        auto pos_it = q_l.end();
        if(num_to_it.count(next_n) != 0){
          pos_it = num_to_it[next_n];
        }
        num_to_it[cur_n] = q_l.insert(pos_it, cur_n);
    }

    for(auto item : q_l){
        cout << item << " ";
    }
    return 0;
}
