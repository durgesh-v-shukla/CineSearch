#pragma once
#include <fstream>
#include <vector>
#include <string>
#include "Content.h"
using namespace std;

Content fallbackData[] = {
    {"s0001", "Movie", "Inception", 2010, "Action", "PG-13"},
    {"s0002", "TV Show", "Stranger Things", 2016, "Drama", "TV-14"},
    {"s0003", "TV Show", "Breaking Bad", 2008, "Drama", "TV-MA"},
    {"s0004", "Movie", "Avengers Endgame", 2019, "Action", "PG-13"},
    {"s0005", "Movie", "Bird Box", 2018, "Thriller", "R"},
    {"s0006", "TV Show", "Narcos", 2015, "Action", "TV-MA"},
    {"s0007", "TV Show", "Money Heist", 2017, "Thriller", "TV-MA"},
    {"s0008", "TV Show", "Dark", 2017, "Drama", "TV-MA"},
    {"s0009", "TV Show", "Squid Game", 2021, "Thriller", "TV-MA"},
    {"s0010", "TV Show", "The Crown", 2016, "Drama", "TV-MA"},
    {"s0011", "TV Show", "Ozark", 2017, "Thriller", "TV-MA"},
    {"s0012", "TV Show", "Tiger King", 2020, "Documentary", "TV-MA"},
    {"s0013", "TV Show", "The Witcher", 2019, "Action", "TV-MA"},
    {"s0014", "Movie", "Extraction", 2020, "Action", "R"},
    {"s0015", "Movie", "The Irishman", 2019, "Drama", "R"},
    {"s0016", "Movie", "Roma", 2018, "Drama", "R"},
    {"s0017", "Movie", "The Social Dilemma", 2020, "Documentary", "PG-13"},
    {"s0018", "Movie", "Marriage Story", 2019, "Drama", "R"},
    {"s0019", "TV Show", "Sacred Games", 2018, "Thriller", "TV-MA"},
    {"s0020", "Movie", "The Gray Man", 2022, "Action", "PG-13"},
    {"s0021", "Movie", "Red Notice", 2021, "Action", "PG-13"},
    {"s0022", "Movie", "The Adam Project", 2022, "Action", "PG-13"},
    {"s0023", "Movie", "Army of the Dead", 2021, "Action", "R"},
    {"s0024", "Movie", "Enola Holmes", 2020, "Comedy", "PG-13"},
    {"s0025", "TV Show", "Wednesday", 2022, "Comedy", "TV-14"},
    {"s0026", "TV Show", "Black Mirror", 2011, "Thriller", "TV-MA"},
    {"s0027", "TV Show", "Better Call Saul", 2015, "Drama", "TV-MA"},
    {"s0028", "TV Show", "Peaky Blinders", 2013, "Drama", "TV-MA"},
    {"s0029", "TV Show", "The Last Dance", 2020, "Documentary", "TV-MA"},
    {"s0030", "Movie", "Don\"t Look Up", 2021, "Comedy", "R"},
    {"s0031", "Movie", "Glass Onion", 2022, "Comedy", "PG-13"},
    {"s0032", "Movie", "The Dig", 2021, "Drama", "PG-13"},
    {"s0033", "TV Show", "The Queen\"s Gambit", 2020, "Drama", "TV-MA"},
    {"s0034", "TV Show", "Manifest", 2018, "Drama", "TV-14"},
    {"s0035", "Movie", "Outside the Wire", 2021, "Action", "R"},
    {"s0036", "Movie", "Project Power", 2020, "Action", "R"},
    {"s0037", "Movie", "The Platform", 2019, "Thriller", "TV-MA"},
    {"s0038", "TV Show", "Lupin", 2021, "Thriller", "TV-MA"},
    {"s0039", "TV Show", "Sex Education", 2019, "Comedy", "TV-MA"},
    {"s0040", "TV Show", "Alice in Borderland", 2020, "Thriller", "TV-MA"},
    {"s0041", "Movie", "Dhamaka", 2021, "Thriller", "TV-MA"},
    {"s0042", "Movie", "Minnal Murali", 2021, "Action", "TV-14"},
    {"s0043", "TV Show", "Delhi Crime", 2019, "Drama", "TV-MA"},
    {"s0044", "TV Show", "Kota Factory", 2019, "Drama", "TV-14"},
    {"s0045", "Movie", "Jamtara", 2020, "Drama", "TV-MA"},
    {"s0046", "Movie", "Extraction 2", 2023, "Action", "R"},
    {"s0047", "TV Show", "Wednesday Addams Files", 2023, "Comedy", "TV-14"},
    {"s0048", "Movie", "The Pale Blue Eye", 2023, "Thriller", "R"},
    {"s0049", "TV Show", "Our Planet", 2019, "Documentary", "TV-G"},
    {"s0050", "Movie", "13th", 2016, "Documentary", "TV-MA"}
};

const int FALLBACK_COUNT = 50;

// This function parses one CSV line while handling quoted commas.
vector<string> parseCSVLine(const string& line) {
    vector<string> fields;
    string curr = "";
    bool inQuotes = false;
    for (int i = 0; i < (int)line.size(); i++) {
        if (line[i] == '"') {
            inQuotes = !inQuotes;
        } else if (line[i] == ',' && !inQuotes) {
            fields.push_back(curr);
            curr = "";
        } else {
            curr += line[i];
        }
    }
    fields.push_back(curr);
    return fields;
}

// This function loads Netflix data from CSV, else uses fallback records.
vector<Content> loadCSV(const string& filename) {
    vector<Content> titles;
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "[WARN]  netflix_titles.csv not found." << endl;
        cout << "[INFO]  Loading built-in fallback dataset." << endl;
        for (int i = 0; i < FALLBACK_COUNT; i++) {
            titles.push_back(fallbackData[i]);
        }
        return titles;
    }

    string line;
    getline(file, line);
    while (getline(file, line)) {
        if (line.empty()) {
            continue;
        }
        if (line.back() == '\r') {
            line.pop_back();
        }
        vector<string> f = parseCSVLine(line);
        if ((int)f.size() < 12) {
            continue;
        }

        Content c;
        c.showId = f[0];
        c.type = f[1];
        c.title = f[2];
        try {
            c.releaseYear = stoi(f[7]);
        } catch (...) {
            continue;
        }
        if (c.releaseYear == 0 || c.title.empty()) {
            continue;
        }
        c.genre = f[10];
        c.rating = f[8];
        titles.push_back(c);
    }

    file.close();
    return titles;
}
