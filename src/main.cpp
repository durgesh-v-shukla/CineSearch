// ============================================================
// CineSearch: Smart Query Optimization Engine
// CS2308 Data Structures II - VIT Pune - AY 2025-26
// ============================================================
// 7 Data Structures Implemented From Scratch:
//   Unit 1: B+ Tree (industry), AVL Tree (comparison)
//   Unit 2: Max-Heap (top-K proposal)
//   Unit 3: Trie (prefix search proposal)
//   Unit 4: Skip List (range query comparison)
//   Unit 5: Segment Tree (range aggregation proposal)
//   Plus:   Hash Table (exact search proposal)
// ============================================================
// Three approaches compared in every scenario:
//   Approach 1: Naive (linear scan - no index)
//   Approach 2: Industry Standard (B+ Tree - MySQL InnoDB)
//   Approach 3: Specialized DS (designed for that query)
// ============================================================

#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <iomanip>
#include <limits>
#include <cctype>
#include <algorithm>
#include <functional>
#include "data.h"
#include "Display.h"
#include "LinearSearch.h"
#include "BPlusTreeStr.h"
#include "BPlusTreeInt.h"
#include "HashTable.h"
#include "AVLTree.h"
#include "Trie.h"
#include "SegmentTree.h"
#include "SkipList.h"
#include "MaxHeap.h"
using namespace std;

// This helper benchmarks one operation and returns average ns per operation.
long long benchmarkAvgNs(function<void()> op, int minReps) {
    int reps = max(1, minReps);
    const int MAX_REPS = 10000000;
    const long long TARGET_NS = 2000000; // keep timing window >= 2 ms
    const int TRIALS = 5;
    long long elapsedNs = 0;

    // Warm up once to reduce first-run cache and branch predictor noise.
    for (int i = 0; i < 16; i++) {
        op();
    }

    while (true) {
        auto t1 = chrono::high_resolution_clock::now();
        for (int i = 0; i < reps; i++) {
            op();
        }
        auto t2 = chrono::high_resolution_clock::now();
        elapsedNs = chrono::duration_cast<chrono::nanoseconds>(t2 - t1).count();

        if (elapsedNs >= TARGET_NS || reps >= MAX_REPS) {
            break;
        }
        reps *= 2;
    }

    vector<long long> samples;
    samples.reserve(TRIALS);
    for (int trial = 0; trial < TRIALS; trial++) {
        auto t1 = chrono::high_resolution_clock::now();
        for (int i = 0; i < reps; i++) {
            op();
        }
        auto t2 = chrono::high_resolution_clock::now();
        long long runNs = chrono::duration_cast<chrono::nanoseconds>(t2 - t1).count();
        long long avg = runNs / reps;
        if (avg <= 0) {
            avg = 1;
        }
        samples.push_back(avg);
    }

    sort(samples.begin(), samples.end());
    long long avgNs = samples[TRIALS / 2]; // median
    if (avgNs <= 0) {
        avgNs = 1;
    }
    return avgNs;
}

#define BENCH(block, reps, result) {                               \
    (result) = benchmarkAvgNs([&](){ block }, (reps));             \
}

Display display;

// This function returns winner name and handles ties for near-equal timings.
string bestOfWithTie(vector<string> names, vector<long long> times) {
    if (names.empty() || times.empty() || names.size() != times.size()) {
        return "N/A";
    }

    long long best = (times[0] <= 0) ? 1 : times[0];
    for (int i = 1; i < (int)times.size(); i++) {
        long long curr = (times[i] <= 0) ? 1 : times[i];
        if (curr < best) {
            best = curr;
        }
    }

    // 5% tolerance (minimum 200 ns) to treat very close values as tie.
    long long tol = best / 20;
    if (tol < 200) {
        tol = 200;
    }

    vector<string> winners;
    for (int i = 0; i < (int)times.size(); i++) {
        long long curr = (times[i] <= 0) ? 1 : times[i];
        long long diff = (curr > best) ? (curr - best) : (best - curr);
        if (diff <= tol) {
            winners.push_back(names[i]);
        }
    }

    if (winners.empty()) {
        return names[0];
    }
    if ((int)winners.size() == 1) {
        return winners[0];
    }

    string ans = "Tie: ";
    for (int i = 0; i < (int)winners.size(); i++) {
        if (i > 0) {
            ans += " / ";
        }
        ans += winners[i];
    }
    return ans;
}

