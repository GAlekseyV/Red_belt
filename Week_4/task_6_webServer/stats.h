#pragma once

#include "http_request.h"

#include <string_view>
#include <map>
#include <vector>
using namespace std;

class Stats {
public:
  Stats();
  //Реализовать методы класса
  void AddMethod(string_view method);
  void AddUri(string_view uri);
  const map<string_view, int>& GetMethodStats() const;
  const map<string_view, int>& GetUriStats() const;


private:
  const vector<string> methodNames;
  const vector<string> uriNames;

  map<string_view, int> methodStats;
  map<string_view, int> uriStats;
};

// Реализовать функцию
HttpRequest ParseRequest(string_view line);
