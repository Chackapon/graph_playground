.SUFFIXES: .cpp .x .o .json

CXX = g++
CXXFLAGS = -std=c++11 -pedantic -Wall
#LD = $(CXX)
#LFLAGS = -Wall -std=c++11 -pedantic -O

SRC = $(wildcard lib/*.cpp)
TESTS_SRC = $(wildcard tests/*.cpp)
CLASS_TEST_SRC = tests/graph_classes_test.cpp
ITERATOR_TEST_SRC = tests/graph_iterator_test.cpp

INCLUDE = $( wildcard include/*.hpp )

OBJ = $(SRC:.cpp=.o)
CLASS_TEST_OBJ = $(CLASS_TEST_SRC:.cpp=.o)
ITERATOR_TEST_OBJ = $(ITERATOR_TEST_SRC:.cpp=.o)

CLASS_TEST_EXE = GraphTester.x
ITERATOR_TEST_EXE = IteratorTester.x


$(CLASS_TEST_EXE): $(OBJ) $(CLASS_TEST_OBJ) $(INCLUDE)
	$(CXX) $(CXXFLAGS) $(OBJ) $(CLASS_TEST_OBJ) -o $(CLASS_TEST_EXE)

$(ITERATOR_TEST_EXE): $(OBJ) $(ITERATOR_TEST_OBJ) $(INCLUDE)
	$(CXX) $(CXXFLAGS) $(OBJ) $(ITERATOR_TEST_OBJ) -o $(ITERATOR_TEST_EXE)


#.PHONY: all
#all: $(TARGET)


.PHONY: class_test
class_test: $(CLASS_TEST_EXE)
	./$(CLASS_TEST_EXE)

.PHONY: iterator_test
iterator_test: $(ITERATOR_TEST_EXE)
	./$(ITERATOR_TEST_EXE)


# RUN TESTS
.PHONY: test_graphs
test_graphs: clean_json clean_img class_test

.PHONY: test_iterators
test_iterators: clean_json clean_img iterator_test


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