// This function normalizes prefix to title-like case for fair matching across DS.
string normalizePrefix(string s) {
    if (s.empty()) {
        return s;
    }
    s[0] = (char)toupper((unsigned char)s[0]);
    for (int i = 1; i < (int)s.size(); i++) {
        s[i] = (char)tolower((unsigned char)s[i]);
    }
    return s;
}

// This function avoids divide-by-zero and ultra-small timing artifacts.
long long safeTime(long long t) {
    if (t <= 0) {
        return 1;
    }
    return t;
}

// This function handles scenario 1 exact title search.
void scenario1_ExactSearch(vector<Content>& titles, BPlusTreeStr& bptStr, HashTable& hashTable, AVLTree& avlTree, LinearSearch& linear) {
    display.printScenarioHeader("EXACT SEARCH");

    cout << "Enter exact title to search: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    string target;
    getline(cin, target);

    display.printResultsHeader();
    Content* found = hashTable.search(target);
    if (found != nullptr) {
        display.printResult(found->title + " (" + found->type + " " + to_string(found->releaseYear) + ")");
    } else {
        cout << "  [NOT FOUND]" << endl;
    }

    display.printPerfHeader();
    int REPS = 10000;
    long long t1 = 0, t2 = 0, t3 = 0, t4 = 0;

    BENCH(linear.exactSearch(titles, target);, REPS, t1)
    BENCH(bptStr.exactSearch(target);, REPS, t2)
    BENCH(avlTree.exactSearch(target);, REPS, t3)
    BENCH(hashTable.search(target);, REPS, t4)

    display.printPerfRow("Brute Force (Linear Scan)", t1, "O(n)");
    display.printPerfRow("Industry   (B+ Tree)", t2, "O(log n)");
    display.printPerfRow("Proposed   (AVL Tree)", t3, "O(log n)");
    display.printPerfRow("Proposed   (Hash Table)", t4, "O(1)");

    vector<string> names;
    names.push_back("Brute Force (Linear Scan)");
    names.push_back("Industry (B+ Tree)");
    names.push_back("Proposed (AVL Tree)");
    names.push_back("Proposed (Hash Table)");
    vector<long long> times;
    times.push_back(t1);
    times.push_back(t2);
    times.push_back(t3);
    times.push_back(t4);
    display.printWinner(bestOfWithTie(names, times));

    display.printWhyExplanation(
        "In brute force, every single title is checked one by one until "
        "a match is found. With 8807 records, this means up to 8807 "
        "comparisons per search.\n\n"
        "B+ Tree (used by MySQL InnoDB) traverses from root to leaf, "
        "making only O(log n) comparisons - about 13 steps for 8807 records.\n\n"
        "AVL Tree achieves the same O(log n) complexity. Both B+ Tree and "
        "AVL Tree are similar here because both are balanced trees.\n\n"
        "Hash Table computes the bucket directly using a hash function. "
        "No traversal needed - O(1) average. This is why Redis is used "
        "alongside MySQL as a cache layer for hot exact-lookup queries.");

    display.pressEnterToContinue();
}

