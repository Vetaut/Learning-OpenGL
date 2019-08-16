//
//  main.cpp
//  Window
//
//  Created by William Goniprow on 5/19/19.
//  Copyright © 2019 William Goniprow. All rights reserved.
//
#define STB_IMAGE_IMPLEMENTATION

#include <iostream>
#include <math.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "stb_image.h"
#include "shader.h"

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
    "layout (location = 0) in vec3 aPos;\n"                 // the position variable has attribute position 0
    "layout (location = 1) in vec3 aColor;\n"               // the color variable has attribute postion 1
    "out vec3 ourColor;"                                    // output a color to the fragment shader
    "void main() { \n"
    "   gl_Position = vec4(aPos, 1.0); \n"
    "   ourColor = aColor;"                                 // set ourColor to the input color we got from the vertex data
    "}\0";

const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "in vec3 ourColor;\n"                                   // the input var from the vertex shader (same type and name)
    "void main() { \n"
    "   FragColor = vec4(ourColor, 1.0);\n"
    "}\n\0";

/*
const char *fragmentShaderSource2 = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main() { \n"
"   FragColor = vec4(0.8f, 0.8f, 0.0f, 1.0f);\n"
"}\n\0";

const char *fragmentShaderSource3 = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main() { \n"
"   FragColor = vec4(0.1f, 0.0f, 0.8f, 1.0f);\n"
"}\n\0";
 */

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    
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
    
    // build and compile our shader program
    // ------------------------------------
    Shader ourShader("vShader.vs", "fShader.fs");
     
    float vertices[] = {
        // positions        // colors         // texture coords
         0.5f,  0.5f, 0.f,  1.0f, 0.0f, 0.0f, 1.0f, 1.0f,       // top right
        -0.5f, -0.5f, 0.f,  0.0f, 1.0f, 0.0f, 0.0f, 0.0f,       // bottom left
        -0.5f,  0.5f, 0.f,  0.0f, 0.0f, 1.0f, 0.0f, 1.0f,       // top left
         0.5f, -0.5f, 0.f,  0.0f, 0.0f, 1.0f, 1.0f, 0.0f        // bottom right
    };
    
    unsigned int indices[] {
        0, 1, 2,    // first triangle
        0, 1, 3
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    // 5. then set the color attributees pointer
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // 6. then set the vertex attributes pointer
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    unsigned int texture1, texture2;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load texture data
    stbi_set_flip_vertically_on_load(true);
    int width, height, nrChannels;
    unsigned char *data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
    if(data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
    // load texture2 data
    
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    
    data = stbi_load("awesomeface.png", &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
    
    ourShader.use();
    ourShader.setInt("texture1", 0);
    ourShader.setInt("texture2", 1);
    
    // Render Loop
    while(!glfwWindowShouldClose(window)) {
        // Input
        processInput(window);
        
        // Set background color
        // clear the colorbuffer
        glClearColor(0.5f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        // ... Drawing ...
        ourShader.use();
        ourShader.setFloat("mixture", 0.2f);
        
        glm::mat4 trans = glm::mat4(1.0f);
        trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
        trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0, 0.0, 1.0));
        
        unsigned int transformLoc = glGetUniformLocation(ourShader.ID, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
        
        // render the triangles
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);
        
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        
        float scaleAmount = sin(glfwGetTime());
        trans = glm::mat4(1.0f);
        trans = glm::scale(trans, glm::vec3(scaleAmount, scaleAmount, scaleAmount));
        
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, &trans[0][0]);
        
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        
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
