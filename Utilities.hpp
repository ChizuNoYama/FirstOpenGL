#pragma once
#include <string>

#include <iostream>
#include <glad/glad.h>

namespace Utility{

    const std::string FRAGMENT_SHADER_TYPE = "FRAGMENT";
    const std::string VERTEX_SHADER_TYPE = "VERTEX";
    
    static bool validateShaderCompilation(unsigned int shader, const std::string* shaderType)
    {
        // Check if compilation was complete
        int success;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

        if(!success)
        {
            char infoLog[512];
            glGetShaderInfoLog(shader, 512, nullptr, infoLog);
            std::cout << "ERROR::SHADER::" << shaderType << "::COMPILATION_FAILED\n" << infoLog << "\n";

            return false;
        }

        return true;
    }

    static bool validateProgramLinking(unsigned int program)
    {
        int success;
        glGetProgramiv(program, GL_COMPILE_STATUS, &success);

        if(!success)
        {
            char infoLog[512];
            glGetProgramInfoLog(program, 512, nullptr, infoLog);
            std::cout << "ERROR::PROGRAM::COMPILATION_FAILED\n" << infoLog << "\n";

            return false;
        }
        return true;
    }
}