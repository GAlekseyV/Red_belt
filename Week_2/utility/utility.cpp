//
// Created by GAlekseyV on 25.09.2018.
//
// Вспомогательные функции
#include <iostream>
#include <fstream>
#include <random>
#include <string>
using namespace std;

const int MAX_Q_NUMBERS = 1'000'000;
const int MAX_USER_ID = 100'000;
const int MAX_PAGES = 1000;

void Task3GenAddInputFile(const string& file){
    ofstream of(file);
    minstd_rand gen;
    for(int i = 0; i < 100000; i++){
        of << "ADD" << " " << gen() << " " << gen() << '\n';
    }
}

void Task3GenAddAndGOInputFile(const string& file){
    ofstream of(file);
    of << 100000 << '\n';
    minstd_rand gen;
    for(int i = 0; i < 50000; i++){
        of << "ADD" << " " << 1 << " " << gen() << '\n';
    }
    for(int i = 0; i < 50000; i++){
        of << "GO" << " " << 1 << " " << gen() << '\n';
    }
}

void Task4GenRead(const string& file){
    ofstream of(file);
    minstd_rand gen;
    of << MAX_Q_NUMBERS << '\n';
    for(int i = 0; i < MAX_Q_NUMBERS - MAX_PAGES; i++){
        of << "READ" << " " << gen() % MAX_USER_ID + 1 << " " << gen() % (MAX_PAGES - 1) << '\n';
    }
    for(int i = 0; i < MAX_PAGES; i++){
        of << "READ" << " " << gen() % MAX_USER_ID + 1 << " " << MAX_PAGES << '\n';
    }
}

void Task4GenReadAndCheer(const string& file){
    ofstream of(file);
    minstd_rand gen;
    of << MAX_Q_NUMBERS << '\n';
    for(int i = 0; i < MAX_Q_NUMBERS/2 - MAX_PAGES; i++){
        of << "READ" << " " << gen() % MAX_USER_ID + 1 << " " << gen() % (MAX_PAGES - 1) << '\n';
    }
    for(int i = 0; i < MAX_PAGES; i++){
        of << "READ" << " " << gen() % MAX_USER_ID + 1 << " " << MAX_PAGES << '\n';
    }
    for(int i = 0; i < MAX_Q_NUMBERS/2; i++){
        of << "CHEER" << " " << gen() % MAX_USER_ID + 1 << '\n';
    }
}

int main(){
    //Task3GenAddInputFile("inputADD.txt");
    //Task3GenAddAndGOInputFile("inputADDAndGO.txt");
    Task4GenRead("inputRead.txt");
    Task4GenReadAndCheer("inputReadAndCheer.txt");
    return 0;
}
