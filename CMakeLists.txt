cmake_minimum_required(VERSION 3.15)


project(Kooder
    VERSION 0.1
    DESCRIPTION "Visualisation tool"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(src)