//
//  LightDirectional.cpp
//  OpenGLTest
//
//  Created by Jacob Xu on 2020/2/13.
//  Copyright © 2020 Jacob Xu. All rights reserved.
//

#include "LightDirectional.hpp"

LightDirectional::LightDirectional(glm::vec3 _position, glm::vec3 _angles, glm::vec3 _colors):
    position(_position),
    angles(_angles),
    colors(_colors){
        UpdateDirection();
}
