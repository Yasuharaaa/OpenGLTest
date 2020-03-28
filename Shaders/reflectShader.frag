#version 330 core

in vec3 Normal;
in vec3 Position;

out vec4 FragColor;

uniform vec3 cameraPos;
uniform samplerCube skybox;

void main(){
    /*
    -----reflect-----
    vec3 I = normalize(Position - cameraPos);
    vec3 R = reflect(I, normalize(Normal));
    FragColor = vec4(texture(skybox, R).rgb, 1.0);
     */
    // refract
    float ratio = 1.00 / 1.52;
    vec3 I = normalize(Position - cameraPos);
    vec3 R = refract(I, normalize(Normal), ratio);
    FragColor = vec4(texture(skybox, R).rgb, 1.0);
    //FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
}
