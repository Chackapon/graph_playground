.SUFFIXES: .cpp .x .o .json
CONFIG_FILE = config.yaml


# COMPILER
KERNEL_NAME := $(shell uname -s)
ifeq ($(KERNEL_NAME),Darwin)
    CXX = g++-15
    VENV = .venv/bin/ # WARNING THIS IS ONLY TO MAKE TESTS FOR ME EASIER
else
    CXX = g++
endif
CXXFLAGS = -std=c++20 -pedantic -Wall -Wextra
LD = $(CXX)
LFLAGS = $(CXXFLAGS)


# AVAILABLE TESTS
TESTS = classes iterator bfs dfs tarjan connected shortest_path
# TEST EXECUTABLE NAME PATTERN
define test_name_pattern
graph_$(1)_test.x
endef


# PYTHON SCRIPTS DIRECTORY (HARDCODED)
SCRIPT_DIR = scripts
# CODE DIRECTORIES
LIB_DIR = $(shell python3 $(SCRIPT_DIR)/config_parser.py SOURCE_DIRECTORY)
TESTS_DIR = $(shell python3 $(SCRIPT_DIR)/config_parser.py TESTS_DIRECTORY)
OBJ_DIR = $(shell python3 $(SCRIPT_DIR)/config_parser.py OBJECT_DIRECTORY)
EXE_DIR = $(shell python3 $(SCRIPT_DIR)/config_parser.py EXECUTABLES_DIRECTORY)

# EXPORT DIRECTORIES
JSON_DIR = $(shell python3 $(SCRIPT_DIR)/config_parser.py JSON_DIRECTORY)
IMG_DIR = $(shell python3 $(SCRIPT_DIR)/config_parser.py IMAGE_DIRECTORY)
REPORT_DIR = $(shell python3 $(SCRIPT_DIR)/config_parser.py REPORT_DIRECTORY)


# IDEA replace with specific instructions for the libraries
LIB_SRC = $(wildcard $(LIB_DIR)/*.cpp)
LIB_OBJ = $(LIB_SRC:.cpp=.o)



# Generate src tests variables
$(foreach test,$(TESTS), \
	$(eval\
		$(test)_test_src = $(TESTS_DIR)/graph_$(test)_test.cpp \
	)\
)

# Generate object files variables for tests
$(foreach test,$(TESTS), \
	$(eval\
		$(test)_test_obj = $(OBJ_DIR)/graph_$(test)_test.o \
	)\
)

# Generate dependencies files variables for tests
$(foreach test,$(TESTS), \
	$(eval\
		$(test)_test_dep = $(OBJ_DIR)/graph_$(test)_test.d \
	)\
)

# Generate variables for executable files' names
$(foreach test,$(TESTS), \
	$(eval\
		$(test)_test_exe = $(call test_name_pattern,$(test))\
	)\
)

# Rule for generating object files for tests
$(OBJ_DIR)/%.o: $(TESTS_DIR)/%.cpp
	mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@


# Rule for including dependencies files for tests
-include $(foreach test,$(TESTS),$($(test)_test_dep))

# Generate executable building rules for all tests
$(foreach test,$(TESTS),$(eval \
	$($(test)_test_exe): $($(test)_test_obj) $(LIB_OBJ);\
		mkdir -p $(EXE_DIR) ;\
		$(LD) $(LFLAGS) $$^ -o $(EXE_DIR)/$$@;\
))

# Generate make run command for all test executables
.PHONY: $(foreach test,$(TESTS),run_$(test)_test)
$(foreach test,$(TESTS),$(eval \
	run_$(test)_test : $($(test)_test_exe) ; \
		./$(EXE_DIR)/$($(test)_test_exe) \
))



# Generate all rule
.PHONY: all
all: $(foreach test,$(TESTS),$($(test)_test_exe))

.PHONY: clean_export
clean_export:
	rm -f $(IMG_DIR)/*
	rm -f $(REPORT_DIR)/*
	rm -f $(JSON_DIR)/*

.PHONY: clean
clean:
	rm -f $(OBJ_DIR)/*.o
	rm -f $(LIB_DIR)/*.o # TODO move lib obj files to obj folder

.PHONY: visualise
visualise:
	$(VENV)python3 $(SCRIPT_DIR)/graph_visualiser.py

.PHONY: test
test:
	$(info ./$(EXE_DIR)/bfs_test_exe)
