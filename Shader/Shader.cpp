#include "Shader.hpp"

uint _compileShader(uint type, const std::string input)
{
    if (type != GL_VERTEX_SHADER && type != GL_FRAGMENT_SHADER)
    {
        std::cerr << "ERROR::SHADER::INVALID_SHADER_TYPE\n";
        return -1;
    }

    // create and compile shader
    const char *shaderSource = input.c_str();
    uint shader;
    shader = glCreateShader(type);
    glShaderSource(shader, 1, &shaderSource, 0);
    glCompileShader(shader);

    // check for compilation errors
    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::" << (type == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT") << "::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    }
    return shader;
}

std::string _readFile(const std::string file_location)
{
    std::fstream file;
    std::stringstream file_stream;
    std::string content;
    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        file.open(file_location, std::ios::in);
        file_stream << file.rdbuf();
        content = file_stream.str();
    }
    catch (std::ifstream::failure e)
    {
        std::cerr << "ERROR::SHADER:FILE_ERROR: " << file_location << std::endl;
        std::cerr << e.what() << std::endl;
    }
    file.close();
    return content;
}

Shader::Shader(const std::string vertexPath, const std::string fragmentPath)
{
    uint vertexShader = _compileShader(GL_VERTEX_SHADER, _readFile(vertexPath));
    uint fragmentShader = _compileShader(GL_FRAGMENT_SHADER, _readFile(fragmentPath));

    ID = glCreateProgram();
    glAttachShader(ID, vertexShader);
    glAttachShader(ID, fragmentShader);
    glLinkProgram(ID);

    int success;
    char infoLog[512];
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

Shader::~Shader(){
    glDeleteProgram(ID);
}

void Shader::use(){
    glUseProgram(ID);
}

void Shader::setBool(const std::string& name, bool value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), int(value));
}

void Shader::setInt(const std::string& name, int value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) const {
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setVec4f(const std::string& name, const float value1, const float value2, const float value3, const float value4) const {
    glUniform4f(glGetUniformLocation(ID, name.c_str()), value1, value2, value3, value4);
}
