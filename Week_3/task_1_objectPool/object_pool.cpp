#include "test_runner.h"

#include <queue>
#include <set>
#include <stdexcept>
#include <string>

using namespace std;

template<class T>
class ObjectPool
{
public:
  T *Allocate()
  {
    // Если есть хотя бы один освобожденный объект, перености его
    // во множество выделенных и возвращает указатель.
    // Если освобожденных объектов нет, создает новый объект и помещает
    // его во множество выделенных, возвращает указатель на него
    if (free.empty()) {
      free.push(new T);
    }
    auto *new_object = free.front();
    free.pop();
    allocated.insert(new_object);
    return new_object;
  }

  T *TryAllocate()
  {
    // Работает аналогично Allocate, если освобожденных объектов нет
    // возвращает nullptr;
    T *new_object = nullptr;
    if (free.empty()) {
      return nullptr;
    }
    return Allocate();
  }

  void Deallocate(T *object)
  {
    // Переносит объект из множества выделенных в множество освобождённых;
    // если переданный объект не содержится в множестве выделенных, метод Deallocate
    // должен бросать исключение invalid_argument.
    if (allocated.count(object) != 0) {
      free.push(object);
      allocated.erase(object);
    } else {
      throw invalid_argument("Object is not find");
    }
  }


  ~ObjectPool()
  {
    for (auto *x : allocated) {
      delete x;
    }
    while (!free.empty()) {
      T *deleting_object = free.front();
      free.pop();// Delete from queue and call destructor
      delete deleting_object;
    }
  }

private:
  // Добавьте сюда поля
  // Два набора объектов: выделенные и освобожденные
  set<T *> allocated;
  queue<T *> free;
};

void TestObjectPool()
{
  ObjectPool<string> pool;

  auto *ptr1 = pool.Allocate();
  auto *ptr2 = pool.Allocate();
  auto *ptr3 = pool.Allocate();

  *ptr1 = "first";
  *ptr2 = "second";
  *ptr3 = "third";

  pool.Deallocate(ptr2);
  ASSERT_EQUAL(*pool.Allocate(), "second");

  pool.Deallocate(ptr3);
  pool.Deallocate(ptr1);
  ASSERT_EQUAL(*pool.Allocate(), "third");
  ASSERT_EQUAL(*pool.Allocate(), "first");

  pool.Deallocate(ptr1);
}

int main()
{
  TestRunner trunner;
  RUN_TEST(trunner, TestObjectPool);
  return 0;
}
