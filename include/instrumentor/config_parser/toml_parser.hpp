/**
 * @file toml_parser.hpp
 * @brief Configuration Schema for KooderConfig.toml
 * * This module parses a TOML configuration file. Below is the supported
 * structure and the allowed values for each field.
 * * ============================================================================
 * TOML SCHEMA SPECIFICATION
 * ============================================================================
 * * [target_var]
 * name             = "string"  
 * var_type         = "string"  # Options: "int", "float", "double", "char", ""
 * 
 * * [scope]
 * mode             = "string"  # Options: "semi-auto", ""
 * 
 * * [visual]
 * sim_visual       = "string"  # Options: "1D-grid", "2D-grid", "3D-grid" 
 * 
 * * [compilation]
 * compiler         = "string"  # Options: "gcc", "clangd"
 * exe_output_path  = "string"  # Valid system path
 * 
 * * ============================================================================
 * 
 * FIELD DETAILS:
 * * 
 * ============================================================================
 */


#pragma once

#include <cstdio> 
#include <string>
#include <sstream>
#include "spdlog/spdlog.h"
#include "instrumentor/config_parser/toml.hpp"

namespace Toml
{
    class TomlParser
    {
        public:

            struct TargetVarField
            {
                enum class Type {INT, FLOAT, CHAR};
                Type type = Type::INT;
                std::string name;
            };

            struct ScopeField
            {
                enum class Mode { SemiAuto };
                Mode mode = Mode::SemiAuto;
            };

            struct VisualField
            {
                enum class VisualType { OneDGrid, TwoDGrid, ThreeDGrid };
                VisualType simVisual = VisualType::TwoDGrid;
            };

            struct CompilationField 
            {
                enum class Compiler { Gcc, Clangd };
                Compiler compiler = Compiler::Gcc;
                std::string exeOutputPath;
            };

            struct Fields
            {
                TargetVarField      targetVar;
                ScopeField          scope;
                VisualField         visual;
                CompilationField    compilation;
            };

            TomlParser(const std::string& filePath);
            ~TomlParser();

            const Fields& getFields() const { return _fields; }
            
        private:
            Fields  _fields;

            void parse(const std::string& filePath);            
    };
}

