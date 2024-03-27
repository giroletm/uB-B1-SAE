@echo off

echo.
echo Compilation du client...
g++ src/client.cpp src/common.cpp src/console.cpp -o build/Client.exe -static-libgcc -static-libstdc++ -lws2_32 -Wall

echo.
echo Compilation du serveur...
g++ src/serveur.cpp src/common.cpp src/console.cpp -o build/Serveur.exe -static-libgcc -static-libstdc++ -lws2_32 -Wall
