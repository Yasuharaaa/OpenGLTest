//
//  LightSpot.cpp
//  OpenGLTest
//
//  Created by Jacob Xu on 2020/2/14.
//  Copyright © 2020 Jacob Xu. All rights reserved.
//

#include "LightSpot.hpp"

LightSpot::LightSpot(glm::vec3 _position, glm::vec3 _angles, glm::vec3 _colors):
    position(_position),
    angles(_angles),
    colors(_colors){
    UpdateDirection();
}

void LightSpot::UpdateDirection(){
    direction = glm::vec3(0, 0, 1.0f); // move to position forward
    direction = glm::rotateZ(direction, angles.z);
    direction = glm::rotateY(direction, angles.y);
    direction = glm::rotateX(direction, angles.x);
    direction = -1.0f * direction;
    
}
