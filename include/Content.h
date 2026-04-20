#pragma once
#include <iostream>
#include <string>
#include <iomanip>
using namespace std;

struct Content {
    string showId;
    string type;
    string title;
    int releaseYear;
    string genre;
    string rating;

    // This function prints one content record in clean aligned format.
    void print() const {
        cout << "  [" << setw(6) << left << showId << "] "
             << setw(42) << left << title.substr(0, 40)
             << setw(9) << left << type
             << releaseYear << endl;
    }
};
