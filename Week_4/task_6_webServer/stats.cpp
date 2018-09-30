//
// Created by GAlekseyV on 30.09.2018.
//

#include "stats.h"

Stats::Stats()
: methodNames({"GET", "POST", "DELETE", "PUT", "UNKNOWN"})
, uriNames({"/", "/order", "/product", "/basket", "/help", "unknown"})
{
    for(auto& method : methodNames){
        methodStats[method] = 0;
    }
    for(auto& uri : uriNames){
        uriStats[uri] = 0;
    }
}

void Stats::AddMethod(string_view method) {
    if(methodStats.count(method) > 0){
        ++methodStats[method];
    }else{
        ++methodStats[methodNames.back()];
    }
}

void Stats::AddUri(string_view uri) {
    if(uriStats.count(uri) > 0){
        ++uriStats[uri];
    }else{
        ++uriStats[uriNames.back()];
    }
}

const map<string_view, int>& Stats::GetMethodStats() const {
    return methodStats;
}

const map<string_view, int>& Stats::GetUriStats() const {
    return uriStats;
}

HttpRequest ParseRequest(string_view line){
    HttpRequest parseRequest;
    size_t pos = 0;
    while(line[pos] == ' '){
        pos++;
    }
    size_t space = line.find(' ', pos);
    parseRequest.method = line.substr(pos, space - pos);
    pos = space + 1;
    space = line.find(' ', pos);
    parseRequest.uri = line.substr(pos, space - pos);
    pos = space + 1;
    parseRequest.protocol = line.substr(pos);

    return parseRequest;
}
