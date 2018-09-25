//
// Created by GAlekseyV on 25.09.2018.
//
// Вспомогательные функции
#include <iostream>
#include <fstream>
#include <random>
#include <string>
using namespace std;

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

int main(){
    //Task3GenAddInputFile("inputADD.txt");
    Task3GenAddAndGOInputFile("inputADDAndGO.txt");
    return 0;
}
