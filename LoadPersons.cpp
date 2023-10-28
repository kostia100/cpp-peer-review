#include "LoadPersons.h"
#include <sstream>


using namespace std::literals;

struct ConnectionParameters {
    std::string_view db_name;
    int db_connection_timeout;
    bool db_allow_exceptions;
    DBLogLevel db_log_level;
};

class ConnectionParametersBuilder {
public:
    ConnectionParametersBuilder& SetDBName(std::string_view name) {
        db_name_ = name;
    }

    ConnectionParametersBuilder& SetDBTimeOut(int timeout) {
        db_connection_timeout_ = timeout;
    }

    ConnectionParametersBuilder& SetDBExceptions(bool allow_exceptions) {
        db_allow_exceptions_ = allow_exceptions;
    }

    ConnectionParametersBuilder& SetDBLogLevel(DBLogLevel db_log_level) {
        db_log_level_ = db_log_level;
    }

    operator ConnectionParameters() const {
        return { db_name_ ,db_connection_timeout_ , db_allow_exceptions_  , db_log_level_ };
    }

private:
    std::string_view db_name_;
    int db_connection_timeout_;
    bool db_allow_exceptions_;
    DBLogLevel db_log_level_;
};


struct FilterPersons {
    int min_age;
    int max_age;
    std::string_view name_filter;
};

class FilterBuilder {
public:
    FilterBuilder& SetMinAge(int min_age) {
        min_age_ = min_age;
    }

    FilterBuilder& SetMaxAge(int max_age) {
        max_age_ = max_age;
    }

    FilterBuilder& SetFilterNameAge(std::string_view name_fileter) {
        name_filter_ = name_fileter;
    }

    operator FilterPersons() const {
        return { min_age_ ,max_age_,name_filter_ };
    }

private:
    int min_age_;
    int max_age_;
    std::string_view name_filter_;
};


std::vector<Person> LoadPersons(ConnectionParameters connection, FilterPersons filter) {
    
    DBConnector connector(connection.db_allow_exceptions, connection.db_log_level);
    DBHandler db;
    if (connection.db_name.starts_with("tmp."s)) {
        db = connector.ConnectTmp(connection.db_name, connection.db_connection_timeout);
    }
    else {
        db = connector.Connect(connection.db_name, connection.db_connection_timeout);
    }
    if (!connection.db_allow_exceptions && !db.IsOK()) {
        return {};
    }

    std::ostringstream query_str;
    query_str << "from Persons "s
        << "select Name, Age "s
        << "where Age between "s << filter.min_age << " and "s << filter.max_age << " "s
        << "and Name like '%"s << db.Quote(filter.name_filter) << "%'"s;
    DBQuery query(query_str.str());

    std::vector<Person> persons;
    for (auto [name, age] : db.LoadRows<std::string, int>(query)) {
        persons.push_back({ std::move(name), age });
    }
    return persons;
    
}

