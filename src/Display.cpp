#include "Display.h"
#include <sstream>
#include <limits>
#include <algorithm>
using namespace std;

// This function prints the CineSearch banner.
void Display::printBanner() {
    cout << "+--------------------------------------------------------------------+" << endl;
    cout << "|      ____ ___ _   _ _____ ____  _____    _    ____   ____ _   _   |" << endl;
    cout << "|     / ___|_ _| \\ | | ____/ ___|| ____|  / \\  |  _ \\ / ___| | | |  |" << endl;
    cout << "|    | |    | ||  \\| |  _| \\___ \\|  _|   / _ \\ | |_) | |   | |_| |  |" << endl;
    cout << "|    | |___ | || |\\  | |___ ___) | |___ / ___ \\|  _ <| |___|  _  |  |" << endl;
    cout << "|     \\____|___|_| \\_|_____|____/|_____/_/   \\_\\_| \\_\\\\____|_| |_|  |" << endl;
    cout << "|                           CINESEARCH                              |" << endl;
    cout << "|               Smart Query Optimization Engine                     |" << endl;
    cout << "+--------------------------------------------------------------------+" << endl;
}

// This function prints the main menu.
void Display::printMenu() {
    cout << "\n+------------------------------------------------------+" << endl;
    cout << "|                      MAIN MENU                       |" << endl;
    cout << "+------------------------------------------------------+" << endl;
    cout << "|  1. Exact Search (Title / ID)                        |" << endl;
    cout << "|  2. Autocomplete Search (Prefix)                     |" << endl;
    cout << "|  3. Range Query (Year Filter)                        |" << endl;
    cout << "|  4. Top-K Query (Latest Content)                     |" << endl;
    cout << "|  5. Compare All Methods                              |" << endl;
    cout << "|  6. Dataset Size Performance Test                    |" << endl;
    cout << "|  7. Complexity Analysis                              |" << endl;
    cout << "|  0. Exit                                             |" << endl;
    cout << "+------------------------------------------------------+" << endl;
}

// This function prints scenario title header.
void Display::printScenarioHeader(string scenarioTitle) {
    cout << "\n============================================================" << endl;
    cout << " SCENARIO: " << scenarioTitle << endl;
    cout << "============================================================" << endl;
    cout << endl;
}

// This function prints results section header.
void Display::printResultsHeader() {
    cout << "[RESULTS FOUND]" << endl;
}

// This function prints one result line.
void Display::printResult(string title) {
    cout << "  -> " << title << endl;
}

// This function prints performance section header.
void Display::printPerfHeader() {
    cout << "\n------------------------------------------------------------" << endl;
    cout << " PERFORMANCE COMPARISON" << endl;
    cout << "------------------------------------------------------------" << endl;
}

// This function formats integer microseconds with commas.
string Display::formatTime(long long t) {
    string s = to_string(t);
    string ans = "";
    int n = (int)s.size();
    int cnt = 0;
    for (int i = n - 1; i >= 0; i--) {
        ans += s[i];
        cnt++;
        if (cnt == 3 && i > 0) {
            ans += ',';
            cnt = 0;
        }
    }
    reverse(ans.begin(), ans.end());
    return ans;
}

// This function prints one performance row.
void Display::printPerfRow(string approach, long long timeNs, string complexity) {
    long long shown = timeNs;
    string unit = " ns/op";

    if (timeNs >= 1000000) {
        shown = timeNs / 1000000;
        unit = " ms/op";
    } else if (timeNs >= 1000) {
        shown = timeNs / 1000;
        unit = " us/op";
    }

    string t = formatTime(shown);
    cout << left;
    cout.width(28);
    cout << approach;
    cout << " -> ";
    cout << right;
    cout.width(8);
    cout << t << unit;
    cout << "   -> " << complexity << endl;
}

// This function prints winner name.
void Display::printWinner(string dsName) {
    cout << "\n** BEST APPROACH: " << dsName << " **" << endl;
}

