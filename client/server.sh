#!/bin/bash

lsof -i :8080
cd ../server
make
cd bin
./main.exe