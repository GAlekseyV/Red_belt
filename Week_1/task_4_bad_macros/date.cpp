//
// Created by GAlekseyV on 23.09.2018.
//

#include "date.h"

#include <iomanip>

ostream& operator<<(ostream& os, const Date& date) {
    os << setw(4) << setfill('0') << date.year << '-'
       << setw(2) << setfill('0') << date.month << '-'
       << setw(2) << setfill('0') << date.day;
    return os;
}