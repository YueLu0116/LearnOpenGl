#include <iostream>

#include "glad/glad.h"
#include <GLFW/glfw3.h>

constexpr uint32_t kWidth = 800;
constexpr uint32_t kHeight = 600;

// shaders
const char *vertex_shader_source = "#version 330 core\n"
                                 "layout (location = 0) in vec3 aPos;\n"
                                 "void main()\n"
                                 "{\n"
                                 "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                 "}\0";

const char *fragment_shader_source = "#version 330 core\n"
                              "out vec4 FragColor;\n"
                              "void main()\n"
                              "{\n"
                              "    FragColor = vec4(1.0f, 0.7f, 0.5f, 1.0f);\n"
                              "}\n";


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
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // vertex settings
    float vertex[] = {
            -0.5, -0.5, 0.0f,
            0.5, -0.5, 0.0f,
            0.0f, 0.5f, 0.0f,
    };
    unsigned int vbo, vao;
    glGenBuffers(1, &vbo);
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // shaders
    unsigned int vertex_shader = 0;
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_source, nullptr);
    glCompileShader(vertex_shader);
    int success;
    char info[512];
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if (success == 0) {
        glGetShaderInfoLog(vertex_shader, 512, nullptr, info);
        std::clog << "Failed to compile gl vertex shader. Reason: " << info << "\n";
        glDeleteShader(vertex_shader);
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo);
        glfwTerminate();
        return -2;
    }

    unsigned int fragment_shader = 0;
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_source, nullptr);
    glShaderSource(fragment_shader, 1, &fragment_shader_source, nullptr);
    glCompileShader(fragment_shader);
    success = 0;
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if (success == 0) {
        glGetShaderInfoLog(vertex_shader, 512, nullptr, info);
        std::clog << "Failed to compile gl fragment shader. Reason: " << info << "\n";
        glDeleteShader(fragment_shader);
        glDeleteShader(vertex_shader);
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo);
        glfwTerminate();
        return -3;
    }

    unsigned int shader_program = 0;
    shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);
    glDeleteShader(fragment_shader);
    glDeleteShader(vertex_shader);
    success = 0;
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if(success == 0){
        glGetProgramInfoLog(shader_program, 512, nullptr, info);
        std::clog << "Failed to link gl shaders. Reason: " << info << "\n";
        glDeleteProgram(shader_program);
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo);
        glfwTerminate();
        return -4;
    }

    // render loop
    while(!glfwWindowShouldClose(window)){

        process_input(window);

        glClearColor(0.2f, 0.3f, 0.4f, 0.5f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shader_program);
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteProgram(shader_program);
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glfwTerminate();
    return 0;
}
