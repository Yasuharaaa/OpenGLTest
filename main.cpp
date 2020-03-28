#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include <iostream>
#include "Shader.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <thread>
#include <chrono>
#include "Camera.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "Material.hpp"
#include "LightDirectional.hpp"
#include "LightPoint.hpp"
#include "LightSpot.hpp"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Mesh.hpp"
#include "Model.hpp"
#include <vector>
#include <string>
#include "e_Shader.h"

Camera camera(glm::vec3(0, 0, 3.0f), glm::radians(15.0f), glm::radians(180.0f), glm::vec3(0, 1.0f, 0));
LightDirectional lightD(glm::vec3(1.0f, 100.0f, -100.0f), glm::vec3(glm::radians(45.0f), 0, 0), glm::vec3(1.0f, 1.0f, 1.0f));

LightPoint lightP0(glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(glm::radians(45.0f), glm::radians(45.0f), 0), glm::vec3(1.0f, 0.0f, 0.0f));
LightPoint lightP1(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(glm::radians(45.0f), glm::radians(45.0f), 0), glm::vec3(0.0f, 1.0f, 0.0f));
LightPoint lightP2(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(glm::radians(45.0f), glm::radians(45.0f), 0), glm::vec3(0.0f, 0.0f, 1.0f));
LightPoint lightP3(glm::vec3(1.0f, 1.0f, 11.0f), glm::vec3(glm::radians(45.0f), glm::radians(45.0f), 0), glm::vec3(1.0f, 1.0f, 1.0f));

