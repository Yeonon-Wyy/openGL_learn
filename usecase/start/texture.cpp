#include "log.h"
// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on
#include <iostream>
#include <memory>
#include <cmath>

#include "window.h"
#include "shader.h"
#include "texture.h"

bool  isWireframeMode = false;
float mixValue        = 0.0f;

void frameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    else if(key == GLFW_KEY_L && action == GLFW_PRESS)
    {
        isWireframeMode = !isWireframeMode;
        if(isWireframeMode)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
        else
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
    }
    else if(key == GLFW_KEY_UP && action == GLFW_PRESS)
    {
        mixValue += 0.1f;
        if(mixValue >= 1.0f)
        {
            mixValue = 1.0f;
        }
    }
    else if(key == GLFW_KEY_DOWN && action == GLFW_PRESS)
    {
        mixValue -= 0.1f;
        if(mixValue <= 0.0f)
        {
            mixValue = 0.0f;
        }
    }
}

int main()
{
    Window window(800, 600);
    window.setFrameBufferSizeCallback(frameBufferSizeCallback);
    window.setKeyCallback(keyCallback);
    auto* glfwWindow = window.glfwWindow();

    ShaderProgram shaderProgram("../../resource/shader/1-start/texture.vs", "../../resource/shader/1-start/texture.fs");
    Texture       texture("../../resource/texture/wall.jpg");
    // std::vector<float> borderColors = {1.0f, 1.0f, 0.0f, 1.0f};
    // texture.setWarpType(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER, borderColors);
    // texture.setFilterType(GL_LINEAR, GL_NEAREST);
    Texture textureFace("../../resource/texture/awesomeface.png");

    // clang-format off
    float vertices[] = {
    //     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
        0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // 右上
        0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // 右下
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // 左下
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // 左上
    };
    unsigned int indices[] = {  
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };
    // clang-format on

    // vao
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // vbo
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    shaderProgram.use();

    shaderProgram.setInt("texture1", 0);
    shaderProgram.setInt("texture2", 1);

    while(!glfwWindowShouldClose(glfwWindow))
    {

        // render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture.id());
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, textureFace.id());

        shaderProgram.use();
        shaderProgram.setFloat("mixValue", mixValue);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(glfwWindow);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}