// This helper splits text into wrapped lines of max width.
static vector<string> wrapText(const string& text, int width) {
    vector<string> lines;
    string word = "";
    string curr = "";
    for (int i = 0; i <= (int)text.size(); i++) {
        char ch = (i < (int)text.size()) ? text[i] : ' ';
        if (ch == '\n') {
            if (!word.empty()) {
                if ((int)curr.size() + (curr.empty() ? 0 : 1) + (int)word.size() <= width) {
                    if (!curr.empty()) curr += " ";
                    curr += word;
                } else {
                    if (!curr.empty()) lines.push_back(curr);
                    curr = word;
                }
                word = "";
            }
            if (!curr.empty()) {
                lines.push_back(curr);
                curr = "";
            }
            lines.push_back("");
        } else if (ch == ' ' || i == (int)text.size()) {
            if (!word.empty()) {
                if ((int)curr.size() + (curr.empty() ? 0 : 1) + (int)word.size() <= width) {
                    if (!curr.empty()) curr += " ";
                    curr += word;
                } else {
                    if (!curr.empty()) lines.push_back(curr);
                    curr = word;
                }
                word = "";
            }
        } else {
            word += ch;
        }
    }
    if (!curr.empty()) {
        lines.push_back(curr);
    }
    return lines;
}

// This function prints wrapped explanation paragraph.
void Display::printWhyExplanation(string explanation) {
    cout << "\nWHY (DETAILED EXPLANATION):\n" << endl;
    vector<string> lines = wrapText(explanation, 60);
    for (int i = 0; i < (int)lines.size(); i++) {
        cout << lines[i] << endl;
    }
    cout << endl;
}

// This function waits for enter key.
void Display::pressEnterToContinue() {
    cout << "\nPress ENTER to continue..." << endl;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

// This function prints a section line with title.
void Display::printSectionLine(string title) {
    cout << "\n------------------------------------------------------------" << endl;
    cout << " " << title << endl;
    cout << "------------------------------------------------------------" << endl;
}

// This function prints complexity table for all DS.
void Display::printComplexityTable() {
    cout << "+------------------+----------+----------+----------+----------+----------+" << endl;
    cout << "| Data Structure   | Insert   | Exact    | Prefix   | Range    | Top-K    |" << endl;
    cout << "+------------------+----------+----------+----------+----------+----------+" << endl;
    cout << "| Linear Scan      |  O(1)    |  O(n)    |  O(n)    |  O(n)    | O(nlogn) |" << endl;
    cout << "| B+ Tree          | O(logn)  | O(logn)  | O(logn)  | O(logn)  | O(logn)  |" << endl;
    cout << "| AVL Tree         | O(logn)  | O(logn)  |  O(n)    |  O(n)    |  O(n)    |" << endl;
    cout << "| Hash Table       |  O(1)    |  O(1)    |  O(n)*   |  O(n)*   |  O(n)    |" << endl;
    cout << "| Trie             |  O(L)    |  O(L)    | O(L+k)   |  O(n)    |  O(n)    |" << endl;
    cout << "| Segment Tree     | O(logn)  |  O(n)    |  O(n)    | O(logn)  |  O(n)    |" << endl;
    cout << "| Skip List        | O(logn)  | O(logn)  |  O(n)    | O(logn)  |  O(n)    |" << endl;
    cout << "| Max-Heap         | O(logn)  |  O(n)    |  O(n)    |  O(n)    | O(klogn) |" << endl;
    cout << "+------------------+----------+----------+----------+----------+----------+" << endl;
    cout << "L = prefix/key length  |  k = number of results  |  a = inverse Ackermann" << endl;
}

// This function prints success message.
void Display::printSuccess(string msg) {
    cout << "[SUCCESS] " << msg << endl;
}

// This function prints info message.
void Display::printInfo(string msg) {
    cout << "[INFO]    " << msg << endl;
}

// This function prints warning message.
void Display::printWarn(string msg) {
    cout << "[WARN]    " << msg << endl;
}

// This function prints loading message.
void Display::printLoading(string msg) {
    cout << "[LOADING] " << msg << endl;
}
