#version 330 core
layout (location = 0) in vec3 aPos;
// layout (location = 7) in vec3 aColor;
// layout (location = 8) in vec2 aTexCoord;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
//out vec4 vertexColor;
//out vec2 TexCoord;

// uniform mat4 transform;
uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;


out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoord;
void main()
{
    gl_Position = proj * view * model * vec4(aPos.xyz, 1.0);
    Normal = mat3(transpose(inverse(model))) * aNormal;
    FragPos = vec3(model * vec4(aPos, 1.0));
    // vertexColor = vec4(aColor, 1.0);
    // TexCoord = aTexCoord;
    TexCoord = aTexCoord;
}
