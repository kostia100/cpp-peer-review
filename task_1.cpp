#include <iostream>
#include <iomanip>
#include <vector>

using namespace std;


class EBook {
public:

    EBook() : users_(0), users_to_page(100'001) , page_to_users(1001){

    }

    void UpdateUserData(int user_id , int page_number) {
        int current_page = users_to_page[user_id];
        if (current_page ==0) {
            //new user
            users_++;
            users_to_page[user_id] = page_number;
            for (int page = 1; page <= page_number; ++page) {
                page_to_users[page]++;
            }
        }
        else {
            //already existing user
            users_to_page[user_id] = page_number;
            for (int page = current_page + 1; page <= page_number; ++page) {
                page_to_users[page]++;
            }
        }

       
     }

    double CheerUser(int user_id) {

        int pages = users_to_page[user_id];  
        if (pages == 0) {
            return 0.0;
        }
        int reads = page_to_users[pages];

        if (reads == 1) {
            return 1;
        }

        int no_reads = users_ - reads;

        return (no_reads )/((users_-1) *1.0);
    }

private:

    int users_;
    vector<int> users_to_page;
    vector<int> page_to_users;


};

void ReadEBook(EBook& book,istream& input, ostream& output ) {
    int q;
    input >> q;
    int j = 1;
    while (j <= q) {
        string message;
        input >> message;
        if (message == "READ") {
            int user_id;
            int page_number;
            input >> user_id >> page_number;
            book.UpdateUserData(user_id, page_number);
            ++j;

        }
        else if (message == "CHEER") {
            int user_id;
            input >> user_id;
            output << setprecision(6) << book.CheerUser(user_id) << endl;
            ++j;
        }
    }
}

int main()
{
    EBook book;
    ReadEBook(book,cin,cout);

}

