#!/bin/bash

gcc client.c -o test_program_client

if [ $? -eq 0 ]; then
    echo "Compilation successful. Running the program..."
    ./test_program_client
else
    echo "Compilation failed."
fi