//
// Created by GAlekseyV on 02.10.2018.
//
#include "profile.h"

#include <algorithm>
#include <iostream>
#include <iomanip>
#include <vector>
#include <future>
#include <mutex>

using namespace std;

struct Account{
    int balance = 0;
    vector<int> history;
    mutex m;

    // Критическая секция
    bool Spend(int value){
        lock_guard<mutex> g(m); // Защита критической секции
        if(value <= balance){
            balance -= value;
            // Возможность релокации из нескольких потоков
            history.push_back(value);
            return true;
        }
        return false;
    }
};

int SpendMoney(Account& account){
    int total_spent = 0;
    for(int i = 0; i < 100000; ++i){
        if(account.Spend(1)){
            ++total_spent;
        }
    }
    return total_spent;
}

int main(){
    Account family_account{100000};
    //
    // В функцию передана функция (имя функции, ref(параметры по ссылке)
    auto husband = async(SpendMoney, ref(family_account));
    auto wife = async(SpendMoney, ref(family_account));
    auto son = async(SpendMoney, ref(family_account));
    auto daughter = async(SpendMoney, ref(family_account));

    int spent = husband.get() + wife.get() + son.get() + daughter.get();

    cout << "Total spent: " << spent << '\n'
         << "Balance: " << family_account.balance << endl;
    return 0;
}