// This function handles scenario 2 prefix autocomplete.
void scenario2_PrefixSearch(vector<Content>& titles, BPlusTreeStr& bptStr, Trie& trie, LinearSearch& linear) {
    display.printScenarioHeader("AUTOCOMPLETE SEARCH");

    cout << "Enter search prefix (e.g. Str, Th, Na): ";
    string prefix;
    cin >> prefix;
    string benchPrefix = normalizePrefix(prefix);

    vector<Content> trieResults = trie.prefixSearch(benchPrefix);
    display.printResultsHeader();
    int limit = min(5, (int)trieResults.size());
    for (int i = 0; i < limit; i++) {
        display.printResult(trieResults[i].title);
    }
    if ((int)trieResults.size() > limit) {
        cout << "  ... and " << ((int)trieResults.size() - limit) << " more" << endl;
    }

    display.printPerfHeader();
    int REPS = 10000;
    long long t1 = 0, t2 = 0, t3 = 0;

    BENCH(linear.prefixSearch(titles, benchPrefix);, REPS, t1)
    BENCH(bptStr.prefixSearch(benchPrefix);, REPS, t2)
    BENCH(trie.prefixSearch(benchPrefix);, REPS, t3)

    display.printPerfRow("Brute Force (Linear Scan)", t1, "O(n x L)");
    display.printPerfRow("Industry   (B+ Tree)", t2, "O(log n + k)");
    display.printPerfRow("Specialized (Trie)", t3, "O(L + k)");

    vector<string> names;
    names.push_back("Brute Force (Linear Scan)");
    names.push_back("Industry (B+ Tree)");
    names.push_back("Specialized (Trie)");
    vector<long long> times;
    times.push_back(t1);
    times.push_back(t2);
    times.push_back(t3);
    display.printWinner(bestOfWithTie(names, times));

    display.printWhyExplanation(
        "Brute force checks every title to see if it starts with the "
        "given prefix. All 8807 records are scanned every keystroke.\n\n"
        "B+ Tree (MySQL approach for LIKE 'prefix%') navigates to the "
        "first matching key in O(log n) then scans forward through "
        "linked leaves. Better than brute force but still scans linearly "
        "through matching entries.\n\n"
        "Trie is specifically designed for prefix search. It navigates "
        "directly - one step per character in the prefix. At the prefix "
        "node, it collects only matching titles. Non-matching titles are "
        "never touched. This is why Elasticsearch uses a Trie-based term "
        "index for text autocomplete.");

    display.pressEnterToContinue();
}

// This function handles scenario 3 year range query.
void scenario3_RangeQuery(vector<Content>& titles, BPlusTreeInt& bptInt, SegmentTree& segTree, SkipList& skipList, LinearSearch& linear) {
    display.printScenarioHeader("RANGE QUERY (YEAR FILTER)");

    cout << "  a) 2020 - 2023 (Recent content)" << endl;
    cout << "  b) 2015 - 2019 (Last half decade)" << endl;
    cout << "  c) 2010 - 2014 (Earlier decade)" << endl;
    cout << "  d) Custom range" << endl;
    cout << "Choose (a/b/c/d): ";

    char ch;
    cin >> ch;
    int yearL = 2018, yearR = 2022;

    if (ch == 'a' || ch == 'A') {
        yearL = 2020;
        yearR = 2023;
    } else if (ch == 'b' || ch == 'B') {
        yearL = 2015;
        yearR = 2019;
    } else if (ch == 'c' || ch == 'C') {
        yearL = 2010;
        yearR = 2014;
    } else if (ch == 'd' || ch == 'D') {
        cout << "Enter start year: ";
        cin >> yearL;
        cout << "Enter end year: ";
        cin >> yearR;
        if (yearL > yearR) {
            int temp = yearL;
            yearL = yearR;
            yearR = temp;
        }
    }

    int segCount = segTree.countInRange(yearL, yearR);
    cout << "\n  Titles found: " << segCount << endl;
    vector<Content> sampleResults = segTree.titlesInRange(titles, yearL, yearR);
    int limit = min(5, (int)sampleResults.size());
    for (int i = 0; i < limit; i++) {
        display.printResult(sampleResults[i].title + " (" + to_string(sampleResults[i].releaseYear) + ")");
    }

    display.printPerfHeader();
    display.printInfo("Running benchmarks... please wait");
    int REPS = 3000;
    long long t1 = 0, t2 = 0, t3 = 0, t4 = 0;

    BENCH(linear.rangeQuery(titles, yearL, yearR);, REPS, t1)
    BENCH(bptInt.rangeQuery(yearL, yearR);, REPS, t2)
    BENCH(segTree.countInRange(yearL, yearR);, REPS, t3)
    BENCH(skipList.rangeQuery(yearL, yearR);, REPS, t4)

    display.printPerfRow("Brute Force (Linear Scan)", t1, "O(n)");
    display.printPerfRow("Industry   (B+ Tree)", t2, "O(log n + k)");
    display.printPerfRow("Proposed   (Skip List)", t4, "O(log n + k)");
    display.printPerfRow("Specialized (Seg Tree)", t3, "O(log n)");

    vector<string> names;
    names.push_back("Brute Force (Linear Scan)");
    names.push_back("Industry (B+ Tree)");
    names.push_back("Proposed (Skip List)");
    names.push_back("Specialized (Segment Tree)");
    vector<long long> times;
    times.push_back(t1);
    times.push_back(t2);
    times.push_back(t4);
    times.push_back(t3);
    display.printWinner(bestOfWithTie(names, times));

    display.printWhyExplanation(
        "Brute force loops through all 8807 records checking each year.\n\n"
        "B+ Tree (MySQL InnoDB range scan) navigates to the starting year "
        "in O(log n) then walks linked leaf nodes. Still visits every "
        "matching record - O(log n + k).\n\n"
        "Skip List achieves similar O(log n + k) using probabilistic "
        "balancing. Redis uses Skip List for its sorted sets which "
        "support range queries.\n\n"
        "Segment Tree pre-computes range sums during build. The COUNT "
        "query is answered in O(log n) - no individual records visited. "
        "For analytics (how many titles per decade, average per year), "
        "Segment Tree is the fastest choice.");

    display.pressEnterToContinue();
}

