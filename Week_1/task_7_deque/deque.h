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
    Deque() = default;

    T& Front(){
        return head.empty() ? tail.front() : head.back();
    }
    const T& Front()const{
        return head.empty() ? tail.front() : head.back();
    }
    T& Back(){
        return tail.empty() ? head.front() : tail.back();
    }
    const T& Back()const{
        return tail.empty() ? head.front() : tail.back();
    }

    T& At(size_t index){
        CheckIndex(index);
        return (*this)[index];
    }
    const T& At(size_t index) const{
        CheckIndex(index);
        return (*this)[index];
    }

    T& operator[](size_t index){
        if(index < head.size()){
            return head[head.size() - index - 1];
        }else{
            return tail[index - head.size()];
        }
    }

    const T& operator[](size_t index) const{
        if(index < head.size()){
            return head[head.size() - index - 1];
        }else{
            return tail[index - head.size()];
        }
    }

    bool Empty()const{
        return head.empty() && tail.empty();
    }

    void Clear(){
        head.clear();
        tail.clear();
    }

    void PushBack(const T& item){
        tail.push_back(item);
    }
    void PushFront(const T& item){
        head.push_back(item);
    }

    size_t Size()const{
        return head.size() + tail.size();
    }

private:
    void CheckIndex(size_t i){
        if(i >= Size()){
            throw out_of_range("Index is out of range");
        }
    }

    vector<T> head;
    vector<T> tail;
};

#endif //RED_BELT_DEQUE_H
