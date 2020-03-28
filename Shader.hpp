//
//  Shader.hpp
//  OpenGLTest
//
//  Created by Jacob Xu on 2020/1/30.
//  Copyright © 2020 Jacob Xu. All rights reserved.
//

#ifndef Shader_hpp
#define Shader_hpp
#include <string>
#include <stdio.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader{
public:
    // Shader();
    Shader(const char* vertexPath, const char* fragPath, const char* gemoPath= nullptr);
    ~Shader();
    std::string vertexString;
    std::string fragmentString;
    std::string geometryString;
    const char* vertexSource;
    const char* fragmentSource;
    const char* geometrySource;
    unsigned int ID;
    void use();
    void SetUniform3f(const char* paraName, glm::vec3 param);
    void SetUniform1i(const char* paraName, unsigned int param);
    void SetUniform4m(const char* paraName, glm::mat4 mat);
    void setFloat(const char* paraName, float value);
    enum Slot{
        DIFFUSE = 0,
        SPECULAR = 1
    };
private:
    void ShadercheckCompileErrors(unsigned int ID, std::string shader_type);
    
};
#endif /* Shader_hpp */
