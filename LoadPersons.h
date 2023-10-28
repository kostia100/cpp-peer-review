#pragma once

#include <vector>
#include <string_view>



struct  Person {
    std::string name_;
    int age_;

};
class DBLogLevel {

};

class DBQuery {
public:
    DBQuery(std::string query) {

    }
};


class DBHandler {
public:

    DBHandler() {

    }

    bool IsOK() {
        return false;
    }

    std::string Quote(std::string_view name_filter) {
        return "";
    }

    template <typename A, typename B>
    std::vector<Person> LoadRows(DBQuery query);


};


template <typename A, typename B>
std::vector<Person> DBHandler::LoadRows(DBQuery query) {
    return {};
}

class DBConnector {
public:
    DBConnector(bool db_allow_exceptions, DBLogLevel db_log_level) {

    }

    DBHandler ConnectTmp(std::string_view db_name, int db_connection_timeout) {
        return {};
    }

    DBHandler Connect(std::string_view db_name, int db_connection_timeout) {
        return {};
    }

};






// OLD VERSION
std::vector<Person> LoadPersons(std::string_view db_name, int db_connection_timeout, bool db_allow_exceptions,
    DBLogLevel db_log_level, int min_age, int max_age, std::string_view name_filter);


struct ConnectionParameters;
struct FilterPersons;

//NEW VERSION, implementation in LoadPersons.cpp
std::vector<Person> LoadPersons(ConnectionParameters connection, FilterPersons filter);

