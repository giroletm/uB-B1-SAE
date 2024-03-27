#!/bin/bash

echo
echo Compilation du client...
g++ src/client.cpp src/common.cpp src/console.cpp -o build/Client -pthread -Wall

echo
echo Compilation du serveur...
g++ src/serveur.cpp src/common.cpp src/console.cpp -o build/Serveur -pthread -Wall
