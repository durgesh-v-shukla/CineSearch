# CineSearch - Smart Query Optimization Engine

## 1. What This Project Is
CineSearch is a DS-II course project built on a real Netflix titles dataset. The core idea is simple:

- One query workload is not best served by one data structure.
- Industry defaults (like B+ Tree indexing) are strong general-purpose choices.
- Query-specific data structures can outperform general indexing for specific operations.

So this project does not just implement data structures. It compares them under real query patterns, measures runtime, and shows where each approach wins.

## 2. Thinking Behind the Project
Most systems use one dominant indexing strategy across many query types. This is practical, but not always optimal. CineSearch asks:

- What if we keep industry-standard indexing as baseline?
- What if we add specialized structures for each query pattern?
- How much difference do we observe in actual runtime?

This becomes an evidence-based design exercise instead of a theory-only exercise.

## 3. Unique Research / Innovative Approach
The innovation in this project is a hybrid benchmark-driven indexing strategy:

1. Baseline approach: Brute-force (linear scan)
2. Industry baseline: B+ Tree indexing (general-purpose)
3. Proposed approach: Query-specialized DS per operation

Rather than claiming one DS is globally best, this project proves a more practical rule:

- Choose DS by query shape, not by popularity.

This is an important systems design mindset and matches how production systems combine multiple engines (for example, SQL + cache + search index).

## 4. Why This Is Necessary in the Real World
In real products (OTT, e-commerce, social platforms), requests are mixed:

- Exact lookup
- Prefix/autocomplete
- Range filter
- Top-K retrieval

If all are forced onto one index, the system is simpler but can leave performance on the table.

CineSearch shows why specialized structures matter:

- Faster response time
- Better scalability as data grows
- Better fit for query intent

## 5. Real-World Use Cases Mapped to This Project
- Exact search (title/id): catalog lookup, key-value access, metadata service
- Prefix search: live search suggestions, autocomplete UI, instant search
- Range query (year): faceted filtering, analytics filters, date/time slicing
- Top-K latest: trending/new content lists, recommendation candidates, feed pre-ranking

## 6. Syllabus Analysis and Coverage (CS2308 DS-II)
Below is how your project maps to syllabus topics.

### Section 1 - Advanced Trees, Priority Queues, Strings
- Unit 1 (Advanced Trees):
  - AVL Tree: implemented and benchmarked
  - B+ Tree: implemented (string and int variants) and benchmarked
- Unit 2 (Priority Queues and Heaps):
  - Max-Heap: implemented and benchmarked for Top-K retrieval
- Unit 3 (Data Structures for Strings):
  - Trie: implemented and benchmarked for prefix search

### Section 2 - Randomized, Multidimensional, Miscellaneous
- Unit 4 (Randomized DS):
  - Skip List: implemented and benchmarked for range query
- Unit 5 (Multidimensional DS):
  - Segment Tree: implemented and benchmarked for range counting
- Unit 6 (Miscellaneous):
  - Union-Find was part of earlier iterations but removed from final menu and code path to keep focus tight

### Also Used (supporting comparisons)
- Hash Table: exact search baseline/proposed candidate
- Linear scan: naive baseline for all scenarios

So the project is strong for DS-II because it combines implementation + comparative performance + problem-fit reasoning.

## 7. Why This Is a Strong Course Project Demonstration
This project demonstrates all expected DS-II outcomes:

- Implementation skills: custom structures in C++
- Applied problem solving: each DS tied to a query type
- Experimental thinking: benchmark-based comparison
- System design thinking: no single DS forced for all workloads

This aligns with the course objectives about selecting, applying, and adapting advanced data structures for specialized needs.

## 8. Project Structure

```text
DS Project/
  include/
    AVLTree.h
    BPlusTreeInt.h
    BPlusTreeStr.h
    Content.h
    data.h
    Display.h
    HashTable.h
    LinearSearch.h
    MaxHeap.h
    SegmentTree.h
    SkipList.h
    Trie.h

  src/
    AVLTree.cpp
    BPlusTreeInt.cpp
    BPlusTreeStr.cpp
    Display.cpp
    HashTable.cpp
    LinearSearch.cpp
    MaxHeap.cpp
    SegmentTree.cpp
    SkipList.cpp
    Trie.cpp
    main.cpp

  netflix_titles.csv
  Makefile
  README.md
```

