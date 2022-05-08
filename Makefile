.DEFAULT_GOAL: help

COMPILER=gcc
OPTIONS=-std=c11 -Iinclude -pedantic -Wall -Wextra -Werror -Wshadow -Wconversion -Wunreachable-code
COMPILE=$(COMPILER) $(OPTIONS)
SOURCE_FILES = $(wildcard src/*.c)
OBJECT_FILES = $(patsubst src/%.c,build/%.o,$(SOURCE_FILES))

help:
	@echo "Usage: make [target]"
	@echo "Available targets:"
	@echo "  library: Create the library of the project"
	@echo "  build: Build the command line program"
	@echo "  test: Run the tests"
	@echo "  clean: Clean the build directory"
	@echo "  help: Print this message"

build/%.o: src/%.c include/%.h
	@mkdir -p build
	$(COMPILE) -c $< -o $@

build/library.a: $(OBJECT_FILES)
	ar -rcs build/library.a $(OBJECT_FILES)

build/tests: build/library.a $(wildcard test/*.c)
	$(COMPILE) -o build/tests test/tests.c test/test_unbounded_int.c test/test_unbounded_int.h build/library.a

build/app: build/library.a
	$(COMPILE) -o build/app app/main.c build/library.a

library: build/library.a
test: build/tests
	./build/tests
build: build/app

clean:
	rm -rf build