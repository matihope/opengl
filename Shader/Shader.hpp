#ifndef SHADER_HPP
#define SHADER_HPP

#include "../glad/glad.h"
#include <fstream>
#include <iostream>
#include <sstream>

class Shader {
    public:
        // shader program id
        uint ID;

        Shader(const std::string vertexPath, const std::string fragmentPath);
        ~Shader();
        
        void use();
        void setBool(const std::string& name, bool value) const;
        void setInt(const std::string& name, int value) const;
        void setFloat(const std::string& name, float value) const;
        void setVec4f(const std::string& name, const float value1, const float value2, const float value3, const float value4) const;
};

#endif