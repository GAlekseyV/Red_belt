/*
 * Разработайте систему бронирования отелей, позволяющую бронировать номера клиентами и
 * контролировать спрос владельцами отелей.
 * https://www.coursera.org/learn/c-plus-plus-red/programming/8O9dd/sistiema-bronirovaniia-otieliei
 */
#include "profile.h"
#include "test_runner.h"

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <limits>
#include <map>
#include <set>
#include <string>

using namespace std;

struct Request{
    string type;
    int64_t time;
    string hotel_name;
    int client_id;
    int room_count;
};


class BookingManager{
public:
    BookingManager()
    : last_booking_time(numeric_limits<int64_t>::min())
    {}
    void Book(const int64_t& time, const string& hotel_name, const int client_id, const int room_count){
        // Забронировать номер в отеле
        time_to_clients[hotel_name][time].insert(client_id);
        time_to_rooms[hotel_name][time] += room_count;
        if(time > last_booking_time){
            last_booking_time = time;
        }
    }

    int Clients(const string& hotel_name) const{
        // Количество клиентов бронировавших номера
        if(time_to_clients.count(hotel_name) != 0){
            auto first_book_it = time_to_clients.at(hotel_name).upper_bound(last_booking_time - seconds_in_day);
            set<int> total_clients;
            for(auto it = first_book_it; it != time_to_clients.at(hotel_name).end(); ++it){
                total_clients.insert(it->second.begin(), it->second.end());
            }
            return total_clients.size();
        }
        return 0;
    }

    int Rooms(const string& hotel_name) const{
        // Возвращает количество номеров забронированных за сутки
        if(time_to_rooms.count(hotel_name) != 0){
            auto first_book_it = time_to_rooms.at(hotel_name).upper_bound(last_booking_time - seconds_in_day);
            int total_rooms_booked = 0;
            for(auto it = first_book_it; it != time_to_rooms.at(hotel_name).end(); ++it){
                total_rooms_booked += it->second;
            }
            return total_rooms_booked;
        }
        return 0;
    }

private:
    static const int64_t seconds_in_day = 86400;
    int64_t last_booking_time;

    map<string, map<int64_t , set<int>>> time_to_clients;
    map<string, map<int64_t , int>> time_to_rooms;
};

void TestCase1();
void TestCase2();
void TestCase3Negative();
void TestDiffClients();

