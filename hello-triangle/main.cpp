#include <iostream>
#include <vector>

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

const char *fragment_shader_source_yellow = "#version 330 core\n"
                              "out vec4 FragColor;\n"
                              "void main()\n"
                              "{\n"
                              "    FragColor = vec4(1.0f, 0.7f, 0.5f, 1.0f);\n"
                              "}\n";

const char *fragment_shader_source_blue = "#version 330 core\n"
                                            "out vec4 FragColor;\n"
                                            "void main()\n"
                                            "{\n"
                                            "    FragColor = vec4(0.19f, 0.09f, 0.72f, 1.0f);\n"
                                            "}\n";

inline void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

inline void process_input(GLFWwindow* window) {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

bool genShaders(GLenum shader_types, const char* shader_source, unsigned int& shader) {
    shader = glCreateShader(shader_types);
    glShaderSource(shader, 1, &shader_source, nullptr);
    glCompileShader(shader);
    int success = 0;
    char info[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (success == 0) {
        glGetShaderInfoLog(shader, 512, nullptr, info);
        std::clog << "Failed to compile gl vertex shader. Reason: " << info << "\n";
        glDeleteShader(shader);
        return false;
    }
    return true;
}

bool genShaderProgram(const std::vector<unsigned int>& shader_vec, unsigned int& shader_program) {
    shader_program = glCreateProgram();
    for (auto shader : shader_vec) {
        glAttachShader(shader_program, shader);
    }
    glLinkProgram(shader_program);
    for (auto shader : shader_vec){
        glDeleteShader(shader);
    }
    int success = 0;
    char info[512];
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if (success == 0) {
        glGetProgramInfoLog(shader_program, 512, nullptr, info);
        std::clog << "Failed to link gl shaders. Reason: " << info << "\n";
        glDeleteProgram(shader_program);
        return false;
    }
    return true;
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
            1.0f,0.5f, 0.0f,
    };
    unsigned int indices[] ={
            0,1,2,
            1,3,2,
    };
    unsigned int vbo, vao, ebo;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof indices, indices, GL_STATIC_DRAW);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // shaders
    std::vector<unsigned int> shader_vec{};
    unsigned int vertex_shader = 0;
    bool ret = genShaders(GL_VERTEX_SHADER, vertex_shader_source, vertex_shader);
    if (!ret) {
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo);
        glfwTerminate();
        return -2;
    }
    shader_vec.push_back(vertex_shader);

    unsigned int fragment_shader = 0;
    ret = genShaders(GL_FRAGMENT_SHADER, fragment_shader_source_yellow, fragment_shader);
    if (!ret) {
        glDeleteShader(vertex_shader);
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo);
        glfwTerminate();
        return -3;
    }
    shader_vec.push_back(fragment_shader);

    unsigned int shader_program = 0;
    ret = genShaderProgram(shader_vec, shader_program);
    if (!ret) {
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
        //glDrawArrays(GL_TRIANGLES, 0, 6);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteProgram(shader_program);
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glfwTerminate();
    return 0;
}
