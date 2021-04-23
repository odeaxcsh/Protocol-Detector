# Protocol-Detector
A semi-regex based packet detector written in C++

## Compile and Run
After installing `cmake`, `make` and `GTest`(optional for compile tests) use following instruction
- create a build folder
- generate appropriate `makefile` using `cmake <path to source folder>`
  - add `-Dcompile_project_tests` option to compile tests too.
- compile code using `make`
- executables are placed in `bin`

## Components diagram
![Screenshot](project-data/project-diagram.png)
