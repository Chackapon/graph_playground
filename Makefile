.SUFFIXES: .cpp .x .o .json

CXX = g++
CXXFLAGS = -std=c++11 -pedantic -Wall
#LD = $(CXX)
#LFLAGS = -Wall -std=c++11 -pedantic -O

SRC = $(wildcard lib/*.cpp)
MAIN_TEST_SRC = tests/graph_classes_test.cpp
INCLUDE = $( wildcard include/*.hpp )
OBJ = $(MAIN_TEST_SRC:.cpp=.o) $(SRC:.cpp=.o)

TARGET = GraphTester.x


$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(TARGET)

.PHONY: all
all: $(TARGET)

.PHONY: run
run: $(TARGET)
	./$(TARGET)

.PHONY: visualise
visualise:
	.venv/bin/python3 graph_visualiser.py

.PHONY: clean
clean:
	rm -f tests/*.o $(TARGET)

.PHONY: clean_json
clean_json:
	rm -f json/*.json

.PHONY: clean_img
clean_img:
	rm -f img/*
