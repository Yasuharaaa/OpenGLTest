//
//  Camera.hpp
//  OpenGLTest
//
//  Created by Jacob Xu on 2020/2/4.
//  Copyright © 2020 Jacob Xu. All rights reserved.
//

#ifndef Camera_hpp
#define Camera_hpp
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <stdio.h>

class Camera{
public:
    Camera(glm::vec3 position, glm::vec3 target, glm::vec3 world_up);
    Camera(glm::vec3 position, float pitch, float yaw, glm::vec3 world_up); 
    glm::vec3 Position;
    glm::vec3 Forward;
    glm::vec3 Right;
    glm::vec3 Up;
    glm::vec3 World_up;
    float sense_x = 0.005f;
    float sense_y = 0.005f;
    float Pitch;
    float Yaw;
    float speed_x = 0.0f;
    float speed_y = 0.0f;
    float speed_z = 0.0f;
    glm::mat4 GetViewMatrix();
    void ProcessMouseMovement(float delta_x, float delta_y);
    void MoveY(float delta_y);
    void MoveX(float delta_x);
    void MoveZ(float delta_z);
    void UpDateCameraPosition(std::string p);
    
private:
    void UpdateCameraVectors();
    
};

#endif /* Camera_hpp */
