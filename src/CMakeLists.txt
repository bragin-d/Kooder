add_executable(MyKooder
    main.cpp
    instrumentor/config_parser/toml_parser.cpp
)

target_include_directories(MyKooder 
    PRIVATE "${PROJECT_SOURCE_DIR}/include"
)