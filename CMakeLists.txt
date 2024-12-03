cmake_minimum_required(VERSION 3.2)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(DSAProj3)

set(SFML_DIR "C:/Users/colli/CLionProjects/SFML/lib/cmake/SFML")
find_package(SFML 2.5.1 COMPONENTS system window graphics audio REQUIRED)

add_executable(DSAProj3 main.cpp
        StatsDatabase.h
        StatsDatabase.cpp
        Window.h
        Window.cpp
        SearchMenu.h
        SearchMenu.cpp
        MainMenu.h
        MainMenu.cpp)

target_link_libraries(DSAProj3 sfml-system sfml-window sfml-graphics sfml-audio)