LightSpot lightS(glm::vec3(0.0f, 10.0f, 0.0f), glm::vec3(glm::radians(90.0f), 0, 0), glm::vec3(1.0f, 1.0f, 1.0f));
float last_x, last_y;
bool first_mouse = true;
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
unsigned int LoadImageToGPU(const char* path, GLint internalFormat, GLenum format, int textureSlot, int& width, int& height, int& nrchannels);
unsigned int loadCubemap(std::vector<std::string> faces);
void mouse_call_back(GLFWwindow *window, double xPos, double yPos){
    if (first_mouse==true){
        last_x = xPos;
        last_y = yPos;
    }
    
    first_mouse = false;
    float delta_x, delta_y;
    delta_x = -(xPos - last_x);
    delta_y = -(yPos - last_y);
    last_x = xPos;
    last_y = yPos;
    camera.ProcessMouseMovement(delta_x, delta_y);
        
        // std::cout << xPos << std::endl;
    
}
// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main()
{
    // std::cout << argv[0] << std::endl;
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif
    /*
    glm::vec4 vec(1.0f, 0, 0, 1.0f);
    glm::mat4 trans;
    
    trans = glm::translate(trans, glm::vec3(2.0f, 0, 0));
    vec = trans * vec;
    std::cout << vec.x << " " << vec.y << " " << vec.z << std::endl;
     */
    
    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_call_back);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    int width, height, nrchannels;
    
    // shaders
    //Shader* shader = new Shader("./Shaders/reflectShader.vert", "./Shaders/reflectShader.frag");
    //Shader* shader = new Shader("./Shaders/explodeVertex.vert", "./Shaders/explodeFragment.frag", "./Shaders/explodeGeom.geom");
    Shader* shader = new Shader("./Shaders/explodeVertex.vert", "./Shaders/explodeFragment.frag");
    Shader* furShader = new Shader("./Shaders/furV.vert", "./Shaders/furF.frag", "./Shaders/furG.geom");
    //Shader* explodeShader = new Shader("./Shaders/explodeShader.vert", "./Shaders/explodeFragment.frag");
    Shader* houseShader = new Shader("./Shaders/houseVertex.vert", "./Shaders/house.frag", "./Shaders/houseGemo.geom");
    Shader* skybox_shader = new Shader("./Shaders/skybox_shader.vert", "./Shaders/skybox_shader.frag");
    Shader* shaderRed = new Shader("./Shaders/advanced_glsl.vert", "./Shaders/fragmentRed.frag");
    Shader* shaderGreen = new Shader("./Shaders/advanced_glsl.vert", "./Shaders/fragmentGreen.frag");
    Shader* shaderBlue = new Shader("./Shaders/advanced_glsl.vert", "./Shaders/fragmentBlue.frag");
    Shader* shaderYellow = new Shader("./Shaders/advanced_glsl.vert", "./Shaders/fragmentYellow.frag");
    Material* material = new Material(shader,
                                      LoadImageToGPU("container2.png", GL_RGBA, GL_RGBA, Shader::DIFFUSE, width, height, nrchannels),
                                      LoadImageToGPU("container2_specular.png", GL_RGBA, GL_RGBA, Shader::SPECULAR, width, height, nrchannels),
                                      glm::vec3(1.0f, 1.0f, 1.0f),
                                      32.0f);
    // data
    float cubeVertices[] = {
        // positions
        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,

        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,

         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,

        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
    };
    float vertices[] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
    };
    glm::vec3 cubePositions[] = {
      glm::vec3( 0.0f,  0.0f,  0.0f),
      glm::vec3( 2.0f,  5.0f, -15.0f),
      glm::vec3(-1.5f, -2.2f, -2.5f),
      glm::vec3(-3.8f, -2.0f, -12.3f),
      glm::vec3( 2.4f, -0.4f, -3.5f),
      glm::vec3(-1.7f,  3.0f, -7.5f),
      glm::vec3( 1.3f, -2.0f, -2.5f),
      glm::vec3( 1.5f,  2.0f, -2.5f),
      glm::vec3( 1.5f,  0.2f, -1.5f),
      glm::vec3(-1.3f,  1.0f, -1.5f)
    };
    std::vector<std::string> faces{
        "right.jpg",
        "left.jpg",
        "top.jpg",
        "bottom.jpg",
        "back.jpg",
        "front.jpg"
    };
    
    float housePoints[] = {
        //--position  /--color
        -0.5f,  0.5f, 1.0f, 0.0f, 0.0f,
        0.5f,  0.5f, 0.0f, 1.0f, 0.0f,
        0.5f, -0.5f, 1.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.0f, 0.0f, 1.0f
    };
    
    unsigned int cubemapTexture = loadCubemap(faces);
    
    float skyboxVertices[] = {
        // positions
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };
   
    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    
    //Mesh cube(vertices);
    Model Solider("./model/nanosuit/nanosuit.obj");
    // skyVAO, skyVBO
    unsigned int skyVAO;
    glGenVertexArrays(1, &skyVAO);
    glBindVertexArray(skyVAO);
    unsigned int skyVBO;
    glGenBuffers(1, &skyVBO);
    glBindBuffer(GL_ARRAY_BUFFER, skyVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(6);
    glVertexAttribPointer(6, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    
    unsigned int houseVAO, houseVBO;
    glGenVertexArrays(1, &houseVAO);
    glBindVertexArray(houseVAO);
    glGenBuffers(1, &houseVBO);
    glBindBuffer(GL_ARRAY_BUFFER, houseVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(housePoints), &housePoints, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(2*sizeof(float)));
    glBindVertexArray(0);
    
    
    unsigned int cubeVAO, cubeVBO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    
    

    
/*
    unsigned int VAO;
    unsigned int VBO[2];
    glGenVertexArrays(1, &VAO);
    glGenBuffers(2, VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);
 
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // 输入参数：栏位， 几个数据构成一个点， 数据类型， 是否需要归一化，数据间隔， 数据起始偏移
    glVertexAttribPointer(6, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)0);
    glEnableVertexAttribArray(6);
//    glVertexAttribPointer(7, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(3*sizeof(float)));
//    glEnableVertexAttribArray(7);
    glVertexAttribPointer(7, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(7);
    glVertexAttribPointer(8, 2, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(6*sizeof(float)));
    glEnableVertexAttribArray(8);
 */
    // ------------材质------------
    stbi_set_flip_vertically_on_load(true);
//    unsigned int textureA, textureB;
    
    
    
//    textureA = LoadImageToGPU("container.jpg", GL_RGB, GL_RGB, 0, width, height, nrchannels);
//    textureB = LoadImageToGPU("awesomeface.png", GL_RGBA, GL_RGBA, 1, width, height, nrchannels);
    

    //-----advanced GLSL-----
    unsigned int uniformBlockIndexRed = glGetUniformBlockIndex(shaderRed->ID, "Matrices");
    unsigned int uniformBlockIndexGreen = glGetUniformBlockIndex(shaderGreen->ID, "Matrices");
    unsigned int uniformBlockIndexBlue = glGetUniformBlockIndex(shaderBlue->ID, "Matrices");
    unsigned int uniformBlockIndexYellow = glGetUniformBlockIndex(shaderYellow->ID, "Matrices");
     
    //-----绑定点-----
    glUniformBlockBinding(shaderRed->ID, uniformBlockIndexRed, 0);
    glUniformBlockBinding(shaderRed->ID, uniformBlockIndexGreen, 0);
    glUniformBlockBinding(shaderRed->ID, uniformBlockIndexBlue, 0);
    glUniformBlockBinding(shaderRed->ID, uniformBlockIndexYellow, 0);
    //-----创建Buffer-----
    unsigned int uboMatrices;
    glGenBuffers(1, &uboMatrices);
    glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
    glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW); //c此时缓冲大小确定，但是缓冲里没有内容
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    
    glBindBufferRange(GL_UNIFORM_BUFFER, 0, uboMatrices, 0, 2 * sizeof(glm::mat4)); //将uboMatrices这个缓冲的所有部分都绑定到b绑定点0上
    //glBindVertexArray(VAO);
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)width/(float)height, 0.1f, 100.0f);
    glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(projection)); // 将投影矩阵填入缓冲中
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    
   
    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // render loop
    // -----------
    //float timeValue;
    //shader->use();
    /*
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, material->diffuse);
    glActiveTexture(GL_TEXTURE0+1);
    glBindTexture(GL_TEXTURE_2D, material->specular);
     */
    
