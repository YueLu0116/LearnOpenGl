#include <iostream>

#include "glad/glad.h"
#include <GLFW/glfw3.h>

constexpr uint32_t kWidth = 800;
constexpr uint32_t kHeight = 600;

inline void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

inline void process_input(GLFWwindow* window) {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    auto window = glfwCreateWindow(kWidth, kHeight, "LearnOpenGl", nullptr, nullptr);
    if (!window) {
        std::clog << "Failed to create GLFW window\n";
        return -1;
    }
    glfwMakeContextCurrent(window);

    if(!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))){
        std::clog << "Failed to init glad\n";
        return -1;
    }

    glViewport(0, 0, kWidth, kHeight);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // render loop
    while(!glfwWindowShouldClose(window)){

        process_input(window);

        glClearColor(0.2f, 0.3f, 0.4f, 0.5f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