## 9. Build and Run

### Build (Windows + g++)
```bash
g++ -std=c++17 -O0 -Wall -Iinclude src/main.cpp src/LinearSearch.cpp src/BPlusTreeStr.cpp src/BPlusTreeInt.cpp src/HashTable.cpp src/AVLTree.cpp src/Trie.cpp src/SegmentTree.cpp src/SkipList.cpp src/MaxHeap.cpp src/Display.cpp -o cinesearch.exe
```

### Run
```bash
./cinesearch.exe
```

## 10. Startup Intro and Menu Behavior
The startup explanation now appears one time only at launch. It is not repeated after every menu cycle.

Main menu options:

1. Exact Search (Title / ID)
2. Autocomplete Search (Prefix)
3. Range Query (Year Filter)
4. Top-K Query (Latest Content)
5. Compare All Methods
6. Dataset Size Performance Test
7. Complexity Analysis
0. Exit

## 11. Benchmark Methodology (Important)
To keep results real and reduce fake zero-time artifacts, benchmarking uses:

- Adaptive repetitions (reps grow until at least ~2 ms timing window)
- Warmup runs before measurement
- Multiple trials (5 trials)
- Median trial chosen for stability
- Output shown as ns/op, us/op, or ms/op depending on magnitude

This is significantly more reliable than one-shot microsecond timing.

## 12. Detailed Option-by-Option Explanation

### Option 1 - Exact Search
Input: exact title string

What runs:
- Linear exact search
- B+ Tree exact search
- AVL exact search
- Hash Table exact search

Benchmark settings:
- Base reps passed: 10000
- Actual reps adaptively increased by timer helper if needed

What is checked:
- Point lookup efficiency
- Hash and balanced/tree index differences

Industry standard:
- B+ Tree commonly used in DB indexes

Why specialized/proposed may win:
- Hash table often wins exact key lookup due to O(1) average behavior

---

### Option 2 - Autocomplete (Prefix Search)
Input: prefix string

What runs:
- Linear prefix scan
- B+ Tree prefix scan
- Trie prefix search

Benchmark settings:
- Base reps passed: 10000

What is checked:
- Prefix-oriented query performance

Industry standard:
- B+ Tree style prefix range in DB index

Why specialized/proposed may win:
- Trie follows character path directly and is designed for prefix access

---

### Option 3 - Range Query (Year Filter)
Input: preset range or custom year range

What runs:
- Linear range filter
- B+ Tree range query
- Skip List range query
- Segment Tree range count

Benchmark settings:
- Base reps passed: 3000

What is checked:
- Range filtering and count patterns

Industry standard:
- B+ Tree range scans are common in DBs

Why specialized/proposed may win:
- Segment Tree is excellent for range aggregate/count style queries
- Skip List offers randomized O(log n + k)-style behavior for ordered access

---

### Option 4 - Top-K Latest
Input: k

What runs:
- Linear full sort then take top-k
- B+ Tree rightmost traversal for latest records
- Max-Heap top-k retrieval

Benchmark settings:
- Base reps passed: 100

What is checked:
- Efficiency of repeated top-k extraction

Industry standard:
- B+ Tree with ORDER BY + LIMIT style access

Why specialized/proposed may win:
- Heap-based approach is naturally aligned with priority retrieval and can be very efficient

---

### Option 5 - Compare All Methods
No custom input required for benchmark setup.

Default internal test cases:
- Exact: target = "Inception"
- Prefix: prefix = "Str"
- Range: 2018 to 2022
- Top-K: k = 10

What runs:
- Benchmarks all four query families sequentially
- Prints final summary table with best structure and speedup vs naive

Benchmark settings:
- Exact: 500
- Prefix: 500
- Range: 500
- Top-K: 100

What is checked:
- End-to-end comparative profile across all query families

---

### Option 6 - Dataset Size Performance Test
Focus: Prefix search scaling by dataset size.

Sizes tested:
- 100
- 500
- 1000
- 3000
- Full dataset size

