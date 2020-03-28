//
//  LightPoint.hpp
//  OpenGLTest
//
//  Created by Jacob Xu on 2020/2/13.
//  Copyright © 2020 Jacob Xu. All rights reserved.
//

#ifndef LightPoint_hpp
#define LightPoint_hpp

#include <stdio.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>

class LightPoint{
public:
    glm::vec3 position;
    glm::vec3 colors;
    glm::vec3 angles; //光源的x旋转角度
    glm::vec3 direction; //光源指向向量
    LightPoint(glm::vec3 _position, glm::vec3 _angles, glm::vec3 _colors);
    float constantParam = 1.0f;
    float linear = 0.09f;
    float quadratic = 0.032f;
};
#endif /* LightPoint_hpp */
