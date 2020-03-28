//
//  Mesh.cpp
//  OpenGLTest
//
//  Created by Jacob Xu on 2020/2/17.
//  Copyright © 2020 Jacob Xu. All rights reserved.
//
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Mesh.hpp"
#include <iostream>
#include <string>
Mesh::Mesh(float vertices[]){
    this->vertices.resize(36);
    memcpy(&(this->vertices[0]), vertices, 36*8*sizeof(float));
    
    setupMesh();
}

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures){
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;
    setupMesh();
}

void Mesh::Draw(Shader* shader){
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    for(unsigned int i = 0; i <textures.size(); i++){
        glActiveTexture(GL_TEXTURE0+i); //激活对应材质栏位
        std::string number;
        std::string name = textures[i].type;
        if(name == "texture_diffuse"){
            number = std::to_string(diffuseNr++);
//            glActiveTexture(GL_TEXTURE0);
//            glBindTexture(GL_TEXTURE_2D, textures[0].id);
//            shader->SetUniform1i("texture_diffuse1", 0);
        }
        else if(textures[i].type == "texture_specular"){
            number = std::to_string(specularNr++);
//            glActiveTexture(GL_TEXTURE1);
//            glBindTexture(GL_TEXTURE_2D, textures[0].id);
//            shader->SetUniform1i("material.specular", 1);
        }
        shader->SetUniform1i((name + number).c_str(), i);
        //std::cout<<(name + number)<<std::endl;
        glBindTexture(GL_TEXTURE_2D, textures[i].id);
    }
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);
    // glDrawArrays(GL_TRIANGLES, 0, 36);
    //std::cout << indices.size() <<std::endl;
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    
    
}

void Mesh::setupMesh(){
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
    
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), &indices[0], GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(6*sizeof(float)));
    
    glBindVertexArray(0);
}
