//
// Created by GAlekseyV on 24.09.2018.
//
#include <iostream>
#include <vector>
#include <set>
#include <deque>
#include <utility>
using namespace std;

template <typename Iterator>
class IteratorRange{
private:
    Iterator first, last;
public:
    // Конструктор для автоматического вывода типа. Работает с С++1z
    IteratorRange(Iterator f, Iterator l)
    : first(f)
    , last(l){
    }

    Iterator begin() const{
        return first;
    }
    Iterator end() const{
        return last;
    }
};

// Итерирование по префиксу произвольного контейнера
template <typename Container>
auto Head(Container& v, size_t top){
    return IteratorRange{v.begin(), next(v.begin(), min(top, v.size()))};
}

// Порождающая функция для IteratorRange
//template <typename Iterator>
//IteratorRange<Iterator> MakeRange(Iterator begin, Iterator end){
//    return IteratorRange<Iterator>{begin, end};
//}

int main(){
    vector<int> v = {1, 2, 3, 4, 5};
    for(int& x : Head(v, 3)){
        x++;
    }
    for(int x : v){
        cout << x << ' ';
    }
    cout << endl;
    IteratorRange second_half(v.begin() + v.size()/2, v.end());
    for(int x : second_half){
        cout << x << ' ';
    }
    cout << endl;

    set<int> nums = {5, 7, 12, 8, 10, 5, 6, 1};
    for(int x : Head(nums, 4)){
        cout << x << ' ';
    }
    cout << endl;

    const deque<int> nums2 = {5, 7, 12, 8, 10, 5, 6, 1};
    for(int x : Head(nums2, 4)){
        cout << x << ' ';
    }
    cout << endl;
}
