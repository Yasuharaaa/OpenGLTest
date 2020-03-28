#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aTexCoords;

out VS_OUT {
    vec2 texCoords;
} vs_out;
out vec2 TexCoords;
uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;

void main(){
    vs_out.texCoords = aTexCoords;
    TexCoords = aTexCoords;
    gl_Position = proj * view * model * vec4(aPos, 1.0);
}
