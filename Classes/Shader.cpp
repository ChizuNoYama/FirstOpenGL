#include "Shader.hpp"
#include "../Utilities.hpp"

Shader::Shader()
{
      // programID is not initialized here
}

void Shader::initializeProgram(const char *vertexPath, const char *fragmentPath)
{
      // 1. read the file contents of the shader and copy them to C-style strings
      std::string vertexCode;
      std::string fragmentCode;
      std::ifstream vShaderFile;
      std::ifstream fShaderFile;

      vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
      fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
      try
      {
            // open files
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);
            
            //read file's buffer contents into streams
            std::stringstream vShaderStream;
            std::stringstream fShaderStream;
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();

            //close the files
            vShaderFile.close();
            fShaderFile.close();

            // copy contents from the stream to strings
            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();
      }
      catch(std::ifstream::failure&)
      {
            vShaderFile.close();
            fShaderFile.close();
            throw std::exception {"ERROR::SHADER::FILE_NOT_SUCCESSFULLY::READ"};
      }

      // Convert string to a C-style string, which is a char* pointer. given that it is an array of char and array is a pointer
      const char* vShaderCode = vertexCode.c_str();
      const char* fShaderCode = fragmentCode.c_str();

      // 2. compile and link the shader code
      unsigned int vertexShaderID { glCreateShader(GL_VERTEX_SHADER)};
      glShaderSource(vertexShaderID, 1, &vShaderCode, nullptr);
      glCompileShader(vertexShaderID);

      if(!Utility::validateShaderCompilation(vertexShaderID, &Utility::VERTEX_SHADER_TYPE))
      {
            glDeleteShader(vertexShaderID);
            
            throw std::exception{};
      }

      unsigned int fragmentShaderID { glCreateShader(GL_FRAGMENT_SHADER) };
      glShaderSource(fragmentShaderID, 1, &fShaderCode, nullptr);
      glCompileShader(fragmentShaderID);
      
      if(!Utility::validateShaderCompilation(fragmentShaderID, &Utility::FRAGMENT_SHADER_TYPE))
      {
            glDeleteShader(fragmentShaderID);
            
            throw std::exception{};
      }

      // Shader Program
      this->program_id = glCreateProgram();
      glAttachShader(this->program_id, vertexShaderID);
      glAttachShader(this->program_id, fragmentShaderID);
      glLinkProgram(this->program_id);

      glDeleteShader(vertexShaderID);
      glDeleteShader(fragmentShaderID);

      if(!Utility::validateProgramLinking(this->program_id))
      {
            throw std::exception{};
      }
}

void Shader::use() const
{
      glUseProgram(this->program_id);
}

void Shader::setBool(const std::string &name, bool value) const
{
      glUniform1i(glGetUniformLocation(this->program_id, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, float value) const
{
      glUniform1f(glGetUniformLocation(this->program_id, name.c_str()), value);
}

void Shader::setInt(const std::string &name, int value) const
{
      glUniform1i(glGetUniformLocation(this->program_id, name.c_str()), value);
}


