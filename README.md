# 8085-Microprocessor-Simulator
8085 Microprocessor Simulator in C++

## How to build
```Shell
~$ cmake -S . -B build
~$ cmake --build build
```

## How to run
binary/executable will be generated in bin
```Shell
~$ ./bin/8085
```

## Build with debug symbols
```Shell
~$ cmake -DCMAKE_BUILD_TYPE=Debug -B build/debug -S .
~$ cmake --build build/debug

# check binary has debug info
~$ file ./bin/8085
# output: "with debug_info, not stripped"
```
