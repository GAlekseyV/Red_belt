//
// Created by GAlekseyV on 24.09.2018.
//

#ifndef RED_BELT_DEQUE_H
#define RED_BELT_DEQUE_H

#include <stdexcept>
#include <vector>

using namespace std;
template <typename T>

class Deque{
public:
    T& Front(){
        return *prev(toPushFront.end());
    }
    const T& Front()const{
        return *prev(toPushFront.end());
    }
    T& Back(){
        return *prev(toPushBack.end());
    }
    const T& Back()const{
        return *prev(toPushBack.end());
    }

    T& At(size_t index){
        if(index < toPushFront.size()){
            return toPushFront.at(toPushFront.size() - index - 1);
        }else{
            if(index == toPushFront.size()){
                return toPushBack[index];
            }else{
                return toPushBack.at(index - toPushFront.size() + 1);
            }
        }
    }
    const T& At(size_t index) const{
        if(index < toPushFront.size()){
            return toPushFront.at(toPushFront.size() - index - 1);
        }else{
            if(index == toPushFront.size()){
                return toPushBack[index];
            }else{
                return toPushBack.at(index - toPushFront.size() + 1);
            }
        }
    }

    T& operator[](size_t index){
        if(index < toPushFront.size()){
            return toPushFront[toPushFront.size() - index - 1];
        }else{
            return toPushBack[index + 1 - toPushFront.size()];
        }
    }

    const T& operator[](size_t index) const{
        if(index < toPushFront.size()){
            return toPushFront[toPushFront.size() - index - 1];
        }else{
            return toPushBack[index + 1 - toPushFront.size()];
        }
    }

    bool Empty()const{
        return toPushFront.empty() && toPushBack.empty();
    }
    void PushBack(const T& item){
        if(Empty()){
            toPushFront.push_back(item);
            toPushBack.push_back(item);
        }else{
            toPushBack.push_back(item);
        }

    }
    void PushFront(const T& item){
        if(Empty()){
            toPushFront.push_back(item);
            toPushBack.push_back(item);
        }else{
            toPushFront.push_back(item);
        }
    }

    size_t Size()const{
        if(toPushFront.empty() || toPushBack.empty()){
            return toPushFront.size() + toPushBack.size();
        }
        return toPushFront.size() + toPushBack.size() - 1;
    }

private:
    vector<T> toPushFront;
    vector<T> toPushBack;
};

#endif //RED_BELT_DEQUE_H
