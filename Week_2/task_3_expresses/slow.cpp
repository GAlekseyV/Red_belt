#include "profile.h"

#include <algorithm>
#include <cmath>
#include <iostream>
#include <map>
#include <string>
#include <set>
#include <vector>
#include <fstream>

using namespace std;

class RouteManager {
public:
  void AddRoute(int start, int finish) {
    //reachable_lists_[start].push_back(finish);
    //reachable_lists_[finish].push_back(start);

    reachable_lists_set[start].insert(finish);
    reachable_lists_set[finish].insert(start);
  }

  int FindNearestFinish(int start, int finish) const {
    int result = abs(start - finish);
    if (reachable_lists_set.count(start) < 1) {
        return result;
    }
    const set<int>& reachable_stations = reachable_lists_set.at(start); // O(logN)
      const auto finish_pos = reachable_stations.lower_bound(finish);
        if(finish_pos != reachable_stations.end()){
            // Все элементы меньше заданного
            result = min(result, abs(*finish_pos - finish));
        }
        if(finish_pos != reachable_stations.begin()) {
            result = min(result, abs(*prev(finish_pos) - finish));
        }
    return result;
  }
private:
  //map<int, vector<int>> reachable_lists_;
  map<int, set<int>> reachable_lists_set;
};


int main() {
    RouteManager routes;
    ifstream in("inputADDAndGO.txt");
    ofstream of("out.txt");
    int query_count;
    in >> query_count;
    LOG_DURATION("Time")
    for (int query_id = 0; query_id < query_count; ++query_id) {
        string query_type;
        in >> query_type;
        int start, finish;
        in >> start >> finish;
        if (query_type == "ADD") {
          routes.AddRoute(start, finish);
        } else if (query_type == "GO") {
            //routes.FindNearestFinish(start, finish);
          of << routes.FindNearestFinish(start, finish) << "\n";
        }
    }


    return 0;
}

int main1(){
    RouteManager routes;

    int query_count;
    cin >> query_count;

    for (int query_id = 0; query_id < query_count; ++query_id) {
        string query_type;
        cin >> query_type;
        int start, finish;
        cin >> start >> finish;
        if (query_type == "ADD") {
            routes.AddRoute(start, finish);
        } else if (query_type == "GO") {
            cout << routes.FindNearestFinish(start, finish) << "\n";
        }
    }
    return 0;
}
