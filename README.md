# Numbinator

A project for my second year of computer science degree. It
is composed of a library to manipulate numbers of any size
and a library of an interpreter.

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
./build/app/app -i [INPUT-FILE] -o [OUTPUT-FILE]
```

4. run the tests

```shell
cmake --build build --target test
```