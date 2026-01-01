add_executable(MyKooder
    main.cpp
    instrumentor/config_parser/toml_parser.cpp
)

target_link_libraries(MyKooder PRIVATE spdlog::spdlog)

target_include_directories(MyKooder 
    PRIVATE "${PROJECT_SOURCE_DIR}/include"
)
