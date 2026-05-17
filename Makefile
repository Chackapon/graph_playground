.SUFFIXES: .cpp .x .o .json

CXX = g++
CXXFLAGS = -std=c++20 -pedantic -Wall
#LD = $(CXX)
#LFLAGS = -Wall -std=c++11 -pedantic -O

SRC = $(wildcard lib/*.cpp)
TESTS_SRC = $(wildcard tests/*.cpp)
CLASS_TEST_SRC = tests/graph_classes_test.cpp
ITERATOR_TEST_SRC = tests/graph_iterator_test.cpp
BFS_TEST_SRC = tests/graph_bfs_test.cpp
DFS_TEST_SRC = tests/graph_dfs_test.cpp

INCLUDE = $( wildcard include/*.hpp )

OBJ = $(SRC:.cpp=.o)
CLASS_TEST_OBJ = $(CLASS_TEST_SRC:.cpp=.o)
ITERATOR_TEST_OBJ = $(ITERATOR_TEST_SRC:.cpp=.o)
BFS_TEST_OBJ = $(BFS_TEST_SRC:.cpp=.o)
DFS_TEST_OBJ = $(DFS_TEST_SRC:.cpp=.o)

CLASS_TEST_EXE = GraphTester.x
ITERATOR_TEST_EXE = IteratorTester.x
BFS_TEST_EXE = BFSTester.x
DFS_TEST_EXE = DFSTester.x


$(CLASS_TEST_EXE): $(OBJ) $(CLASS_TEST_OBJ) $(INCLUDE)
	$(CXX) $(CXXFLAGS) $(OBJ) $(CLASS_TEST_OBJ) -o $(CLASS_TEST_EXE)

$(ITERATOR_TEST_EXE): $(OBJ) $(ITERATOR_TEST_OBJ) $(INCLUDE)
	$(CXX) $(CXXFLAGS) $(OBJ) $(ITERATOR_TEST_OBJ) -o $(ITERATOR_TEST_EXE)

$(BFS_TEST_EXE): $(OBJ) $(BFS_TEST_OBJ) $(INCLUDE)
	$(CXX) $(CXXFLAGS) $(OBJ) $(BFS_TEST_OBJ) -o $(BFS_TEST_EXE)

$(DFS_TEST_EXE): $(OBJ) $(DFS_TEST_OBJ) $(INCLUDE)
	$(CXX) $(CXXFLAGS) $(OBJ) $(DFS_TEST_OBJ) -o $(DFS_TEST_EXE)


#.PHONY: all
#all: $(TARGET)


.PHONY: class_test
class_test: $(CLASS_TEST_EXE)
	./$(CLASS_TEST_EXE)

.PHONY: iterator_test
iterator_test: $(ITERATOR_TEST_EXE)
	./$(ITERATOR_TEST_EXE)

.PHONY: bfs_test
bfs_test: $(BFS_TEST_EXE)
	./$(BFS_TEST_EXE)

.PHONY: dfs_test
dfs_test: $(DFS_TEST_EXE)
	./$(DFS_TEST_EXE)


# RUN TESTS
.PHONY: test_graphs
test_graphs: clean_json clean_img class_test

.PHONY: test_iterators
test_iterators: clean_json clean_img iterator_test

.PHONY: test_bfs
test_bfs: clean_json clean_img bfs_test

.PHONY: test_dfs
test_dfs: clean_json clean_img dfs_test

all: $(CLASS_TEST_EXE) $(ITERATOR_TEST_EXE) $(BFS_TEST_EXE) $(DFS_TEST_EXE)


# PYTHON
.PHONY: visualise
visualise:
	.venv/bin/python3 graph_visualiser.py


# CLEANS
.PHONY: clean
clean:
	rm -f tests/*.o $(TARGET)
	rm -f lib/*.o $(TARGET)

.PHONY: clean_json
clean_json:
	rm -f json/*.json

.PHONY: clean_img
clean_img:
	rm -f img/*

.PHONY: clean_report
clean_img:
	rm -f raport/*
