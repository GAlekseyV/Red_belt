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
    T* new_object;
    if(!free_objects.empty()){
      new_object = free_objects.front();
      selected_objects.insert(free_objects.front());
      free_objects.pop();
    }else{
      new_object = new T;
      selected_objects.insert(new_object);
    }
    return new_object;
  }

  T* TryAllocate(){
    // Работает аналогично Allocate, если освобожденных объектов нет
    // возвращает nullptr;
    T* new_object;
    if(!free_objects.empty()){
      new_object = free_objects.front();
      selected_objects.insert(free_objects.front());
      free_objects.pop();
    }else{
      new_object = nullptr;
    }
    return new_object;
  }

  void Deallocate(T* object){
    // Переносит объект из множества выделенных в множество освобождённых;
    // если переданный объект не содержится в множестве выделенных, метод Deallocate
    // должен бросать исключение invalid_argument.
    if(selected_objects.count(object) != 0){
      free_objects.push(object);
      selected_objects.erase(object);
    }else{
      throw invalid_argument("Object is not find");
    }
  }


  ~ObjectPool(){
    while(!selected_objects.empty()){
      auto deallocate_object = *selected_objects.begin();
      Deallocate(deallocate_object);
    }
    while(!free_objects.empty()){
      T* deleting_object = free_objects.front();
      free_objects.pop(); // Delete from queue and call destructor
      delete deleting_object;
    }
  }

private:
  // Добавьте сюда поля
  // Два набора объектов: выделенные и освобожденные
  set<T*> selected_objects;
  queue<T*> free_objects;
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
