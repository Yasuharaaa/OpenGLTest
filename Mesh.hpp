//
//  Mesh.hpp
//  OpenGLTest
//
//  Created by Jacob Xu on 2020/2/17.
//  Copyright © 2020 Jacob Xu. All rights reserved.
//

#ifndef Mesh_hpp
#define Mesh_hpp
#include <glm/glm.hpp>
#include <stdio.h>
#include <string>
#include <vector>
#include "Shader.hpp"

struct Vertex{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

struct Texture{
    unsigned int id;
    std::string type;
    std::string path;
};

class Mesh{
public:
    Mesh(float vertices[]);
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;
    void Draw(Shader* shader);
private:
    unsigned int VAO, VBO, EBO;
    void setupMesh();
    
};
#endif /* Mesh_hpp */
