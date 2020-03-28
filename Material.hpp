//
//  Material.hpp
//  OpenGLTest
//
//  Created by Jacob Xu on 2020/2/11.
//  Copyright © 2020 Jacob Xu. All rights reserved.
//

#ifndef Material_hpp
#define Material_hpp
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stdio.h>
#include "Shader.hpp"
class Material{
    public:
    Material(Shader * _shader, unsigned int _diffuse, unsigned int _specular, glm::vec3 _ambient, float shininess);
    Shader* shader;
    unsigned int diffuse;
    unsigned int specular;
    glm::vec3 ambient;
    float shininess;
}; 
#endif /* Material_hpp */
