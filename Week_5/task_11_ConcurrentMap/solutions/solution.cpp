#include "profile.h"
#include "test_runner.h"

#include <algorithm>
#include <future>
#include <mutex>
#include <numeric>
#include <random>
#include <string>
#include <vector>
using namespace std;

template<typename K, typename V>
class ConcurrentMap
{
public:
  static_assert(is_integral_v<K>, "ConcurrentMap supports only integer keys");

  struct Access
  {
    lock_guard<mutex> lock;
    V &ref_to_value;
  };

  explicit ConcurrentMap(size_t bucket_count)
    : bucket_count_(bucket_count), buckets(vector<map<K, V>>(bucket_count_)), mutexes(vector<mutex>(bucket_count_))
  {
  }

  Access operator[](const K &key)
  {
    // Buckets are numbered by int values 0, 1, ..., n - 1,
    // where n is the bucket count.
    // Key k belongs to bucket b if k % n == b.
    size_t bucket_index = key % bucket_count_;

    // key may be less than zero, in which case *mod* division
    // may give a negative number for some compilers
    // therefore additional check is needed
    if (bucket_index < 0) {
      bucket_index += bucket_count_;
    }

    // Order of members of Access struct is important.
    // Lock guard should be first to lock data before accessing them in constructor
    return {
      lock_guard<mutex>(mutexes[bucket_index]),
      buckets[bucket_index][key]
    };
  }

  map<K, V> BuildOrdinaryMap()
  {
    map<K, V> result;
    for (size_t i = 0; i < bucket_count_; ++i) {
      lock_guard<mutex> lock(mutexes[i]);
      result.insert(buckets[i].begin(), buckets[i].end());
    }
    return result;
  }

private:
  size_t bucket_count_;
  vector<map<K, V>> buckets;
  vector<mutex> mutexes;
};
