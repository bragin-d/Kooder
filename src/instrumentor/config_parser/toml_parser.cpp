#include "instrumentor/config_parser/toml_parser.hpp"

namespace Toml 
{
    // Constructor
    TomlParser::TomlParser(const std::string& filePath)
    {
        parse(filePath);
        spdlog::info("Config loaded from {}", filePath);
    }

    // Destructor
    TomlParser::~TomlParser()
    {
        spdlog::info("Destructor called");
    } 

    void TomlParser::parse(const std::string& filePath)
    {
        try
        {
            // toml::parse_file returns a table directly
            auto tbl = toml::parse_file(filePath);
            
            _fields.targetVar.name = tbl["target_var"]["name"].value_or("default");
            
            std::stringstream ss;
            ss << tbl; 
            spdlog::info("Parsed TOML:\n{}", ss.str());
        }
        catch (const toml::parse_error& err)
        {
            spdlog::error("Error parsing the file: {}", err.description());
            return;
        }
    }
}