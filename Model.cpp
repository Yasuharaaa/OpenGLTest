//
//  Model.cpp
//  OpenGLTest
//
//  Created by Jacob Xu on 2020/2/18.
//  Copyright © 2020 Jacob Xu. All rights reserved.
//

#include "Model.hpp"
#include <iostream>
#include "stb_image.h"
Model::Model(std::string path){
    loadModel(path);
}

void Model::Draw(Shader* shader){
    for(unsigned int i = 0; i < meshes.size(); i++){
        meshes[i].Draw(shader);
    }
}
void Model::loadModel(std::string path){
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode){
        std::cout << "Scene loading error!" <<std:: endl;
        return;
    }
    directory = path;
    // std::cout << path << std::endl;
    processNode(scene->mRootNode, scene);
}

Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene){
    std::vector<Vertex> tempVertices;
    std::vector<unsigned int> tempIndices;
    std::vector<Texture> tempTextures;
    
    // glm::vec3 tempVec;
    for(unsigned int i = 0; i < mesh->mNumVertices; i++){
        
        Vertex tempVertex;
        // 顶点
        tempVertex.Position.x = mesh->mVertices[i].x;
        tempVertex.Position.y = mesh->mVertices[i].y;
        tempVertex.Position.z = mesh->mVertices[i].z;
        // 法向量
        tempVertex.Normal.x = mesh->mNormals[i].x;
        tempVertex.Normal.y = mesh->mNormals[i].y;
        tempVertex.Normal.z = mesh->mNormals[i].z;
        
        if (mesh->mTextureCoords[0]){
            tempVertex.TexCoords.x = mesh->mTextureCoords[0][i].x;
            tempVertex.TexCoords.y = mesh->mTextureCoords[0][i].y;
        }
        else{
            tempVertex.TexCoords = glm::vec2(0.0f, 0.0f);
        }
        tempVertices.push_back(tempVertex);
    }
    // 序号
    for(unsigned int i = 0; i < mesh->mNumFaces; i++){
        for(unsigned int j = 0; j < mesh->mFaces[i].mNumIndices; j++){
            tempIndices.push_back(mesh->mFaces[i].mIndices[j]);
            // std::cout << "1" << std::endl;
        }
    }
    if(mesh->mMaterialIndex >= 0){
        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
        std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        tempTextures.insert(tempTextures.end(), diffuseMaps.begin(), diffuseMaps.end());
        
        std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        tempTextures.insert(tempTextures.end(), specularMaps.begin(), specularMaps.end());
    }
    return Mesh(tempVertices, tempIndices, tempTextures);
}

void Model::processNode(aiNode *node, const aiScene *scene){
    // std::cout << node->mName.data << std::endl;
    for (unsigned int i = 0; i < node->mNumMeshes; i++){
        aiMesh* curMesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(ProcessMesh(curMesh, scene));
    }
    for (unsigned int i = 0; i < node->mNumChildren; i++){
        processNode(node->mChildren[i], scene);
    }
    
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial * mat, aiTextureType type, std::string typeName){
    std::vector<Texture> textures;
    for(unsigned int i = 0; i < mat->GetTextureCount(type); i++){
        aiString str;
        mat->GetTexture(type, i, &str);
        bool skip = false;
        for(unsigned int j = 0; j < textures_loaded.size(); j++)
        {
            if(std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
            {
                textures.push_back(textures_loaded[j]);
                skip = true;
                break;
            }
        }
        if(!skip)
        {   // 如果纹理还没有被加载，则加载它
            Texture texture;
            texture.id = TextureFromFile(str.C_Str(), directory);
            texture.type = typeName;
            texture.path = str.C_Str();
            textures.push_back(texture);
            textures_loaded.push_back(texture); // 添加到已加载的纹理中
            
        }
    }
    return textures;
}

unsigned int Model::TextureFromFile(const char *path, const std::string &directory, bool gamma)
{
    std::string filename = std::string(path);
    std::string filePath = directory.substr(0, directory.find_last_of('/'));
    filename = filePath + '/' + filename;
    // std::cout << directory << std::endl;
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}
