#include <cmath>
#include <iostream>
#include <filesystem>
#include <vector>

#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include "shader_s.h"

constexpr uint32_t kWidth = 800;
constexpr uint32_t kHeight = 600;

inline void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

inline void processInput(GLFWwindow* window) {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

inline void drawTris(const Shader &shader, unsigned int vao) {
    shader.use();
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void genVertexData(const std::vector<float>& vertex_vec, unsigned int& vao, unsigned int& vbo);
void genVertexWithColorData(const std::vector<float>& vertex_vec, unsigned int& vao, unsigned int& vbo);

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    auto window = glfwCreateWindow(kWidth, kHeight, "LearnOpenGl", nullptr, nullptr);
    if (!window) {
        std::clog << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if(!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))){
        std::clog << "Failed to init glad\n";
        glfwTerminate();
        return -1;
    }

    glViewport(0, 0, kWidth, kHeight);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    // vertex settings
    const std::vector<float> vertex_vec {
            -0.5, -0.5, 0.0f,  1.0f, 0.0f, 0.0f,
            0.5, -0.5, 0.0f,   0.0f, 1.0f, 0.0f,
            0.0f, 0.5f, 0.0f,  0.0f, 0.0f, 1.0f
    };

    unsigned int vbo, vao;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    genVertexWithColorData(vertex_vec, vao, vbo);

    // shaders
    auto cur_path = std::filesystem::current_path();
    std::clog << "current path:" << cur_path.string() << "\n";
    Shader shader{"../shader_vs.glsl", "../shader_fs.glsl"};

    // render loop
    while(!glfwWindowShouldClose(window)){

        processInput(window);

        glClearColor(0.2f, 0.3f, 0.4f, 0.5f);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw
        //auto timeValue = static_cast<float>(glfwGetTime());
        //float greenValue = std::sin(timeValue) / 2.0f + 0.5f;
        //shader.setFloat4("ourColor", std::vector<float>{0.0f, greenValue, 0.0f, 1.0f});
        //shader.setFloat("xOffset", 0.5f);
        drawTris(shader, vao);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glfwTerminate();
    return 0;
}

void genVertexData(const std::vector<float>& vertex_vec, unsigned int& vao, unsigned int& vbo) {
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * static_cast<GLsizeiptr>(vertex_vec.size()), vertex_vec.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)nullptr);
    glEnableVertexAttribArray(0);
}

void genVertexWithColorData(const std::vector<float>& vertex_vec, unsigned int& vao, unsigned int& vbo) {
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * static_cast<GLsizeiptr>(vertex_vec.size()), vertex_vec.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)nullptr);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);
}
