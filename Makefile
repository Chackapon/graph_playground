.SUFFIXES: .cpp .x .o .json

CXX = g++
CXXFLAGS = -std=c++20 -pedantic -Wall
#LD = $(CXX)
#LFLAGS = -Wall -std=c++11 -pedantic -O
EXE_DIR = exe

SRC = $(wildcard lib/*.cpp)

TESTS_SRC = $(wildcard tests/*.cpp)
TESTS = CLASS ITERATOR BFS DFS

CLASS_TEST_SRC = tests/graph_classes_test.cpp
ITERATOR_TEST_SRC = tests/graph_iterator_test.cpp
BFS_TEST_SRC = tests/graph_bfs_test.cpp
DFS_TEST_SRC = tests/graph_dfs_test.cpp

INCLUDE = $( wildcard include/*.hpp )


#CLASS_TEST_OBJ = $(CLASS_TEST_SRC:.cpp=.o)
#ITERATOR_TEST_OBJ = $(ITERATOR_TEST_SRC:.cpp=.o)
#BFS_TEST_OBJ = $(BFS_TEST_SRC:.cpp=.o)
#DFS_TEST_OBJ = $(DFS_TEST_SRC:.cpp=.o)

OBJ = $(SRC:.cpp=.o)
# Generate object files for every test file
$(foreach test,$(TESTS), \
  $(eval $(test)_TEST_OBJ := $($(test)_TEST_SRC:.cpp=.o)) \
)

CLASS_TEST_EXE = $(EXE_DIR)/GraphTester.x
ITERATOR_TEST_EXE = $(EXE_DIR)/IteratorTester.x
BFS_TEST_EXE = $(EXE_DIR)/BFSTester.x
DFS_TEST_EXE = $(EXE_DIR)/DFSTester.x
# Generate executable rules for every test
$(foreach test,$(TESTS), \
  $(eval $($(test)_TEST_EXE): $(OBJ) $($(test)_TEST_OBJ) $(INCLUDE) ; \
  	mkdir -p $(EXE_DIR) ; \
    $(CXX) $(CXXFLAGS) $(OBJ) $($(test)_TEST_OBJ) -o $$@) \
)

#$(CLASS_TEST_EXE): $(OBJ) $(CLASS_TEST_OBJ) $(INCLUDE)
#	$(CXX) $(CXXFLAGS) $(OBJ) $(CLASS_TEST_OBJ) -o $(CLASS_TEST_EXE)
#
#$(ITERATOR_TEST_EXE): $(OBJ) $(ITERATOR_TEST_OBJ) $(INCLUDE)
#	$(CXX) $(CXXFLAGS) $(OBJ) $(ITERATOR_TEST_OBJ) -o $(ITERATOR_TEST_EXE)
#
#$(BFS_TEST_EXE): $(OBJ) $(BFS_TEST_OBJ) $(INCLUDE)
#	$(CXX) $(CXXFLAGS) $(OBJ) $(BFS_TEST_OBJ) -o $(BFS_TEST_EXE)
#
#$(DFS_TEST_EXE): $(OBJ) $(DFS_TEST_OBJ) $(INCLUDE)
#	$(CXX) $(CXXFLAGS) $(OBJ) $(DFS_TEST_OBJ) -o $(DFS_TEST_EXE)


.PHONY: run_class_test
run_class_test: $(CLASS_TEST_EXE)
	./$(CLASS_TEST_EXE)

.PHONY: run_iterator_test
run_iterator_test: $(ITERATOR_TEST_EXE)
	./$(ITERATOR_TEST_EXE)

.PHONY: run_bfs_test
run_bfs_test: $(BFS_TEST_EXE)
	./$(BFS_TEST_EXE)

.PHONY: run_dfs_test
run_dfs_test: $(DFS_TEST_EXE)
	./$(DFS_TEST_EXE)


# RUN TESTS
.PHONY: test_graphs
test_graphs: clean_json clean_img run_class_test

.PHONY: test_iterators
test_iterators: clean_json clean_img run_iterator_test

.PHONY: test_bfs
test_bfs: clean_json clean_img run_bfs_test

.PHONY: test_dfs
test_dfs: clean_json clean_img run_dfs_test

#all: $(CLASS_TEST_EXE) $(ITERATOR_TEST_EXE) $(BFS_TEST_EXE) $(DFS_TEST_EXE)
all: $(foreach test,$(TESTS),$($(test)_TEST_EXE))


# PYTHON
.PHONY: visualise
visualise:
	.venv/bin/python3 graph_visualiser.py


# CLEANS
.PHONY: clean
clean:
	rm -f tests/*.o
	rm -f lib/*.o
	rm -f exe/*.x

.PHONY: clean_json
clean_json:
	rm -f json/*.json

.PHONY: clean_img
clean_img:
	rm -f img/*

.PHONY: clean_report
clean_report:
	rm -f raport/*
