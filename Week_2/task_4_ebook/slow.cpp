#include "profile.h"

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <vector>
#include <set>
#include <utility>

using namespace std;

struct User{
    int user_id;
    int page_count;
};

bool operator<(const User& lhs, const User& rhs){
    return lhs.page_count < rhs.page_count;
}

class ReadingManager {
public:
  ReadingManager()
  : user_page_counts_(vector<int>(MAX_USER_COUNT_ + 1, -1))
  , count_users(0){}

  void Read(int user_id, int page_count) {
     //  Пользователя с id не существует
    if(user_page_counts_[user_id] == -1){
        user_page_counts_[user_id] = page_count;
        pages_to_user_ids[page_count].insert(user_id);
        count_users++;
    }else{
        pages_to_user_ids[user_page_counts_[user_id]].erase(user_id);
        user_page_counts_[user_id] = page_count;
        pages_to_user_ids[page_count].insert(user_id);
    }
  }

  double Cheer(int user_id) const {
      if (user_page_counts_[user_id] == -1) {
          return 0;
      }
      if (count_users == 1) {
          return 1;
      }
      // Всего пользователей
      // Указатель на первый итератор равный или больший заданному.
      auto it_greater = pages_to_user_ids.lower_bound(user_page_counts_[user_id]);
      int user_count_less_pages = 0;
      if(it_greater->first > pages_to_user_ids.size()/2){
          for(auto it = it_greater; it != pages_to_user_ids.end(); it++){
              user_count_less_pages += it->second.size();
          }
          user_count_less_pages = count_users - user_count_less_pages;
      }else {
          for (auto it = pages_to_user_ids.begin(); it != it_greater; ++it) {
              user_count_less_pages += it->second.size();
          }
      }
      return (user_count_less_pages) * 1.0 / (count_users - 1);
  }

private:
  // Статическое поле не принадлежит какому-то конкретному
  // объекту класса. По сути это глобальная переменная,
  // в данном случае константная.
  // Будь она публичной, к ней можно было бы обратиться снаружи
  // следующим образом: ReadingManager::MAX_USER_COUNT.
  static const int MAX_USER_COUNT_ = 100'000;
  static const int MAX_PAGES = 1000;
    int count_users;
  vector<int> user_page_counts_;
//  vector<int> sorted_users_;   // отсортированы по убыванию количества страниц
//  vector<int> user_positions_; // позиции в векторе sorted_users_

  //map<int, int> user_to_pages;
  map<int, set<int>> pages_to_user_ids;
  set<User> users;

//  int GetUserCount() const {
//      // Возвращает общее количество пользователей, для которых произошло событие Read
//    return sorted_users_.size();
//  }
//  void AddUser(int user_id) {
//    sorted_users_.push_back(user_id);
//    user_positions_[user_id] = sorted_users_.size() - 1;
//  }
//  void SwapUsers(int lhs_position, int rhs_position) {
//    const int lhs_id = sorted_users_[lhs_position];
//    const int rhs_id = sorted_users_[rhs_position];
//    swap(sorted_users_[lhs_position], sorted_users_[rhs_position]);
//    swap(user_positions_[lhs_id], user_positions_[rhs_id]);
//  }
};


int main1(){
  // Для ускорения чтения данных отключается синхронизация
  // cin и cout с stdio,
  // а также выполняется отвязка cin от cout
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  //ifstream is("inputReadAndCheer.txt");
  ifstream is("input_task_4.txt");
  ofstream os("output_task_4.txt");

  ReadingManager manager;

  int query_count;
  is >> query_count;
  LOG_DURATION("All time")
  for (int query_id = 0; query_id < query_count; ++query_id) {
    string query_type;
    is >> query_type;
    int user_id;
    is >> user_id;

    if (query_type == "READ") {
      int page_count;
      is >> page_count;
      manager.Read(user_id, page_count);
      //cout << "Query: " << query_id << endl;
    } else if (query_type == "CHEER") {
      os << setprecision(6) << manager.Cheer(user_id) << "\n";
    }
  }

  return 0;
}

int main(){
    // Для ускорения чтения данных отключается синхронизация
    // cin и cout с stdio,
    // а также выполняется отвязка cin от cout
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ReadingManager manager;

    int query_count;
    cin >> query_count;

    for (int query_id = 0; query_id < query_count; ++query_id) {
        string query_type;
        cin >> query_type;
        int user_id;
        cin >> user_id;

        if (query_type == "READ") {
            int page_count;
            cin >> page_count;
            manager.Read(user_id, page_count);
        } else if (query_type == "CHEER") {
            cout << setprecision(6) << manager.Cheer(user_id) << "\n";
        }
    }
    return 0;
}