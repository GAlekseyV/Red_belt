#pragma once

#include <algorithm>
#include <cstdlib>

// Реализуйте шаблон SimpleVector
template<typename T>
class SimpleVector
{
public:
  SimpleVector() = default;

  explicit SimpleVector(size_t size)
    : data(new T[size]), size_(size), capacity_(size)
  {
  }

  SimpleVector(const SimpleVector &other)
    : data(new T[other.capacity_]), size_(other.size_), capacity_(other.capacity_)
  {
    std::copy(other.begin(), other.end(), begin());
  }

  ~SimpleVector()
  {
    delete[] data;
  }

  SimpleVector &operator=(const SimpleVector &rhs)
  {
    if (rhs.capacity_ <= capacity_) {
      std::copy(rhs.begin(), rhs.end(), begin());
      size_ = rhs.size_;
    } else {
      // Это так называемая идиома copy-and-swap.
      // Мы создаём временный вектор с помощью
      // конструктора копирования, а затем обмениваем его поля со своими.
      // Так мы достигаем двух целей:
      //  - избегаем дублирования кода в конструкторе копирования
      //    и операторе присваивания
      //  - обеспечиваем согласованное поведение конструктора копирования
      //    и оператора присваивания
      SimpleVector<T> tmp(rhs);
      std::swap(tmp.data, data);
      std::swap(tmp.size_, size_);
      std::swap(tmp.capacity_, capacity_);
    }
    return *this;
  }

  T &operator[](size_t index)
  {
    return data[index];
  }

  T *begin() { return data; }
  T *end() { return data + size_; }

  const T *begin() const { return data; }
  const T *end() const { return data + size_; }

  [[nodiscard]] size_t Size() const { return size_; }

  [[nodiscard]] size_t Capacity() const { return capacity_; }

  void PushBack(const T &value)
  {
    if (size_ >= capacity_) {
      auto new_cap = capacity_ == 0 ? 1 : 2 * capacity_;
      auto new_start_ = new T[new_cap];
      std::copy(begin(), end(), new_start_);
      delete[] data;
      data = new_start_;
      capacity_ = new_cap;
    }
    data[size_++] = value;
  }

private:
  // Поля для хранения данных вектора
  T *data = nullptr;
  size_t size_ = 0;
  size_t capacity_ = 0;
};