// This function handles scenario 4 top-k latest content.
void scenario4_TopK(vector<Content>& titles, BPlusTreeInt& bptInt, MaxHeap& heap, LinearSearch& linear) {
    display.printScenarioHeader("TOP-K QUERY");

    cout << "How many latest titles to show? (e.g. 5, 10, 20): ";
    int k;
    cin >> k;
    if (k <= 0 || k > 50) {
        k = 10;
    }

    vector<Content> topResults = heap.getTopK(k);
    display.printResultsHeader();
    for (int i = 0; i < (int)topResults.size(); i++) {
        cout << "  " << (i + 1) << ". ";
        topResults[i].print();
    }

    display.printPerfHeader();
    display.printInfo("Running benchmarks... please wait");
    int REPS = 100;
    long long t1 = 0, t2 = 0, t3 = 0;

    BENCH(linear.topK(titles, k);, REPS, t1)
    BENCH(bptInt.topK(k);, REPS, t2)
    BENCH(heap.getTopK(k);, REPS, t3)

    display.printPerfRow("Brute Force (Full Sort)", t1, "O(n log n)");
    display.printPerfRow("Industry   (B+ Tree)", t2, "O(log n + k)");
    display.printPerfRow("Specialized (Max-Heap)", t3, "O(k log n)");

    vector<string> names;
    names.push_back("Brute Force (Full Sort)");
    names.push_back("Industry (B+ Tree)");
    names.push_back("Specialized (Max-Heap)");
    vector<long long> times;
    times.push_back(t1);
    times.push_back(t2);
    times.push_back(t3);
    display.printWinner(bestOfWithTie(names, times));

    display.printWhyExplanation(
        "Brute force sorts all 8807 titles every time someone requests "
        "top-K. Sorting is O(n log n) - wasteful for a small k.\n\n"
        "B+ Tree (MySQL ORDER BY LIMIT k) navigates to the rightmost "
        "leaf and reads k records backwards - O(log n + k). Very fast "
        "for static data with an existing index.\n\n"
        "Max-Heap maintains the maximum element at position 0 always. "
        "Getting top-K costs O(k log n). When Netflix adds a new title "
        "(dynamic insertion), Heap inserts in O(log n). Redis uses "
        "Skip List (same principle) for its global leaderboards.");

    display.pressEnterToContinue();
}

