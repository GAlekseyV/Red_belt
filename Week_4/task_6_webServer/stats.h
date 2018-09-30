#pragma once

#include "http_request.h"

#include <string_view>
#include <map>
#include <array>
using namespace std;

class StatPiece{
public:
    template <typename Container>
    StatPiece(const Container& known_keys, string_view default_key)
    : default_key(default_key){
      counts[default_key] = 0;
      for(string_view key : known_keys){
        counts[key] = 0;
      }
    }
    void Add(string_view value);

    const map<string_view, int>& GetValues() const{
      return counts;
    }

private:
    const string_view default_key;
    map<string_view, int> counts;

};

class Stats {
public:
  Stats() = default;
  //Реализовать методы класса
  void AddMethod(string_view method);
  void AddUri(string_view uri);
  const map<string_view, int>& GetMethodStats() const;
  const map<string_view, int>& GetUriStats() const;

private:
  const array<string, 4> known_methonds = {"GET", "POST", "DELETE", "PUT"};
  const string default_method = "UNKNOWN";
  const array<string, 5> known_uris = {"/", "/product", "/basket", "/help", "/order"};
  const string default_uri = "unknown";

  StatPiece methods{known_methonds, default_method};
  StatPiece uris{known_uris, default_uri};
};

// Реализовать функцию
HttpRequest ParseRequest(string_view line);
