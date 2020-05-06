# Project Name (executable)
PROJECT = part2proj
# Compiler
CC = g++

# Run Options
GENERATE_INPUT = GenerateInput 50000 1 10000000000 300 3200 0.3

# Compiler options during compilation
COMPILE_OPTIONS = -ansi -pedantic -Wall -std=c++14 -Wno-unused-variable -Wno-unused-parameter

#Header include directories
HEADERS =
#Libraries for linking
LIBS =

# Dependency options
DEPENDENCY_OPTIONS = -MM

#-- Do not edit below this line --

# Subdirs to search for additional source files
SUBDIRS := $(shell ls -F | grep "\/" )
DIRS := ./ $(SUBDIRS)
#SOURCE_FILES := $(foreach d, $(DIRS), $(wildcard $(d)*.cpp) )
SOURCE_FILES := $(shell find . -name '*.cpp')

# Create an object file of every cpp file
OBJECTS = $(patsubst %.cpp, %.o, $(SOURCE_FILES))

# Dependencies
DEPENDENCIES = $(patsubst %.cpp, %.d, $(SOURCE_FILES))

# Create .d files
%.d: %.cpp
	$(CC) $(DEPENDENCY_OPTIONS) $< -MT "$*.o $*.d" -MF $*.d

# Make $(PROJECT) the default target
all:$(DEPENDENCIES) $(PROJECT)

$(PROJECT): $(OBJECTS)
	$(CC) -o $(PROJECT) $(OBJECTS) $(LIBS)

# Compile every cpp file to an object
%.o: %.cpp
	$(CC) -c $(COMPILE_OPTIONS) -o $@ $< $(HEADERS)

# Build & Run Project
run: $(PROJECT)
	./$(PROJECT) $(COMMANDLINE_OPTIONS)

generateinput: $(PROJECT)
	./$(PROJECT) $(GENERATE_INPUT)

generateinput: $(PROJECT)
	./$(PROJECT) $(GENERATE_INPUT)

# Clean & Debug
.PHONY: makefile-debug
makefile-debug:

.PHONY: clean
clean:
	rm -f $(PROJECT) $(OBJECTS)
	rm -f $(PROJECT) $(DEPENDENCIES)

.PHONY: depclean
depclean:
	rm -f $(DEPENDENCIES)

clean-all: clean depclean

test:
	$(SOURCE_FILES)
