//
// Created by GAlekseyV on 23.09.2018.
//

#ifndef RED_BELT_DATE_H
#define RED_BELT_DATE_H

#include <iostream>
using namespace std;

struct Date {
    int year, month, day;
};

ostream& operator<<(ostream& os, const Date& d);
#endif //RED_BELT_DATE_H