//    glUniform1i(glGetUniformLocation(shader->ID, "ourTexture"), 0);
//    glUniform1i(glGetUniformLocation(shader->ID, "ourFace"), 3);
    /* 灯光效果关闭
    glUniform3f(glGetUniformLocation(shader->ID, "objColor"), 1.0f, 1.0f, 1.0f);
//    glUniform3f(glGetUniformLocation(shader->ID, "lightColor"), light.colors.x, light.colors.y, light.colors.z);
//    glUniform3f(glGetUniformLocation(shader->ID, "lightPos"), light.position.x, light.position.y, light.position.z);
//    glUniform3f(glGetUniformLocation(shader->ID, "lightDirUniform"), light.position.x, light.position.y, light.position.z);
    glUniform3f(glGetUniformLocation(shader->ID, "abmientColor"), 0.1f, 0.1f, 0.1f);
    glUniform3f(glGetUniformLocation(shader->ID, "cameraPos"), camera.Position.x, camera.Position.y, camera.Position.z);
    
    glUniform3f(glGetUniformLocation(shader->ID, "lightD.pos"), lightD.position.x, lightD.position.y, lightD.position.z);
    glUniform3f(glGetUniformLocation(shader->ID, "lightD.dirToLight"), lightD.direction.x, lightD.direction.y, lightD.direction.z);
    glUniform3f(glGetUniformLocation(shader->ID, "lightD.color"), lightD.colors.x, lightD.colors.y, lightD.colors.z);
    
    glUniform3f(glGetUniformLocation(shader->ID, "lightP0.pos"), lightP0.position.x, lightP0.position.y, lightP0.position.z);
    glUniform3f(glGetUniformLocation(shader->ID, "lightP0.dirToLight"), lightP0.direction.x, lightP0.direction.y, lightP0.direction.z);
    glUniform3f(glGetUniformLocation(shader->ID, "lightP0.color"), lightP0.colors.x, lightP0.colors.y, lightP0.colors.z);
    glUniform1f(glGetUniformLocation(shader->ID, "lightP0.constant"), lightP0.constantParam);
    glUniform1f(glGetUniformLocation(shader->ID, "lightP0.linear"), lightP0.linear);
    glUniform1f(glGetUniformLocation(shader->ID, "lightP0.quadratic"), lightP0.quadratic);
    
    glUniform3f(glGetUniformLocation(shader->ID, "lightP1.pos"), lightP1.position.x, lightP1.position.y, lightP1.position.z);
    glUniform3f(glGetUniformLocation(shader->ID, "lightP1.dirToLight"), lightP1.direction.x, lightP1.direction.y, lightP1.direction.z);
    glUniform3f(glGetUniformLocation(shader->ID, "lightP1.color"), lightP1.colors.x, lightP1.colors.y, lightP1.colors.z);
    glUniform1f(glGetUniformLocation(shader->ID, "lightP1.constant"), lightP1.constantParam);
    glUniform1f(glGetUniformLocation(shader->ID, "lightP1.linear"), lightP1.linear);
    glUniform1f(glGetUniformLocation(shader->ID, "lightP1.quadratic"), lightP1.quadratic);
    
    glUniform3f(glGetUniformLocation(shader->ID, "lightP2.pos"), lightP2.position.x, lightP2.position.y, lightP2.position.z);
    glUniform3f(glGetUniformLocation(shader->ID, "lightP2.dirToLight"), lightP2.direction.x, lightP2.direction.y, lightP2.direction.z);
    glUniform3f(glGetUniformLocation(shader->ID, "lightP2.color"), lightP2.colors.x, lightP2.colors.y, lightP2.colors.z);
    glUniform1f(glGetUniformLocation(shader->ID, "lightP2.constant"), lightP2.constantParam);
    glUniform1f(glGetUniformLocation(shader->ID, "lightP2.linear"), lightP2.linear);
    glUniform1f(glGetUniformLocation(shader->ID, "lightP2.quadratic"), lightP2.quadratic);
    
    glUniform3f(glGetUniformLocation(shader->ID, "lightP3.pos"), lightP3.position.x, lightP3.position.y, lightP3.position.z);
    glUniform3f(glGetUniformLocation(shader->ID, "lightP3.dirToLight"), lightP3.direction.x, lightP3.direction.y, lightP3.direction.z);
    glUniform3f(glGetUniformLocation(shader->ID, "lightP3.color"), lightP3.colors.x, lightP3.colors.y, lightP3.colors.z);
    glUniform1f(glGetUniformLocation(shader->ID, "lightP3.constant"), lightP3.constantParam);
    glUniform1f(glGetUniformLocation(shader->ID, "lightP3.linear"), lightP3.linear);
    glUniform1f(glGetUniformLocation(shader->ID, "lightP3.quadratic"), lightP3.quadratic);
    
    glUniform3f(glGetUniformLocation(shader->ID, "lightS.pos"), lightS.position.x, lightS.position.y, lightS.position.z);
    glUniform3f(glGetUniformLocation(shader->ID, "lightS.dirToLight"), lightS.direction.x, lightS.direction.y, lightS.direction.z);
    glUniform3f(glGetUniformLocation(shader->ID, "lightS.color"), lightS.colors.x, lightS.colors.y, lightS.colors.z);
    glUniform1f(glGetUniformLocation(shader->ID, "lightS.constant"), lightS.constantParam);
    glUniform1f(glGetUniformLocation(shader->ID, "lightS.linear"), lightS.linear);
    glUniform1f(glGetUniformLocation(shader->ID, "lightS.quadratic"), lightS.quadratic);
    glUniform1f(glGetUniformLocation(shader->ID, "lightS.cosInnerPhy"), lightS.cosInnerPhy);
    glUniform1f(glGetUniformLocation(shader->ID, "lightS.cosOuterPhy"), lightS.cosOuterPhy);
    
    material->shader->SetUniform3f("material.ambient", material->ambient);
    material->shader->SetUniform1i("material.diffuse", Shader::DIFFUSE);
    material->shader->SetUniform1i("material.specular", Shader::SPECULAR);
    glUniform1f(glGetUniformLocation(shader->ID, "material.shininess"), material->shininess);
//    glUniform1f(glGetUniformLocation(shader->ID, "lightS.cosInnerPhy"), light.cosInnerPhy);
//    glUniform1f(glGetUniformLocation(shader->ID, "lightS.cosOuterPhy"), light.cosOuterPhy);
//    glUniform1f(glGetUniformLocation(shader->ID, "lightP.constantParam"), light.constantParam);
//    glUniform1f(glGetUniformLocation(shader->ID, "lightP.linear"), light.linear);
//    glUniform1f(glGetUniformLocation(shader->ID, "lightP.quadratic"), light.quadratic);
    */
