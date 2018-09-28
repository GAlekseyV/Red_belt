#include "test_runner.h"

#include <algorithm>
#include <iostream>
#include <string>
#include <queue>
#include <stdexcept>
#include <set>

using namespace std;

template <class T>
class ObjectPool {
public:
  T* Allocate(){
    // Если есть хотя бы один освобожденный объект, перености его
    // во множество выделенных и возвращает указатель.
    // Если освобожденных объектов нет, создает новый объект и помещает
    // его во множество выделенных, возвращает указатель на него
    if(free.empty()){
      free.push(new T);
    }
    auto new_object = free.front();
    free.pop();
    allocated.insert(new_object);
    return new_object;
  }

  T* TryAllocate(){
    // Работает аналогично Allocate, если освобожденных объектов нет
    // возвращает nullptr;
    T* new_object;
    if(free.empty()){
      return nullptr;
    }
    return Allocate();
  }

  void Deallocate(T* object){
    // Переносит объект из множества выделенных в множество освобождённых;
    // если переданный объект не содержится в множестве выделенных, метод Deallocate
    // должен бросать исключение invalid_argument.
    if(allocated.count(object) != 0){
      free.push(object);
      allocated.erase(object);
    }else{
      throw invalid_argument("Object is not find");
    }
  }


  ~ObjectPool(){
    for(auto x : allocated){
      delete x;
    }
    while(!free.empty()){
      T* deleting_object = free.front();
      free.pop(); // Delete from queue and call destructor
      delete deleting_object;
    }
  }

private:
  // Добавьте сюда поля
  // Два набора объектов: выделенные и освобожденные
  set<T*> allocated;
  queue<T*> free;
};

void TestObjectPool() {
  ObjectPool<string> pool;

  auto p1 = pool.Allocate();
  auto p2 = pool.Allocate();
  auto p3 = pool.Allocate();

  *p1 = "first";
  *p2 = "second";
  *p3 = "third";

  pool.Deallocate(p2);
  ASSERT_EQUAL(*pool.Allocate(), "second");

  pool.Deallocate(p3);
  pool.Deallocate(p1);
  ASSERT_EQUAL(*pool.Allocate(), "third");
  ASSERT_EQUAL(*pool.Allocate(), "first");

  pool.Deallocate(p1);
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestObjectPool);
  return 0;
}
