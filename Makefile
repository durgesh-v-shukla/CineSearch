CXX = g++
CXXFLAGS = -std=c++17 -O0 -Wall -Iinclude

SOURCES = src/main.cpp src/LinearSearch.cpp src/BPlusTreeStr.cpp src/BPlusTreeInt.cpp \
          src/HashTable.cpp src/AVLTree.cpp src/Trie.cpp src/SegmentTree.cpp \
		  src/SkipList.cpp src/MaxHeap.cpp src/Display.cpp

all:
	$(CXX) $(CXXFLAGS) $(SOURCES) -o cinesearch.exe

clean:
	rm -f cinesearch cinesearch.exe cinesearch_new.exe qa_*.txt