int main1(){
    TestRunner runner;
    RUN_TEST(runner, TestCase1);
    RUN_TEST(runner, TestCase2);
    RUN_TEST(runner, TestCase3Negative);
    RUN_TEST(runner, TestDiffClients);
    // Для ускорения чтения данных отключается синхронизация
    // cin и cout с stdio,
    // а также выполняется отвязка cin от cout
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    ifstream is("inputTask5Test.txt");
    //ifstream is("input_task_4.txt");
    ofstream os("outputTask5.txt");

    BookingManager manager;

    int query_count;
    is >> query_count;
    Request request;
    LOG_DURATION("All time")
    for (int query_id = 0; query_id < query_count; ++query_id) {
        is >> request.type;
        if (request.type == "BOOK") {
            is >> request.time >> request.hotel_name >> request.client_id >> request.room_count;
            manager.Book(request.time, request.hotel_name, request.client_id, request.room_count);
        } else if (request.type == "CLIENTS") {
            is >> request.hotel_name;
            os << manager.Clients(request.hotel_name) << "\n";
        }else if(request.type == "ROOMS"){
            is >> request.hotel_name;
            os << manager.Rooms(request.hotel_name) << "\n";
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

    BookingManager manager;

    int query_count;
    cin >> query_count;
    Request request;
    LOG_DURATION("All time")
    for (int query_id = 0; query_id < query_count; ++query_id) {
        cin >> request.type;
        if (request.type == "BOOK") {
            cin >> request.time >> request.hotel_name >> request.client_id >> request.room_count;
            manager.Book(request.time, request.hotel_name, request.client_id, request.room_count);
        } else if (request.type == "CLIENTS") {
            cin >> request.hotel_name;
            cout << manager.Clients(request.hotel_name) << "\n";
        }else if(request.type == "ROOMS"){
            cin >> request.hotel_name;
            cout << manager.Rooms(request.hotel_name) << "\n";
        }
    }

    return 0;
}


void TestCase1(){
    BookingManager manager;

    ASSERT_EQUAL(manager.Clients("Marriott"), 0);
    ASSERT_EQUAL(manager.Rooms("Marriott"), 0);

    manager.Book(10, "FourSeasons", 1, 2);
    manager.Book(10, "Marriott", 1, 1);
    manager.Book(86409, "FourSeasons", 2, 1);

    ASSERT_EQUAL(manager.Clients("FourSeasons"), 2);
    ASSERT_EQUAL(manager.Rooms("FourSeasons"), 3);
    ASSERT_EQUAL(manager.Clients("Marriott"), 1);

    manager.Book(86410, "Marriott", 2, 10);
    ASSERT_EQUAL(manager.Rooms("FourSeasons"), 1);
    ASSERT_EQUAL(manager.Rooms("Marriott"), 10);
};

void TestCase2(){
    BookingManager manager;

    // Test empty bookings
    ASSERT_EQUAL(manager.Clients("mariott"), 0);
    ASSERT_EQUAL(manager.Rooms("mariott"), 0);

    manager.Book(0, "mariott", 1, 10);
    manager.Book(0, "mariott", 2, 1);
    manager.Book(0, "mariott", 3, 1);
    manager.Book(0, "mariott", 4, 1);
    manager.Book(0, "hilton", 1, 1);
    manager.Book(1, "hilton", 2, 1);

    // Test correctness
    ASSERT_EQUAL(manager.Clients("mariott"), 4);
    ASSERT_EQUAL(manager.Rooms("mariott"), 13);
    ASSERT_EQUAL(manager.Clients("hilton"), 2);
    ASSERT_EQUAL(manager.Rooms("hilton"), 2);

    // Test event past 1 day resets statics
    manager.Book(86400, "mariott", 1, 1);
    ASSERT_EQUAL(manager.Clients("mariott"), 1);
    ASSERT_EQUAL(manager.Rooms("mariott"), 1);
    ASSERT_EQUAL(manager.Clients("hilton"), 1);
    ASSERT_EQUAL(manager.Rooms("hilton"), 1);

    // Test no clients and room for the last day
    manager.Book(86401, "mariott", 5, 1);
    ASSERT_EQUAL(manager.Clients("mariott"), 2);
    ASSERT_EQUAL(manager.Rooms("mariott"), 2);
    ASSERT_EQUAL(manager.Clients("hilton"), 0);
    ASSERT_EQUAL(manager.Rooms("hilton"), 0);

    manager.Book(1'000'000'000'000'000'000, "mariott", 1000'000'000, 1000);
    ASSERT_EQUAL(manager.Clients("mariott"), 1);
    ASSERT_EQUAL(manager.Rooms("mariott"), 1000);

};

void TestCase3Negative(){
    BookingManager manager;

    ASSERT_EQUAL(manager.Clients("Marriott"), 0);
    ASSERT_EQUAL(manager.Rooms("Marriott"), 0);

    manager.Book(-100'000, "FourSeasons", 1, 2);
    manager.Book(-100'000, "Marriott", 1, 1);
    manager.Book(-80'000, "FourSeasons", 2, 1);

    ASSERT_EQUAL(manager.Clients("FourSeasons"), 2);
    ASSERT_EQUAL(manager.Rooms("FourSeasons"), 3);
    ASSERT_EQUAL(manager.Clients("Marriott"), 1);

    manager.Book(-10'000, "Marriott", 2, 10);
    ASSERT_EQUAL(manager.Rooms("FourSeasons"), 1);
    ASSERT_EQUAL(manager.Rooms("Marriott"), 10);
};

void TestDiffClients(){
    BookingManager manager;

    manager.Book(10, "FourSeasons", 1, 20);
    manager.Book(10, "Marriott", 1, 1);
    manager.Book(20, "FourSeasons", 1, 10);
    manager.Book(30, "FourSeasons", 2, 1);
    manager.Book(86409, "FourSeasons", 1, 10);

    ASSERT_EQUAL(manager.Clients("FourSeasons"), 2);
    ASSERT_EQUAL(manager.Rooms("FourSeasons"), 41);
    ASSERT_EQUAL(manager.Clients("Marriott"), 1);

    manager.Book(86500, "Marriott", 2, 10);
    ASSERT_EQUAL(manager.Rooms("FourSeasons"), 10);
    ASSERT_EQUAL(manager.Rooms("Marriott"), 10);
}