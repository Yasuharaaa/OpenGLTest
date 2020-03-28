//
//  LightDirectional.hpp
//  OpenGLTest
//
//  Created by Jacob Xu on 2020/2/13.
//  Copyright © 2020 Jacob Xu. All rights reserved.
//

#ifndef LightDirectional_hpp
#define LightDirectional_hpp

#include <stdio.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>

class LightDirectional{
public:
    glm::vec3 position;
    glm::vec3 colors;
    glm::vec3 angles; //光源的x旋转角度
    glm::vec3 direction; //光源指向向量
    LightDirectional(glm::vec3 _position, glm::vec3 _angles, glm::vec3 _color=glm::vec3(1.0f, 1.0f, 1.0f));
    void UpdateDirection(){
        direction = glm::vec3(0, 0, 1.0f); // move to position forward
        direction = glm::rotateZ(direction, angles.z);
        direction = glm::rotateY(direction, angles.y);
        direction = glm::rotateX(direction, angles.x);
        direction = -1.0f * direction;
    }
};

#endif /* LightDirectional_hpp */
