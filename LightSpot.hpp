//
//  LightSpot.hpp
//  OpenGLTest
//
//  Created by Jacob Xu on 2020/2/14.
//  Copyright © 2020 Jacob Xu. All rights reserved.
//

#ifndef LightSpot_hpp
#define LightSpot_hpp

#include <stdio.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
 
class LightSpot{
public:
    glm::vec3 position;
    glm::vec3 colors;
    glm::vec3 angles; //光源的x旋转角度
    glm::vec3 direction; //光源指向向量
    float cosInnerPhy = 0.99f;
    float cosOuterPhy = 0.85f;
    float constantParam = 1.0f;
    float linear = 0.09f;
    float quadratic = 0.032f;
    LightSpot(glm::vec3 _position, glm::vec3 _angles, glm::vec3 _color=glm::vec3(1.0f, 1.0f, 1.0f));
    void UpdateDirection();
    
};
#endif /* LightSpot_hpp */