// This function runs all default tests and prints final summary.
void compareAllMethods(vector<Content>& titles, BPlusTreeStr& bptStr, HashTable& hashTable, AVLTree& avlTree, Trie& trie,
                       BPlusTreeInt& bptInt, SegmentTree& segTree, SkipList& skipList, MaxHeap& heap, LinearSearch& linear) {
    display.printScenarioHeader("COMPARE ALL METHODS");

    string target = "Inception";
    string prefix = "Str";
    int yearL = 2018, yearR = 2022;
    int k = 10;

    display.printInfo("Running all scenario benchmarks... this may take a few seconds");

    int REPS1 = 500, REPS2 = 500, REPS3 = 500, REPS4 = 100;
    long long s1Naive = 0, s1Best = 0;
    long long s2Naive = 0, s2Best = 0;
    long long s3Naive = 0, s3Best = 0;
    long long s4Naive = 0, s4Best = 0;

    long long t1 = 0, t2 = 0, t3 = 0, t4 = 0;
    BENCH(linear.exactSearch(titles, target);, REPS1, t1)
    BENCH(bptStr.exactSearch(target);, REPS1, t2)
    BENCH(avlTree.exactSearch(target);, REPS1, t3)
    BENCH(hashTable.search(target);, REPS1, t4)
    s1Naive = t1;
    s1Best = min(min(t2, t3), t4);
    display.printInfo("Benchmark 1/4 complete (Exact Search)");

    long long p1 = 0, p2 = 0, p3 = 0;
    BENCH(linear.prefixSearch(titles, prefix);, REPS2, p1)
    BENCH(bptStr.prefixSearch(prefix);, REPS2, p2)
    BENCH(trie.prefixSearch(prefix);, REPS2, p3)
    s2Naive = p1;
    s2Best = min(p2, p3);
    display.printInfo("Benchmark 2/4 complete (Prefix Search)");

    long long r1 = 0, r2 = 0, r3 = 0, r4 = 0;
    BENCH(linear.rangeQuery(titles, yearL, yearR);, REPS3, r1)
    BENCH(bptInt.rangeQuery(yearL, yearR);, REPS3, r2)
    BENCH(segTree.countInRange(yearL, yearR);, REPS3, r3)
    BENCH(skipList.rangeQuery(yearL, yearR);, REPS3, r4)
    s3Naive = r1;
    s3Best = min(min(r2, r3), r4);
    display.printInfo("Benchmark 3/4 complete (Range Query)");

    long long k1 = 0, k2 = 0, k3 = 0;
    BENCH(linear.topK(titles, k);, REPS4, k1)
    BENCH(bptInt.topK(k);, REPS4, k2)
    BENCH(heap.getTopK(k);, REPS4, k3)
    s4Naive = k1;
    s4Best = min(k2, k3);
    display.printInfo("Benchmark 4/4 complete (Top-K Query)");

    vector<string> exactNames;
    exactNames.push_back("Linear Scan");
    exactNames.push_back("B+ Tree");
    exactNames.push_back("AVL Tree");
    exactNames.push_back("Hash Table");
    vector<long long> exactTimes;
    exactTimes.push_back(t1);
    exactTimes.push_back(t2);
    exactTimes.push_back(t3);
    exactTimes.push_back(t4);
    string bestExact = bestOfWithTie(exactNames, exactTimes);

    vector<string> prefixNames;
    prefixNames.push_back("Linear Scan");
    prefixNames.push_back("B+ Tree");
    prefixNames.push_back("Trie");
    vector<long long> prefixTimes;
    prefixTimes.push_back(p1);
    prefixTimes.push_back(p2);
    prefixTimes.push_back(p3);
    string bestPrefix = bestOfWithTie(prefixNames, prefixTimes);

    vector<string> rangeNames;
    rangeNames.push_back("Linear Scan");
    rangeNames.push_back("B+ Tree");
    rangeNames.push_back("Segment Tree");
    rangeNames.push_back("Skip List");
    vector<long long> rangeTimes;
    rangeTimes.push_back(r1);
    rangeTimes.push_back(r2);
    rangeTimes.push_back(r3);
    rangeTimes.push_back(r4);
    string bestRange = bestOfWithTie(rangeNames, rangeTimes);

    vector<string> topNames;
    topNames.push_back("Linear Sort");
    topNames.push_back("B+ Tree");
    topNames.push_back("Max-Heap");
    vector<long long> topTimes;
    topTimes.push_back(k1);
    topTimes.push_back(k2);
    topTimes.push_back(k3);
    string bestTopK = bestOfWithTie(topNames, topTimes);

        long long s1NaiveSafe = safeTime(s1Naive);
        long long s1BestSafe = safeTime(s1Best);
        long long s2NaiveSafe = safeTime(s2Naive);
        long long s2BestSafe = safeTime(s2Best);
        long long s3NaiveSafe = safeTime(s3Naive);
        long long s3BestSafe = safeTime(s3Best);
        long long s4NaiveSafe = safeTime(s4Naive);
        long long s4BestSafe = safeTime(s4Best);

        cout << "\n============================================================" << endl;
    cout << " FINAL PERFORMANCE SUMMARY" << endl;
    cout << "============================================================" << endl;
    cout << "| Query Type     | Best Data Structure                | Speedup vs Naive |" << endl;
    cout << "|----------------|------------------------------------|------------------|" << endl;
        cout << "| Exact Search   | " << setw(34) << left << bestExact << " | " << fixed << setprecision(2)
            << (double)s1NaiveSafe / (double)s1BestSafe << "x             |" << endl;
        cout << "| Prefix Search  | " << setw(34) << left << bestPrefix << " | " << fixed << setprecision(2)
            << (double)s2NaiveSafe / (double)s2BestSafe << "x             |" << endl;
        cout << "| Range Query    | " << setw(34) << left << bestRange << " | " << fixed << setprecision(2)
            << (double)s3NaiveSafe / (double)s3BestSafe << "x             |" << endl;
        cout << "| Top-K Query    | " << setw(34) << left << bestTopK << " | " << fixed << setprecision(2)
            << (double)s4NaiveSafe / (double)s4BestSafe << "x             |" << endl;

    cout << "\nCONCLUSION:" << endl;
    cout << "Different query types require different data structures." << endl;
    cout << "Using a single approach (even B+ Tree) for all queries" << endl;
    cout << "leaves performance on the table." << endl;
    cout << "Selecting the right index for each query type gives" << endl;
    cout << "significant gains - especially as data scales up." << endl;

    display.pressEnterToContinue();
}