//Initial camera class
    
    
    glm::mat4 model_mat;
    glm::mat4 view_mat;
    glm::mat4 proj_mat = glm::perspective(glm::radians(45.0f), (float)(width)/(float)(height), 0.1f, 100.f);
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);
        unsigned int projLoc, viewLoc;
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //* --关闭自定义模型显示
        // 绘制自定义模型
        material->shader->use();
        model_mat = glm::mat4(1.0f);
        model_mat = glm::translate(model_mat, glm::vec3(0.0f, -10.0f, -10.0f));
        // model_mat = glm::scale(model_mat, glm::vec3(0.5f, 0.5f, 0.5f));
        //model_mat = glm::rotate(model_mat, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));

        proj_mat = glm::perspective(glm::radians(45.0f), (float)(width)/(float)(height), 0.1f, 100.f);
        projLoc = glGetUniformLocation(material->shader->ID, "proj");
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj_mat));
        viewLoc = glGetUniformLocation(material->shader->ID, "view");
        view_mat = camera.GetViewMatrix();
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view_mat));
        unsigned int modelLoc = glGetUniformLocation(material->shader->ID, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model_mat));
        material->shader->setFloat("time", glfwGetTime());
        //material->shader->use();
        Solider.Draw(material->shader);
        
        furShader->use();
        proj_mat = glm::perspective(glm::radians(45.0f), (float)(width)/(float)(height), 0.1f, 100.f);
        projLoc = glGetUniformLocation(furShader->ID, "projection");
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj_mat));
        viewLoc = glGetUniformLocation(furShader->ID, "view");
        view_mat = camera.GetViewMatrix();
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view_mat));
        modelLoc = glGetUniformLocation(furShader->ID, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model_mat));
        Solider.Draw(furShader);
        // 绘制天空盒
        // -----------------
        glDepthMask(GL_FALSE);
        skybox_shader->use();
        skybox_shader->SetUniform1i("skybox", 0);
        glm::mat4 model_mat = glm::mat4(1.0f);
        //model_mat = glm::translate(model_mat, glm::vec3(0.0f, -10.0f, -10.0f));
       // model_mat = glm::scale(model_mat, glm::vec3(0.5f, 0.5f, 0.5f));
       //model_mat = glm::rotate(model_mat, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
        glm::mat4 proj_mat = glm::perspective(glm::radians(45.0f), (float)(width)/(float)(height), 0.1f, 100.f);
        projLoc = glGetUniformLocation(skybox_shader->ID, "projection");
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj_mat));
        view_mat = glm::mat4(glm::mat3(camera.GetViewMatrix()));
        viewLoc = glGetUniformLocation(skybox_shader->ID, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view_mat));
        glBindVertexArray(skyVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthMask(GL_TRUE);
        
        
        //-----绘制四个立方体-----
        /*
        glm::mat4 view = camera.GetViewMatrix();
        glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
        glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(view));
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
        
        glBindVertexArray(cubeVAO);
        shaderRed->use();
        glm::mat4 model;
        model = glm::translate(model, glm::vec3(-0.75f, 0.75f, 0.0f));  // 移动到左上角
        shaderRed->SetUniform4m("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        
        shaderGreen->use();
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.75f, 0.75f, 0.0f));  // 移动到左上角
        shaderGreen->SetUniform4m("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        
        shaderBlue->use();
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-0.75f, -0.75f, 0.0f));  // 移动到左上角
        shaderBlue->SetUniform4m("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        
        shaderYellow->use();
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.75f, -0.75f, 0.0f));  // 移动到左上角
        shaderYellow->SetUniform4m("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        */
//        houseShader->use();
//        glBindVertexArray(houseVAO);
//        glDrawArrays(GL_POINTS, 0, 4);
        glBindVertexArray(0);
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
        //std::this_thread::sleep_for(std::chrono::milliseconds(50  ));
        
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
        camera.MoveY(0.05f);
    }
    else if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
        camera.MoveY(-0.05f);
    }
    else{
        camera.MoveY(0.0f);
    }
    
    
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
        camera.MoveX(0.05f);
    }
    else if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
        camera.MoveX(-0.05f);
    }
    else{
        camera.MoveX(0.0f);
    }
    
    if(glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS){
        camera.MoveZ(0.05f);
    }
    else if(glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS){
        camera.MoveZ(-0.05f);
    }
    else{
        camera.MoveZ(0.0f);
    }
        
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_BACK);
}

