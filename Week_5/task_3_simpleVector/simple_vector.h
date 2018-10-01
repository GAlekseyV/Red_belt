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

  explicit SimpleVector(size_t size)
  : start_(new T[size]), size_(size), capacity_(size){
  }

  SimpleVector(const SimpleVector& other)
  : start_(new T[other.capacity_]), size_(other.size_), capacity_(other.capacity_){
      copy(other.begin(), other.end(), begin());
  }

  ~SimpleVector(){
      delete[] start_;
  }

  SimpleVector& operator=(const SimpleVector& other){
      size_ = other.size_;
      capacity_ = other.capacity_;

      delete[] start_;
      start_ = new T[capacity_];
      std::copy(other.begin(), other.end(), begin());
      return *this;
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
    if(size_ >= capacity_){
        auto new_cap = capacity_ == 0 ? 1 : 2 * capacity_;
        auto new_start_ = new T[new_cap];
        std::copy(begin(), end(), new_start_);
        delete[] start_;
        start_ = new_start_;
        capacity_ = new_cap;
    }
    start_[size_++] = value;
  }

private:
  // Добавьте поля для хранения данных вектора

  T* start_;
  size_t size_;
  size_t capacity_;
};
