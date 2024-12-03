#!/bin/bash

gcc test.c -o test_program

if [ $? -eq 0 ]; then
    echo "Compilation successful. Running the program..."
    ./test_program
else
    echo "Compilation failed."
fi