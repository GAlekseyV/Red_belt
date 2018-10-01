#include <algorithm>
#include <cstdint>

using namespace std;

// Реализуйте SimpleVector в этом файле
// и отправьте его на проверку

template <typename T>
class SimpleVector {
public:
    SimpleVector() = default;

    explicit SimpleVector(size_t size)
            : start_(new T[size]), size_(size), capacity_(size){
    }

    SimpleVector(const SimpleVector& other)
            : start_(new T[other.capacity_]), size_(other.size_), capacity_(other.capacity_){
        std::copy(other.begin(), other.end(), begin());
    }

    SimpleVector(SimpleVector&& other)
            : start_(new T[other.capacity_]), size_(other.size_), capacity_(other.capacity_){
        std::move(other.begin(), other.end(), begin());
    }

    ~SimpleVector(){
        delete[] start_;
    }

    SimpleVector& operator=(const SimpleVector& rhs){
        if(rhs.capacity_ <= capacity_){
            std::copy(rhs.begin(), rhs.end(), begin());
            size_ = rhs.size_;
        }else{
            // Это так называемая идиома copy-and-swap.
            // Мы создаём временный вектор с помощью
            // конструктора копирования, а затем обмениваем его поля со своими.
            // Так мы достигаем двух целей:
            //  - избегаем дублирования кода в конструкторе копирования
            //    и операторе присваивания
            //  - обеспечиваем согласованное поведение конструктора копирования
            //    и оператора присваивания
            SimpleVector<T> tmp(rhs);
            std::swap(tmp.start_, start_);
            std::swap(tmp.size_, size_);
            std::swap(tmp.capacity_, capacity_);
        }
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

    void PushBack(T&& value){
        if(size_ >= capacity_){
            auto new_cap = capacity_ == 0 ? 1 : 2 * capacity_;
            auto new_start_ = new T[new_cap];
            std::move(begin(), end(), new_start_);
            delete[] start_;
            start_ = new_start_;
            capacity_ = new_cap;
        }
        start_[size_++] = move(value);
    }

private:
    // Поля для хранения данных вектора
    T* start_ = nullptr;
    size_t size_ = 0;
    size_t capacity_ = 0;
};
