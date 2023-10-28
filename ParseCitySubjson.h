#pragma once
#include <vector>
#include <string>
#include <variant>

using namespace std::literals;

struct Language {

};

struct City {
    std::string name;
    std::string iso_code;
    std::string city_phone_code;
    std::string country_name;
    std::string country_iso_code;
    std::string country_time_zone;
    std::vector<Language> language;

};




struct  Country {
    std::string name;
    std::string iso_code;
    std::string phone_code;
    std::string time_zone;
    std::vector<Language> languages;
};




class Json {
public:
    std::vector<Json> AsList() const  {
        return {};
    };
    
    Json AsObject() const {
        return {};
    }

    Json& operator[](std::string key) const {

    }

    std::string AsString() const {

    }
};



template<typename T>
T FromString(std::string input) {
    return {};
}


class CityBuilder {
public:

    CityBuilder& SetName(std::string name) {
        name_ = name;
    }

    CityBuilder& SetIsoCode(std::string iso_code) {
        iso_code_ = iso_code;
    }

    CityBuilder& SetPhoneCode(std::string city_phone_code) {
        city_phone_code_ = city_phone_code;
    }

    CityBuilder& SetCountryPhoneCode(std::string country_phone_code) {
        country_phone_code_ = country_phone_code;
    }

    CityBuilder& SetCountryName(std::string country_name) {
        country_name_ = country_name;
    }

    CityBuilder& SetCountryIsoCode(std::string country_iso_code) {
        country_iso_code_ = country_iso_code;
    }

    CityBuilder& SetCountryTimeZone(std::string country_time_zone) {
        country_time_zone_ = country_time_zone;
    }

    CityBuilder& SetLanguages(std::vector<Language> languages) {
        languages_ = languages;
    }

    City Build() {
        return {
            name_,
            iso_code_,
            country_phone_code_ + city_phone_code_,
            country_name_,
            country_iso_code_,
            country_time_zone_,
            languages_
        };
    }

private:
    std::string name_;
    std::string iso_code_;
    std::string country_phone_code_;
    std::string city_phone_code_;
    std::string country_name_;
    std::string country_iso_code_;
    std::string country_time_zone_;
    std::vector<Language> languages_;

};


// Дана функция ParseCitySubjson, обрабатывающая JSON-объект со списком городов конкретной страны:
void ParseCitySubjson(std::vector<City>& cities, const Json& json, CityBuilder& builder) {

    for (const auto& city_json : json.AsList()) {

        const auto& city_obj = city_json.AsObject();

        cities.push_back(
            builder
            .SetName(city_obj["name"s].AsString())
            .SetIsoCode(city_obj["iso_code"s].AsString())
            .SetPhoneCode(city_obj["phone_code"s].AsString())
            .Build());
    }
}

// ParseCitySubjson вызывается только из функции ParseCountryJson следующим образом:
void ParseCountryJson(
    std::vector<Country>& countries,
    std::vector<City>& cities,
    const Json& json) {
    for (const auto& country_json : json.AsList()) {

        const auto& country_obj = country_json.AsObject();

        countries.push_back({
            country_obj["name"s].AsString(),
            country_obj["iso_code"s].AsString(),
            country_obj["phone_code"s].AsString(),
            country_obj["time_zone"s].AsString(),
            });

        Country& country = countries.back();

        for (const auto& lang_obj : country_obj["languages"s].AsList()) {
            country.languages.push_back(FromString<Language>(lang_obj.AsString()));
        }

        CityBuilder builder;
        builder.SetCountryName(country.name)
               .SetCountryIsoCode(country.iso_code)
               .SetCountryPhoneCode(country.phone_code)
               .SetCountryTimeZone(country.time_zone)
               .SetLanguages(country.languages);


        ParseCitySubjson(
            cities, 
            country_obj["cities"s], 
            builder);
    }
}