What runs at each size:
- Linear prefix search
- B+ Tree prefix search
- Trie prefix search

Benchmark settings:
- Base reps passed: 1000 for each approach

What is checked:
- Growth trend with data size
- Which structure scales better for prefix workload

---

### Option 7 - Complexity Analysis
This is a static complexity table and conceptual summary.

What is checked:
- Big-O tradeoffs across implemented structures
- Reinforces that no single DS dominates all operations

## 13. Data Structures Used and Their Role in CineSearch

### 1. Linear Scan
- Role: naive baseline
- Use: all scenarios for comparison
- Value: reference point for speedup claims

### 2. B+ Tree (String + Integer variants)
- Role: industry-standard general index
- Use:
  - String index for exact/prefix title queries
  - Int index for year range and top-k by year traversal
- Value: strong baseline and practical DB-like behavior

### 3. AVL Tree
- Role: balanced BST comparison in exact search
- Use: exact title query
- Value: shows balanced tree behavior vs B+ and hash

### 4. Hash Table
- Role: exact lookup specialist
- Use: exact search
- Value: often fastest for key-based lookup

### 5. Trie
- Role: prefix-specialized string structure
- Use: autocomplete
- Value: designed for prefix traversal by characters

### 6. Segment Tree
- Role: range aggregate specialist
- Use: year range count operations
- Value: efficient range computation behavior

### 7. Skip List
- Role: randomized ordered structure
- Use: range query comparison
- Value: practical randomized alternative to balanced trees

### 8. Max-Heap
- Role: priority retrieval structure
- Use: top-k latest content
- Value: natural fit for top-priority extraction

## 14. Traditional Industry vs Proposed Specialized DS
General pattern demonstrated by this project:

- Industry standard (B+ Tree):
  - Very good all-rounder
  - Great for DB indexing and mixed read patterns

- Proposed query-specific structures:
  - Can beat B+ Tree when the query exactly matches DS design
  - Example pattern:
    - Exact key lookup -> Hash Table
    - Prefix lookup -> Trie
    - Range count -> Segment Tree
    - Top-K priority -> Max-Heap

Difference exists because each DS optimizes a different access pattern and cost model.

## 15. Why Results Differ Across Queries
Performance differs because operations differ:

- Exact lookup wants direct key resolution
- Prefix lookup wants character-wise branching
- Range query wants ordered or aggregate-aware structure
- Top-K wants fast max/prioritized retrieval

So the fastest structure changes with query semantics.

## 16. Interpreting Output Correctly
When you run a scenario, read output in this order:

1. Functional result correctness (records found)
2. Performance table (time per operation)
3. Best approach line
4. Explanation block (why that DS behavior fits)

Key point: best approach may change slightly run-to-run for very close timings, especially on shared OS scheduling. This is expected in real benchmarking.

## 17. Why This Project Is Innovative Yet Practical
This project is not innovation for novelty alone. It is practical innovation:

- Uses real data
- Uses implemented DS from syllabus
- Compares baseline vs industry vs specialized approach
- Produces measurable evidence
- Teaches design decisions, not just coding

That is exactly the kind of project that demonstrates DS-II maturity.

## 18. Limitations and Honest Scope
- Dataset is moderate size, not web-scale
- Benchmarks are in-memory, not true disk-page DB simulation
- Workload set is representative but not exhaustive

Still, for an academic DS-II project, the comparison framework is strong and meaningful.

## 19. Future Scope (Optional Extension Ideas)
- Add Red-Black Tree and compare with AVL on dynamic updates
- Add Fibonacci/Binomial heap for advanced priority queue study
- Add suffix array/tree for deeper string search analysis
- Add spatial structures (R-Tree/QuadTree) for multidimensional queries
- Add update-heavy workloads and mixed read-write benchmarks

## 20. Final Conclusion
CineSearch demonstrates a core advanced-data-structures insight:

- There is no single best DS for every query.
- B+ Tree is a strong industry default.
- Query-specialized structures can deliver clear gains when matched correctly.

So the best system design is often hybrid: keep a stable general index and augment with specialized structures where workload justifies it.

That is the central technical and research contribution of this project.