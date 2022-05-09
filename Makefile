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

library: build/library.a
	@make build/tests
	@./build/tests > build/tests.log || (printf "\033[31mTests failed (see tests.log)\033[0m\n" && exit 1)
	@printf "\033[32mTests passed\n\033[0m"

build/cli: library
	$(COMPILE) -o build/cli cli/main.c build/library.a

test: build/tests
	./build/tests

build: build/cli
	@printf "\033[32mYou can now run ./build/cli\n\033[0m"

clean:
	rm -rf build