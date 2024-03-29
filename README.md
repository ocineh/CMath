# CMath

A project for my second year of computer science degree. It
is composed of a library to manipulate big_ints and an interpreter.

## Summary

1. Detailed description of the project
2. The project structure
3. How to compile and run the project
4. The features of the project

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

## The project structure

```
.
├─ Makefile
├─ src/                    (The source code of the library)
│  ├─ arithmetic.c
│  ├─ interpreter.c
│  ├─ utils.c
│  └─ big_int.c
├─ include/                (The header files of the library)
│  ├─ arithmetic.h
│  ├─ interpreter.h
│  ├─ utils.h
│  └─ big_int.h
├─ test/                   (The tests of the library)
│  ├─ tests.c
│  ├─ test_big_int.c
│  └─ test_big_int.h
└─ cli/                    (The command line interface)
   └─ main.c
```

## Usage

1. clone the repository

```shell
git clone https://github.com/ocineh/CMath.git
# or
git clone git@github.com:ocineh/CMath.git
```

2. build the project or just the library

```shell
make library
# or
make build
```

3. run the project

```shell
./build/app [-i <FILE>] [-o <FILE>] [-e <FILE>] 
```

4. run the tests

```shell
make test
```

## Features

- command line interface
  - [x] Parse the arguments of the command line
  - [x] Run the interpreter with the given flags or default values
  - [x] Print the help message
- library
  - Operations on unbounded numbers
    - [x] Create a big_int from a string
    - [x] Create a big_int from a `long long int`
    - [x] Transform an big_int to a string
    - [x] Compare two big_int
    - [x] Compare a big_int and a `long long int`
    - [x] Calculate the sum of two big_int
    - [x] Calculate the difference between two big_int
    - [x] Calculate the product of two big_int
    - [x] Calculate the quotient of two big_int
    - [x] Calculate the modulo of two big_int
    - [x] Calculate the power of two big_int
  - Interpreter
    - [x] (Re)Assign a variable name with a value
    - [x] Evaluate an expression (arithmetic or assignment)
    - [x] Print a variable
    - [x] Free a variable
    - [x] Compare two variables
