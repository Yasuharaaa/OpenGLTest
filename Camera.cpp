//
//  Camera.cpp
//  OpenGLTest
//
//  Created by Jacob Xu on 2020/2/4.
//  Copyright © 2020 Jacob Xu. All rights reserved.
//

#include "Camera.hpp"
#include <iostream>
Camera::Camera(glm::vec3 position, glm::vec3 target, glm::vec3 world_up){
    Position = position;
    World_up = world_up;
    
    Forward = glm::normalize(target - position);
    Right = glm::normalize(glm::cross(Forward, World_up));
    Up = glm::normalize(glm::cross(Forward, Right));
    UpdateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix(){
    //std::cout << Forward.x << " " << Forward.y << " " << Forward.z << std::endl;
    return glm::lookAt(Position, Position+Forward, World_up);
}

Camera::Camera(glm::vec3 position, float pitch, float yaw, glm::vec3 world_up){
    Position = position;
    World_up = world_up;
    Pitch = pitch;
    Yaw = yaw;
    Forward.x = glm::cos(Pitch) * glm::sin(Yaw);
    Forward.y = glm::sin(Pitch);
    Forward.z = glm::cos(Pitch) * glm::cos(Yaw);
    UpdateCameraVectors();
}

void Camera::UpdateCameraVectors(){
    Forward.x = glm::cos(Pitch) * glm::sin(Yaw);
    Forward.y = glm::sin(Pitch);
    Forward.z = glm::cos(Pitch) * glm::cos(Yaw);
    Forward = glm::normalize(Forward);
    Right = glm::normalize(glm::cross(Forward, World_up));
    Up = glm::normalize(glm::cross(Right, Forward));
}

void Camera::ProcessMouseMovement(float delta_x, float delta_y){
    Pitch += delta_y * sense_y;
    Yaw += delta_x * sense_x;
    UpdateCameraVectors();
    // std::cout << Forward.x << std::endl;
}

void Camera::MoveY(float delta_y){
    speed_y = delta_y;
    if (delta_y>0)
        UpDateCameraPosition("Up+");
    else if(delta_y<0)
        UpDateCameraPosition("Up-");
}

void Camera::MoveX(float delta_x){
    speed_x = delta_x;
    if (delta_x>0)
        UpDateCameraPosition("Right+");
    else if(delta_x<0)
        UpDateCameraPosition("Right-");
}

void Camera::MoveZ(float delta_z){
    speed_z = delta_z;
    if (delta_z>0)
        UpDateCameraPosition("Forward+");
    else if(delta_z<0)
        UpDateCameraPosition("Forward-");
}

void Camera::UpDateCameraPosition(std::string p){
    if(p == "Up+"){
        Position += Up * 0.1f;
    }
    else if (p == "Up-"){
        Position -= Up * 0.1f;
    }
    if(p == "Right+"){
        Position += Right * 0.1f;
    }
    else if (p == "Right-"){
        Position -= Right * 0.1f;
    }if(p == "Forward+"){
        Position += Forward * 0.1f;
    }
    else if (p == "Forward-"){
        Position -= Forward * 0.1f;
    }
    //std::cout << Position.x << " " << Position.y << " " << Position.z << std::endl;
    UpdateCameraVectors();
}