unsigned int LoadImageToGPU(const char* path, GLint internalFormat, GLenum format, int textureSlot, int& width, int& height, int& nrchannels){
    unsigned int TexBuffer;
    glGenTextures(1, &TexBuffer);
    glActiveTexture(GL_TEXTURE0 + textureSlot);
    glBindTexture(GL_TEXTURE_2D, TexBuffer);
    
    
    unsigned char* data = stbi_load(path, &width, &height, &nrchannels, 0);
    if(data){
        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else{
        std::cout << "Failed to load texture." << std::endl;
    }
    stbi_image_free(data);
    return TexBuffer;
    
}

unsigned int loadCubemap(std::vector<std::string> faces){
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
    
    int width, height, nrChannels;
    for(unsigned int i = 0; i < faces.size(); i++){
        unsigned char* data = stbi_load(("./skybox/" + faces[i]).c_str(), &width, &height, &nrChannels, 0);
        if(data){
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+i,
                         0,
                         GL_RGB,
                         width,
                         height,
                         0,
                         GL_RGB,
                         GL_UNSIGNED_BYTE,
                         data);
            std::cout << "Cube map box succeeded to load at path: " << ("./skybox/" + faces[i]) << std::endl;
            stbi_image_free(data);
        }
        else{
            std::cout << "Cube map box failed to load at path: " << ("./skybox/" + faces[i]) << std::endl;
            stbi_image_free(data);
        }
    }
    
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}
