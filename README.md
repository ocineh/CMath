# Numbinator

A project for my second year of computer science degree. It
is composed of a library to manipulate numbers of any size
and a library of an interpreter.

## Detailed description

The project is divided into three parts, the library, the
tests and the application. The library is composed of
functions to manipulate numbers of all sizes and functions
for an interpreter.

The application can be launched with two optional options:

- `-i <FILE>` To specify the name of the file from which to
  read the instructions.
- `-o <FILE>` To specify the name of the file to which write
  the results.
- `-e <FILE>` To specify the name of the file to which write
  the errors.

By default, instructions will be read from **stdin**, the
results will be written to **stdout** and the errors will be
written to **stderr**.

## Usage

1. clone the repository

```shell
git clone https://github.com/ocineh/Numbinator.git
# or
git clone git@github.com:ocineh/Numbinator.git
```

2. build the project

```shell
cmake -S . -B build
cmake --build build
```

3. run the project

```shell
./build/app/app -i <FILE> -o <FILE> -e <FILE> 
```

4. run the tests

```shell
cmake --build build --target test
```

## Features

- application
  - [ ] Parse the arguments
  - [ ] Run the interpreter with the given arguments or
    default
  - [ ] Print the help
- library
  - Operations on unbounded numbers
    - [x] Create an unbounded_int from a string
    - [x] Create an unbounded_int from a `long long int`
    - [x] Transform an unbounded_int to a string
    - [x] Compare two unbounded_int
    - [x] Compare an unbounded_int and a `long long int`
    - [x] Calculate the sum of two unbounded_int
    - [x] Calculate the difference between two unbounded_int
    - [x] Calculate the product of two unbounded_int
  - Interpreter
    - [x] Assign a variable name with a value
    - [x] Evaluate an expression (arithmetic, comparison or assignment)
    - [x] Print a variable