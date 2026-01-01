cmake_minimum_required(VERSION 3.15)


project(Kooder
    VERSION 0.1
    DESCRIPTION "Visualisation tool"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


include(FetchContent)

FetchContent_Declare(
  spdlog
  GIT_REPOSITORY https://github.com/gabime/spdlog.git
  GIT_TAG        v1.12.0 # Check for the latest version
)

FetchContent_MakeAvailable(spdlog)


add_subdirectory(src)