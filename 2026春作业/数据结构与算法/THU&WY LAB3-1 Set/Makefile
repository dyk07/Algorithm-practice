CXX := g++
CXXFLAGS := -g -O2

SRCS := $(shell find lib -name "*.cpp")
HEADERS := $(shell find lib -name "*.h")

TARGET := my_stdset my_skiplist my_bst my_splay_single my_splay_double battle rand_input

all: $(TARGET)

my_stdset: my_stdset.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^ 

my_skiplist: $(HEADERS) my_skiplist.cpp
	$(CXX) $(CXXFLAGS) -o $@ my_skiplist.cpp

my_bst: $(HEADERS) my_bst.cpp
	$(CXX) $(CXXFLAGS) -o $@ my_bst.cpp

my_splay_single: $(HEADERS) my_splay_single.cpp
	$(CXX) $(CXXFLAGS) -o $@ my_splay_single.cpp

my_splay_double: $(HEADERS) my_splay_double.cpp
	$(CXX) $(CXXFLAGS) -o $@ my_splay_double.cpp

battle: battle.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^ 

rand_input: rand_input.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^ 

clean:
	rm -f *.o $(TARGET)
