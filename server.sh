#!/bin/bash

cd server
if [ -d ssl ]; then 
    echo "SSL dir exists"
else 
    mkdir ssl
    cd ssl
    openssl req -x509 -newkey rsa:2048 \
    -keyout key.pem \
    -out cert.pem \
    -days 365 \
    -nodes
    echo "SSL dir created Succesefully"
    cd ..
fi

make clear
make create
cd bin
./main.exe