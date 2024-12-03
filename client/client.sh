#!/bin/bash

clear

echo "Compiling client.c..."

gcc client.c -o client

if [ $? -eq 0 ]; then
    echo "Compilation successful. Running the program..."
    ./client
else
    echo "Compilation failed."
fi