//
//  main.cpp
//  Window
//
//  Created by William Goniprow on 5/19/19.
//  Copyright © 2019 William Goniprow. All rights reserved.
//

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
unsigned int setupShaderProgram(const char *vertexSource, const char *fragmentSource);
unsigned int setupVertexShader(const char *shaderSource);
unsigned int setupFragmentShader(const char *shaderSource);
void checkSetupShaderSuccess(unsigned int shader, std::string classifer);

//Settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main() { \n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);"
    "}\0";

const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main() { \n"
    "   FragColor = vec4(0.1f, 0.0f, 0.8f, 1.0f);\n"
    "}\n\0";

const char *fragmentShaderSource2 = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main() { \n"
"   FragColor = vec4(0.8f, 0.8f, 0.0f, 1.0f);\n"
"}\n\0";

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if(window == NULL) {
        std::cout << "Failed to create window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    
    //ensure GLAD is loaded
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    
    // Tell OpenGL the size of it's drawing space
    glViewport(0, 0, SCR_WIDTH, SCR_WIDTH);
    // Tell OpenGL if the user resized the window
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    unsigned int shaderProgram = setupShaderProgram(vertexShaderSource, fragmentShaderSource);
    unsigned int shaderProgram2 = setupShaderProgram(vertexShaderSource, fragmentShaderSource2);
    
    float vertices[] = {
         0.5f,  0.5f, 0.f,  // top right
        -0.5f,  0.5f, 0.f,  // top left
         0.5f, -0.5f, 0.f,  // bottom right
        -0.5f, -0.5f, 0.f,  // bottom left
    };
    
    unsigned int indices[] {
        0, 1, 3,    // first triangle
        0, 2, 3,    // second triangle
    };
    
    unsigned int VAO, VBO , EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    
    // 1. Bind the vertex array
    glBindVertexArray(VAO);
    // 2. copy our vertices into a buffer for OpenGL to use
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // 3. copy our index array into a buffer for OpenGL to use
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    // 4. then set the vertex attributes pointer
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    // Render Loop
    while(!glfwWindowShouldClose(window)) {
        // Input
        processInput(window);
        
        // Set background color
        glClearColor(0.5f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        // ... Drawing ...
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
        glUseProgram(shaderProgram2);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void *)(sizeof(float) * 3));
        
        // check and call all events and swap the buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    
    glfwTerminate();
    return 0;
}

// Deals with all input from user
void processInput(GLFWwindow *window) {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if(glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    if(glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    if(glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
}

// Changes the viewport size if window size changes
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

unsigned int setupShaderProgram(const char *vertexSource, const char *fragmentSource) {
    int success;
    char infoLog[512];
    unsigned int vertexShader = setupVertexShader(vertexSource);
    unsigned int fragmentShader = setupFragmentShader(fragmentSource);
    
    // Setup of Shader Program
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    
    // Bind the shaders to the program
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    
    // Check if it worked
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    
    // free shaders
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    return shaderProgram;
}

unsigned int setupVertexShader(const char *shaderSource) {
    // Creating vertex shader
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    // Compiling Shader
    glShaderSource(vertexShader, 1, &shaderSource, NULL);
    glCompileShader(vertexShader);
    
    checkSetupShaderSuccess(vertexShader, "VERTEX");
    
    return vertexShader;
}

unsigned int setupFragmentShader(const char *shaderSource) {
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &shaderSource, NULL);
    glCompileShader(fragmentShader);
    
    checkSetupShaderSuccess(fragmentShader, "FRAGMENT");

    return fragmentShader;
}

void checkSetupShaderSuccess(unsigned int shader, std::string classifer) {
    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    
    if(!success) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::" << classifer <<"::COMPILIATION_FAILED\n" << infoLog << std::endl;
    }
}
