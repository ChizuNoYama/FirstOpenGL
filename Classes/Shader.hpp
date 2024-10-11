#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glad/glad.h>

class Shader
{
    public:
        // constructor reads and builds the shader 
        Shader();
        //InitializeProgram
        // ProgramID
        unsigned int program_id;
        // Initialize the program
        void initializeProgram(const char* vertexPath, const char* fragmentPath);
        // use/activate the shader
        void use() const;
        //utility uniform functions
        void setBool(const std::string &name, bool value) const;
        void setInt(const std::string &name, int value) const;
        void setFloat(const std::string &name, float value) const;
};
