#@ ----------------------------------------------------------------------
#@ This is my university project, where I implement elements of graph
#@ theory with C++. You are now reading the help menu of my automated
#@ Makefile, which automatically generates a build target for every .cpp
#@ test file in the tests directory.
#@ ----------------------------------------------------------------------

.SUFFIXES: .cpp .x .o .json
CONFIG_FILE = config.yaml


# COMPILER
KERNEL_NAME := $(shell uname -s)
ifeq ($(KERNEL_NAME),Darwin)
    CXX = g++-15
    VENV = .venv/bin/# WARNING THIS IS ONLY TO MAKE TESTS FOR ME EASIER
else
    CXX = g++
endif
CXXFLAGS = -std=c++20 -pedantic -Wall -Wextra
LD = $(CXX)
LFLAGS = $(CXXFLAGS)


# AVAILABLE TESTS
TESTS = classes iterator bfs dfs tarjan connected
# TEST EXECUTABLE NAME PATTERN
define test_name_pattern
graph_$(1)_test.x
endef


# PYTHON SCRIPTS DIRECTORY (HARDCODED)
SCRIPT_DIR = scripts
# CODE DIRECTORIES
LIB_DIR := $(shell python3 $(SCRIPT_DIR)/config_parser.py SOURCE_DIRECTORY)
TESTS_DIR := $(shell python3 $(SCRIPT_DIR)/config_parser.py TESTS_DIRECTORY)
OBJ_DIR := $(shell python3 $(SCRIPT_DIR)/config_parser.py OBJECT_DIRECTORY)
EXE_DIR := $(shell python3 $(SCRIPT_DIR)/config_parser.py EXECUTABLES_DIRECTORY)

# EXPORT DIRECTORIES
JSON_DIR := $(shell python3 $(SCRIPT_DIR)/config_parser.py JSON_DIRECTORY)
IMG_DIR := $(shell python3 $(SCRIPT_DIR)/config_parser.py IMAGE_DIRECTORY)
REPORT_DIR := $(shell python3 $(SCRIPT_DIR)/config_parser.py REPORT_DIRECTORY)


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
define generate_run_target
.PHONY: test_$(1)
test_$(1): $($(1)_test_exe)
	./$(EXE_DIR)/$($(1)_test_exe)
endef

$(foreach test,$(TESTS),$(eval $(call generate_run_target,$(test))))



# Generate all rule
.PHONY: all
all: ## 	 Generates executables for all tests
all: $(foreach test,$(TESTS),$($(test)_test_exe))

.PHONY: clnex # FIXME previous target name was much better but double tabulator didn't handle it well
clnex: ## Removes all json, png and report files
	rm -f $(IMG_DIR)/*
	rm -f $(REPORT_DIR)/*
	rm -f $(JSON_DIR)/*

.PHONY: clean
clean: ## Deletes all generated object files
	rm -f $(OBJ_DIR)/*.o
	rm -f $(LIB_DIR)/*.o # TODO move lib obj files to obj folder

.PHONY: pyvis # FIXME previous target name was much better but double tabulator didn't handle it well
pyvis: ## Run a Python graph visualiser script
	$(VENV)python3 $(SCRIPT_DIR)/graph_visualiser.py

.PHONY: help # TODO improve this
help: ## Show this help.
	@sed -ne '/@sed/!s/#@ //p' $(MAKEFILE_LIST)
	@echo \> Available test targets: # TODO make actual descriptions for tests
	@$(foreach t,$(TESTS), printf "\t* %s\n" "$(t)";)
	@echo \(?\) To run them use \"make test_\<test_name\>\" \(for example make test_dfs\)
	@echo
	@echo \> Utility targets:
	@sed -ne '/@sed/!{/##/{s/##/\t/; s/^/\t* /; p;};}' $(MAKEFILE_LIST) # TODO replace with something where i can control formatting better
	@echo ----------------------------------------------------------------------


.PHONY: test
test:
	$(shell sed $(MAKEFILE_LIST))
	$(shell sed $(MAKEFILE_LIST))
