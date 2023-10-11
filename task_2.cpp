#include <algorithm>
#include <iostream>
#include <string>
#include <sstream>
#include <string_view>
#include <vector>
#include <unordered_set>
#include <cassert>
#include <algorithm>

using namespace std;

class Domain {
public:
    // разработайте класс домена

    // конструктор должен позволять конструирование из string, с сигнатурой определитесь сами
    Domain(string domain) : domain_(domain) {
        reverse(domain_.begin(),domain_.end());
    }


    // разработайте метод IsSubdomain, принимающий другой домен и возвращающий true, если this его поддомен
    bool IsSubdomain(const Domain& other) const {
        if (other.domain_.size() > this->domain_.size()) {
            //this must be longer to be a sub-domain
            return false;
        }

        if (other.domain_.size() == this->domain_.size()) {
            return other.domain_ == this->domain_;
        }

        int q = 0;
        for (char c : other.domain_) {
            if (c != domain_[q] ) {
                return false;
            }
            ++q;
        }

        if (this->domain_[q] == '.') {
            return true;
        }

        return false;
    }

    bool Equals(const Domain& other) const {
        return domain_ == other.domain_;
    }

    bool operator<(const Domain& other) const {
        return lexicographical_compare(domain_.begin(), domain_.end(),other.domain_.begin(), other.domain_.end());
    }


private:
    string domain_;
};

// разработайте operator==
bool operator==(const Domain& lhs, const Domain& rhs) {
    return lhs.Equals(rhs);
}


class DomainChecker {
public:
    // конструктор должен принимать список запрещённых доменов через пару итераторов
    template<typename Iterator>
    DomainChecker(Iterator first, Iterator last)  {
        vector<Domain> tmp(first, last);

        sort(tmp.begin(), tmp.end());

        auto it = unique(tmp.begin(), tmp.end(), [](const Domain& lhs, const Domain& rhs) {
            return lhs.IsSubdomain(rhs)|| rhs.IsSubdomain(lhs);
            });
        tmp.resize(distance(tmp.begin(), it), Domain{""});
        checked_domains_ = tmp;

    }

    // разработайте метод IsForbidden, возвращающий true, если домен запрещён
    bool IsForbidden(const Domain& domain) const {

        auto it = upper_bound(checked_domains_.begin(), checked_domains_.end(),domain);
        if (it == checked_domains_.begin()) {
            return false;
        }
        --it;
        return domain.IsSubdomain(*it);
    }

    //for tests
    size_t CheckedDomainsNmb() {
        return checked_domains_.size();
    }

private: 
    vector<Domain> checked_domains_;
};

// разработайте функцию ReadDomains, читающую заданное количество доменов из стандартного входа
template <typename Number>
Number ReadNumberOnLine(istream& input) {
    string line;
    getline(input, line);
    Number num;
    std::istringstream(line) >> num;
    return num;
}

vector<Domain> ReadDomains(istream& input, size_t nmb ) {
    vector<Domain> dms;
    string line;
    size_t q = 1;
    while (q <= nmb) {
        getline(input, line);
        dms.push_back({ line });
        ++q;
    }
    return dms;
}

void TestIsSubDomain() {
    Domain basic_dm("abc.xzy.ru");
    Domain other_dm1("xzy.ru");
    Domain other_dm2("ru");
    Domain other_dm3("abc.ru");
    Domain other_dm4("rst.abc.xzy.ru");
    assert(basic_dm.IsSubdomain(other_dm1));
    assert(basic_dm.IsSubdomain(other_dm2));
    assert(!basic_dm.IsSubdomain(other_dm3));
    assert(!basic_dm.IsSubdomain(other_dm4));
}

void TestIsSubDomain2() {
    Domain dm1("a.a");
    Domain dm2("aa.a");
    Domain dm3("a.a.a");
    Domain dm4("aa.a.a");
    assert(dm1.IsSubdomain({ "a.a" }));
    assert(!dm2.IsSubdomain({ "a.a" }));
    assert(dm3.IsSubdomain({ "a.a" }));
    assert(dm4.IsSubdomain({ "a.a" }));
}

void TestDomainEquality() {
    Domain dm1("abc.xzy.ru");
    Domain dm2("xzy.ru");
    Domain dm3("abc.xzy.ru");
    assert(!(dm1 == dm2));
    assert(dm1 == dm3);
}

void TestDomainCheckerConstructor() {
    const std::vector<Domain> forbidden_domains{ {"b.a"} , {"z.b.a"} , {"uuu.z.b.a"} , {"cc.b.a"} };
    DomainChecker checker(forbidden_domains.begin(), forbidden_domains.end());
    assert(checker.CheckedDomainsNmb() == 1);
}

void TestDomainChecker() {
    const std::vector<Domain> forbidden_domains{ {"gdz.ru"} , {"maps.me"} , {"m.gdz.ru"} , {"com"} };
    DomainChecker checker(forbidden_domains.begin(), forbidden_domains.end());
    assert(checker.IsForbidden({ "gdz.ru" }));
    assert(checker.IsForbidden({ "gdz.com" }));
    assert(checker.IsForbidden({ "m.maps.me" }));
    assert(checker.IsForbidden({ "alg.m.gdz.ru" }));
    assert(checker.IsForbidden({ "maps.com" }));
    assert(!checker.IsForbidden({ "maps.ru" }));
    assert(!checker.IsForbidden({ "gdz.ua" }));
}

void TestDomainChecker2() {
    const std::vector<Domain> forbidden_domains{ {"a.a"}  };
    DomainChecker checker(forbidden_domains.begin(), forbidden_domains.end());
    assert(checker.IsForbidden({ "a.a" }));
    assert(checker.IsForbidden({ "aaaaa.a.a" }));
    assert(checker.IsForbidden({ "a.a.a.a.a" }));
    assert(checker.IsForbidden({ "aa.aa.aa.a.a" }));
    assert(!checker.IsForbidden({ "a" }));
    assert(!checker.IsForbidden({ "aa.a" }));
    assert(!checker.IsForbidden({ "a.aa.a" }));
}

void TestDomainChecker3() {
    const std::vector<Domain> forbidden_domains{ {"a.a"} , {"aa.a"} , {"a.aa.a"} , {"aaa.a"} ,  {"a.a.a"} };
    DomainChecker checker(forbidden_domains.begin(), forbidden_domains.end());
    assert(checker.CheckedDomainsNmb() == 3);
    assert(checker.IsForbidden({"a.a"}));
    assert(checker.IsForbidden({ "aa.a" }));
    assert(checker.IsForbidden({ "aaa.a" }));
}

void UnitTests() {
    TestIsSubDomain();
    TestIsSubDomain2();
    TestDomainEquality();
    TestDomainCheckerConstructor();
    TestDomainChecker();
    TestDomainChecker2();
    TestDomainChecker3();
    cout << "Unit tests OK" << endl;
}


int main() {
    UnitTests();
    
    const std::vector<Domain> forbidden_domains = ReadDomains(cin, ReadNumberOnLine<size_t>(cin));
    DomainChecker checker(forbidden_domains.begin(), forbidden_domains.end());

    const std::vector<Domain> test_domains = ReadDomains(cin, ReadNumberOnLine<size_t>(cin));
    for (const Domain& domain : test_domains) {
        cout << (checker.IsForbidden(domain) ? "Bad"sv : "Good"sv) << endl;
    }   
}