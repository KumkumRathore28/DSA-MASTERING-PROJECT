cmake_minimum_required(VERSION 3.10)
project(SocialNetworkGraphSystem)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(server
    main.cpp
    Graph.cpp
    User.cpp
    HTTPParser.cpp
    JSONParser.cpp
)

target_include_directories(server PRIVATE .)

if(WIN32)
    target_link_libraries(server ws2_32)
endif()

target_compile_options(server PRIVATE -pthread)
target_link_libraries(server PRIVATE pthread)
