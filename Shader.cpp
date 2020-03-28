//
//  Shader.cpp
//  OpenGLTest
//
//  Created by Jacob Xu on 2020/1/30.
//  Copyright © 2020 Jacob Xu. All rights reserved.
//

#include "Shader.hpp"
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <sstream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

Shader::Shader(const char* vertexPath, const char* fragmentPath, const char* geomPath){
    std::ifstream vertexFile;
    std::ifstream fragmentFile;
    std::ifstream geometryFile;
    std::stringstream vertexSStream;
    std::stringstream fragmentSStream;
    std::stringstream geometrySStream;
    vertexFile.open(vertexPath);
    fragmentFile.open(fragmentPath);
    vertexFile.exceptions(std::ifstream::failbit||std::ifstream::badbit);
    fragmentFile.exceptions(std::ifstream::failbit||std::ifstream::badbit);
    
    try{
        if (!vertexFile.is_open() || !fragmentFile.is_open()){
            
            throw std::logic_error("Open file error!");
        }
        vertexSStream << vertexFile.rdbuf();
        fragmentSStream << fragmentFile.rdbuf();
        
        vertexString = vertexSStream.str();
        fragmentString = fragmentSStream.str();
        
        vertexSource = vertexString.c_str();
        fragmentSource = fragmentString.c_str();
        // std::cout << vertexSource << std::endl;
        
        unsigned int vertex, fragment;
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vertexSource, NULL);
        glCompileShader(vertex);
        ShadercheckCompileErrors(vertex, "VERTEX");
        unsigned int geometry;
        if (geomPath != nullptr){
            geometryFile.open(geomPath);
            geometrySStream << geometryFile.rdbuf();
            geometryString = geometrySStream.str();
            geometrySource = geometryString.c_str();
            geometry = glCreateShader(GL_GEOMETRY_SHADER);
            glShaderSource(geometry, 1, &geometrySource, NULL);
            glCompileShader(geometry);
            ShadercheckCompileErrors(geometry, "GEOMETRY");
        }
        
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fragmentSource, NULL);
        glCompileShader(fragment);
        ShadercheckCompileErrors(fragment, "FRAGMENT");
        
        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        if (geomPath != nullptr){
            glAttachShader(ID, geometry);
        }
        glLinkProgram(ID);
        // checkCompileErrors(ID, "PROGRAM");
        glDeleteShader(vertex);
        glDeleteShader(fragment);
        
        
    }
    catch (const std::exception&cond){
        std::cout<<cond.what()<<std::endl;
    }
}

void Shader::use(){
    glUseProgram(ID);
}

void Shader::ShadercheckCompileErrors(unsigned int ID, std::string shader_type){
    int success;
    char infoLog[512];
    if (shader_type != "PROGRAM"){
        glGetShaderiv(ID, GL_COMPILE_STATUS, &success);
    
        if (!success){
            glGetShaderInfoLog(ID, 512, NULL, infoLog);
            std::cout << "Shader compile error: " << infoLog << std::endl;
        }
    }
    else{
        glGetProgramiv(ID, GL_LINK_STATUS, &success);
        if(!success){
            glGetProgramInfoLog(ID, 512, NULL, infoLog);
            std::cout << "Program compile error: " << infoLog << std::endl;
        }
    }
    
}

void Shader::SetUniform3f(const char* paraName, glm::vec3 param){
    glUniform3f(glGetUniformLocation(ID, paraName), param.x, param.y, param.z);

}

void Shader::SetUniform1i(const char *paraName, unsigned int param){
    glUniform1i(glGetUniformLocation(ID, paraName), param);
}

void Shader::SetUniform4m(const char* paraName, glm::mat4 mat){
    glad_glUniformMatrix4fv(glGetUniformLocation(ID, paraName), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setFloat(const char *paraName, float value){
    glUniform1f(glGetUniformLocation(ID, paraName), value);
}
