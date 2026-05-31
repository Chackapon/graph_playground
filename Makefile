.SUFFIXES: .cpp .x .o .json

CXX = g++-15
#CXX = g++
CXXFLAGS = -std=c++20 -pedantic -Wall -Wextra
#LD = $(CXX)
#LFLAGS = -std=c++20 -pedantic -Wall -Wextra
EXE_DIR = exe

LIB_SRC = $(wildcard lib/*.cpp)
LIB_OBJ = $(LIB_SRC:.cpp=.o)

TESTS = classes iterator bfs dfs tarjan connected shortest_path


# Generate src tests variables
$(foreach test,$(TESTS), \
	$(eval\
		$(test)_test_src = tests/graph_$(test)_test.cpp \
	)\
)

# Generate object files variables for tests
$(foreach test,$(TESTS), \
	$(eval\
		$(test)_test_obj = obj/graph_$(test)_test.o \
	)\
)

# Generate dependencies files variables for tests
$(foreach test,$(TESTS), \
	$(eval\
		$(test)_test_dep = obj/graph_$(test)_test.d \
	)\
)

# Generate names for executable files
$(foreach test,$(TESTS), \
	$(eval\
		$(test)_test_exe = exe/graph_$(test)_test.x \
	)\
)

# Rule for generating object files for tests
obj/%.o: tests/%.cpp
	mkdir -p obj
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@


# Rule for including dependencies files for tests
-include $(foreach test,$(TESTS),$($(test)_test_dep))

# Generate executable building rules for all tests
$(foreach test,$(TESTS),$(eval \
	$($(test)_test_exe): $($(test)_test_obj) $(LIB_OBJ);\
		mkdir -p exe ;\
		$(CXX) $(CXXFLAGS) $$^ -o $$@;\
))

# Generate make run command for all test executables
.PHONY: $(foreach test,$(TESTS),run_$(test)_test)
$(foreach test,$(TESTS),$(eval \
	run_$(test)_test : $($(test)_test_exe) ; \
		./$($(test)_test_exe) \
))

# Generate all rule
all: $(foreach test,$(TESTS),$($(test)_test_exe))

clean_export:
	rm -f img/*
	rm -f report/*
	rm -f json/*

clean:
	rm -f obj/*.o
	rm -f lib/*.o # TODO move lib obj files to obj folder
