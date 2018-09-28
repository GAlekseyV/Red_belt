#pragma once

#include <algorithm>
#include <cstdlib>

// Реализуйте шаблон SimpleVector
template <typename T>
class SimpleVector {
public:
  SimpleVector()
  : start_(nullptr), size_(0), capacity_(0)
  {}

  explicit SimpleVector(size_t size): size_(size){
    start_ = new T[size_];
    capacity_ = size_;
  }

  ~SimpleVector(){
      delete[] start_;
  }

  T& operator[](size_t index){
      return start_[index];
  }

  T* begin(){return start_;}
  T* end(){return start_ + size_;}

  const T* begin() const {return start_;}
  const T* end() const {return start_ + size_;}

  size_t Size() const {return size_;}

  size_t Capacity() const {return capacity_;}

  void PushBack(const T& value){
      if(start_ == nullptr){
          start_ = new T[1];
          *start_ = value;
          size_++;
          capacity_++;
          return;
      }
      if(size_ == capacity_){
          Resize();
      }
      size_++;
      start_[size_ - 1] = value;
  }

private:
  // Добавьте поля для хранения данных вектора
  void Resize(){
      capacity_ = 2*capacity_;
      T* new_start = new T[capacity_];
      std::copy(start_, start_ + size_, new_start);
      delete[] start_;
      start_ = new_start;
  }

  T* start_;
  size_t size_;
  size_t capacity_;
};
