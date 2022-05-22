.DEFAULT_GOAL: help

COMPILER=gcc
OPTIONS=-std=gnu11 -Iinclude -pedantic -Wall -g -Wextra -Wshadow -Wconversion -Wunreachable-code
COMPILE=$(COMPILER) $(OPTIONS) -O3
SOURCE_FILES = $(wildcard src/*.c)
OBJECT_FILES = $(patsubst src/%.c,build/%.o,$(SOURCE_FILES))

help:
	@echo "Usage: make [target]"
	@echo "Available targets:"
	@echo "  library:     Build the library of the project"
	@echo "  build:       Build the command line program"
	@echo "  all:         Alias for the 'build' target"
	@echo "  run-example: Run the program with a sample input"
	@echo "  test:        Run the tests"
	@echo "  clean:       Clean the build directory"
	@echo "  help:        Print this message"

build/%.o: src/%.c include/%.h
	@mkdir -p build
	$(COMPILE) -c $< -o $@

build/library.a: $(OBJECT_FILES)
	ar -rcs build/library.a $(OBJECT_FILES)

build/tests: build/library.a test/tests.c test/test_unbounded_int.c test/test_unbounded_int.h
	$(COMPILE) -o build/tests test/tests.c test/test_unbounded_int.c build/library.a

library: build/library.a
	@make build/tests
	@./build/tests > build/tests.log || (printf "\033[31mTests failed (see tests.log)\033[0m\n" && exit 1)
	@printf "\033[32mTests passed\n\033[0m"

build/cli: build/library.a
	$(COMPILE) -o build/cli cli/main.c build/library.a

test: build/tests
	./build/tests

build: build/cli library
	@printf "\033[32mYou can now run ./build/cli\n\033[0m"

all: build/tests build

run-example: build/cli
	@printf "========================================================\n"
	@echo -e '\nvar = 5 / 2\nprint var' \
			 '\nvar = 5 * 2\nprint var' \
			 '\nvar = 5 - 2\nprint var' \
			 '\nvar = 5 + 2\nprint var' \
			 '\nvar = 5^2\nprint var' \
			 '\nvar2 = 10^10\nprint var2' \
			 '\nbbbbbbbbbb = var2 - 74 * 96 + -12\nprint bbbbbbbbbb' \
			 '\nvar = 54786 * 854 + var2 / 7^3 + -5963 - bbbbbbbbbb\nprint var\n' > input.txt
	./build/cli -i input.txt -o output.txt
	cat output.txt
	@rm -f input.txt output.txt
	@printf "========================================================\n"

clean:
	rm -rf build
