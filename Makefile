.SUFFIXES: .cpp .x .o .json

CXX = g++-15
#CXX = g++
CXXFLAGS = -std=c++20 -pedantic -Wall -Wextra
#LD = $(CXX)
#LFLAGS = -Wall -std=c++11 -pedantic -O
EXE_DIR = exe

SRC = $(wildcard lib/*.cpp)
TESTS_SRC = $(wildcard tests/*.cpp)
TESTS = CLASS ITERATOR BFS DFS TAJRAN CONNECTION

CLASS_TEST_SRC = tests/graph_classes_test.cpp
ITERATOR_TEST_SRC = tests/graph_iterator_test.cpp
BFS_TEST_SRC = tests/graph_bfs_test.cpp
DFS_TEST_SRC = tests/graph_dfs_test.cpp
TAJRAN_TEST_SRC = tests/graph_tajran_test.cpp
CONNECTION_TEST_SRC = tests/graph_connected_test.cpp

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
TAJRAN_TEST_EXE = $(EXE_DIR)/TajranTester.x
CONNECTION_TEST_EXE = $(EXE_DIR)/GraphConnectionTester.x
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

.PHONY: run_tajran_test
run_tajran_test: $(TAJRAN_TEST_EXE)
	./$(TAJRAN_TEST_EXE)

.PHONY: run_connection_test
run_connection_test: $(CONNECTION_TEST_EXE)
	./$(CONNECTION_TEST_EXE)


ZADANIE3i4_CONTENT := BaseGraph ListGraph MatrixGraph ReportMaker HelperFunctions GraphExceptions Edge
ZADANIE5_CONTENT := BaseGraph ListGraph ReportMaker HelperFunctions GraphExceptions Edge DFS GraphSearchAlgorithm


.PHONY: package_class_test
package_class_test:
	mkdir -p Zadanie3i4 Zadanie3i4/include Zadanie3i4/tests Zadanie3i4/lib zadania
	-cp $(addprefix include/,$(addsuffix .hpp,$(ZADANIE3i4_CONTENT))) Zadanie3i4/include
	-cp $(addprefix lib/,$(addsuffix .cpp,$(ZADANIE3i4_CONTENT))) Zadanie3i4/lib
	cp tests/graph_classes_test.cpp Zadanie3i4/tests
	cp other/Makefile graph_visualiser.py requirements.txt Zadanie3i4
	tar -vczf zadania/Zadanie3i4.tar.gz Zadanie3i4
	rm -rf Zadanie3i4

.PHONY: package_dfs_test
package_dfs_test:
	mkdir -p Zadanie5 Zadanie5/include Zadanie5/tests Zadanie5/lib zadania
	-cp $(addprefix include/,$(addsuffix .hpp,$(ZADANIE5_CONTENT))) Zadanie5/include
	-cp $(addprefix lib/,$(addsuffix .cpp,$(ZADANIE5_CONTENT))) Zadanie5/lib
	cp tests/graph_dfs_test.cpp Zadanie5/tests
	cp other/Makefile graph_visualiser.py requirements.txt Zadanie5
	tar -vczf zadania/Zadanie5.tar.gz Zadanie5
	rm -rf Zadanie5


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
	#python3 graph_visualiser.py

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
	rm -f report/*
