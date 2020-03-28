//
//  Model.hpp
//  OpenGLTest
//
//  Created by Jacob Xu on 2020/2/18.
//  Copyright © 2020 Jacob Xu. All rights reserved.
//

#ifndef Model_hpp
#define Model_hpp
#include "Mesh.hpp"
#include <vector>
#include <string>
#include <stdio.h>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include <iostream>
#include "Shader.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Model{
public:
    Model(std::string path);
    std::vector<Mesh> meshes;
    std::string directory;
    void Draw(Shader* shader);
private:
    unsigned int TextureFromFile(const char *path, const std::string &directory, bool gamma = false);
    std::vector<Texture> textures_loaded;
    void loadModel(std::string path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial*, aiTextureType, std::string);
};
#endif /* Model_hpp */
