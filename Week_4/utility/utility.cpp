//
// Created by GAlekseyV on 25.09.2018.
//
// Вспомогательные функции
#include <iostream>
#include <fstream>
#include <random>
#include <string>
#include <vector>
using namespace std;

const int MAX_Q_NUMBERS = 1'000'000;
const int MAX_USER_ID = 100'000;
const int MAX_PAGES = 1000;

void Task1Gen100KInputFile(const string& file){
    ofstream of(file);
    minstd_rand gen;
    const int request_count = 100000;
    vector<int> nums(request_count);
    of << request_count << '\n';
    int first_n;
    for(int i = 0; i < request_count; i++){
        int cur_n = gen() % request_count;
        if(i == 0) {first_n = cur_n;}
        nums[i] = cur_n;
        of << cur_n << " " << gen() % request_count << '\n';
        //of << cur_n << " " << first_n << '\n';
    }
}

int main(){
    //Task3GenAddInputFile("inputADD.txt");
    //Task3GenAddAndGOInputFile("inputADDAndGO.txt");
    Task1Gen100KInputFile("inputTask1.txt");
    return 0;
}