// This function runs performance test across dataset sizes.
void datasetSizeTest(vector<Content>& titles) {
    display.printScenarioHeader("DATASET SIZE PERFORMANCE TEST (Prefix Search)");

    vector<int> sizes;
    sizes.push_back(100);
    sizes.push_back(500);
    sizes.push_back(1000);
    sizes.push_back(3000);
    sizes.push_back((int)titles.size());

    vector<long long> linearTimes;
    vector<long long> bptTimes;
    vector<long long> trieTimes;

    string prefix = "Str";
    for (int i = 0; i < (int)sizes.size(); i++) {
        int n = sizes[i];
        if (n > (int)titles.size()) {
            n = (int)titles.size();
        }

        vector<Content> arr;
        for (int j = 0; j < n; j++) {
            arr.push_back(titles[j]);
        }

        LinearSearch linear;
        BPlusTreeStr bpt;
        Trie trie;
        bpt.buildIndex(arr);
        trie.buildIndex(arr);

        long long t1 = 0, t2 = 0, t3 = 0;
        BENCH(linear.prefixSearch(arr, prefix);, 1000, t1)
        BENCH(bpt.prefixSearch(prefix);, 1000, t2)
        BENCH(trie.prefixSearch(prefix);, 1000, t3)

        linearTimes.push_back(t1);
        bptTimes.push_back(t2);
        trieTimes.push_back(t3);

    }

        cout << left << setw(12) << "Dataset"
            << setw(14) << "Linear(ns)"
            << setw(14) << "B+Tree(ns)"
            << setw(14) << "Trie(ns)"
         << "Winner" << endl;
    cout << "------------------------------------------------------------------" << endl;

    for (int i = 0; i < (int)sizes.size(); i++) {
        vector<string> names;
        names.push_back("Linear");
        names.push_back("B+ Tree");
        names.push_back("Trie");
        vector<long long> times;
        times.push_back(linearTimes[i]);
        times.push_back(bptTimes[i]);
        times.push_back(trieTimes[i]);
        string winner = bestOfWithTie(names, times);

        cout << left << setw(12) << (to_string(sizes[i]) + " titles")
             << setw(14) << linearTimes[i]
             << setw(14) << bptTimes[i]
             << setw(14) << trieTimes[i]
             << winner << endl;
    }

    cout << "\nNote: values are average nanoseconds per operation (ns/op)." << endl;
    cout << "Tie labels are shown for statistically indistinguishable results." << endl;
    cout << "\nOBSERVATION:" << endl;
    cout << "Naive approach grows linearly - gets worse with more data." << endl;
    cout << "B+ Tree grows logarithmically - manageable." << endl;
    cout << "Trie stays nearly constant - prefix length drives time." << endl;

    display.pressEnterToContinue();
}

