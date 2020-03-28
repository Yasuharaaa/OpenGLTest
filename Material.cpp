//
//  Material.cpp
//  OpenGLTest
//
//  Created by Jacob Xu on 2020/2/11.
//  Copyright © 2020 Jacob Xu. All rights reserved.
//

#include "Material.hpp"

Material::Material(Shader * _shader, unsigned int _diffuse, unsigned int _specular, glm::vec3 _ambient, float _shininess){
    shader = _shader;
    diffuse = _diffuse;
    specular = _specular;
    ambient = _ambient;
    shininess = _shininess;
}




