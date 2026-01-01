#include "instrumentor/config_parser/toml_parser.hpp"
#include "spdlog/spdlog.h"


int main(int argc, char* argv[]) 
{
    if (argc < 2) {
        return 1; // Need a filename!
    }

    Toml::TomlParser parser(argv[1]);

    auto& fields = parser.getFields();
    spdlog::info("Target Name: {}", fields.targetVar.name.c_str());

    return 0; 
}