// This function prints static complexity analysis screen.
void complexityAnalysis() {
    cout << "\n============================================================" << endl;
    cout << " COMPLEXITY ANALYSIS" << endl;
    cout << "============================================================" << endl;
    display.printComplexityTable();

    cout << "\nINSIGHT:" << endl;
    cout << "No single DS is best for every operation." << endl;
    cout << "B+ Tree is the best general purpose index - decent at everything." << endl;
    cout << "Specialized DS win decisively for specific query types." << endl;
    cout << "Trie for prefix, Hash Table for exact, Segment Tree for range count." << endl;
    cout << "This is why MySQL (B+ Tree) is used alongside Redis and Elasticsearch." << endl;

    display.pressEnterToContinue();
}

// This is the main driver function for CineSearch terminal app.
int main() {
    display.printBanner();

        cout << "\nThis project focuses on analyzing how different data structures"
            << " impact the performance of various query operations." << endl;
        cout << "Using a real dataset, we simulate common tasks like searching,"
            << " filtering, and retrieving top results." << endl;

        cout << "\nOur Goal:" << endl;
        cout << "To identify the most efficient data structure for each type of"
            << " query based on performance and behavior." << endl;

        cout << "\nKey Points:" << endl;
        cout << "- Compare brute force (linear scan) with optimized approaches." << endl;
        cout << "- Evaluate indexing methods and specialized data structures." << endl;
        cout << "- Understand how query type influences data structure choice." << endl;

    vector<Content> titles = loadCSV("netflix_titles.csv");

    Trie trie;
    trie.buildIndex(titles);

    SegmentTree segTree;
    segTree.buildIndex(titles);

    MaxHeap heap;
    heap.buildIndex(titles);

    BPlusTreeStr bptStr;
    bptStr.buildIndex(titles);

    BPlusTreeInt bptInt;
    bptInt.buildIndex(titles);

    HashTable hashTable;
    hashTable.buildIndex(titles);

    AVLTree avlTree;
    avlTree.buildIndex(titles);

    SkipList skipList;
    skipList.buildIndex(titles);

    LinearSearch linear;

    int choice = -1;
    while (choice != 0) {
        display.printMenu();
        cout << "Enter choice (0-7): ";
        cin >> choice;

        switch (choice) {
            case 1:
                scenario1_ExactSearch(titles, bptStr, hashTable, avlTree, linear);
                break;
            case 2:
                scenario2_PrefixSearch(titles, bptStr, trie, linear);
                break;
            case 3:
                scenario3_RangeQuery(titles, bptInt, segTree, skipList, linear);
                break;
            case 4:
                scenario4_TopK(titles, bptInt, heap, linear);
                break;
            case 5:
                compareAllMethods(titles, bptStr, hashTable, avlTree, trie, bptInt, segTree, skipList, heap, linear);
                break;
            case 6:
                datasetSizeTest(titles);
                break;
            case 7:
                complexityAnalysis();
                break;
            case 0:
                cout << "Exiting CineSearch." << endl;
                break;
            default:
                display.printWarn("Enter 0-7 only.");
        }
    }

    return 0;
}
