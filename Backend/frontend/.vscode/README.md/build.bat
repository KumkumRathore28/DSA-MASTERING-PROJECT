@echo off
echo Building Social Network Graph System...
cd backend
g++ -std=c++17 -Wall -Wextra -O2 -pthread main.cpp Graph.cpp User.cpp HTTPParser.cpp JSONParser.cpp -o server.exe -lws2_32
if %ERRORLEVEL% EQU 0 (
    echo Build successful!
    echo Server executable: backend\server.exe
) else (
    echo Build failed!
    pause
)
cd ..
