#include <iostream>
#include <vector>

#include "glad/glad.h"
#include <GLFW/glfw3.h>

#define SHOUD_RETURN_N3 if (!ret) { \
    glDeleteShader(vertex_shader);  \
    glDeleteVertexArrays(2, vao); \
    glDeleteBuffers(2, vbo); \
    glfwTerminate(); \
    return -3; \
}

#define SHOUD_RETURN_N4 if (!ret) { \
    glDeleteVertexArrays(2, vao); \
    glDeleteBuffers(2, vbo); \
    glfwTerminate(); \
    return -4; \
}

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

inline void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

inline void processInput(GLFWwindow* window) {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

inline void drawTris(unsigned int shader_program, unsigned int vao) {
    glUseProgram(shader_program);
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

bool genShaders(GLenum shader_types, const char* shader_source, unsigned int& shader);
bool genShaderProgram(const std::vector<unsigned int>& shader_vec, unsigned int& shader_program);
void genVertexData(const std::vector<float>& vertex_vec, unsigned int& vao, unsigned int& vbo);



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
    const std::vector<float> vertex1_vec {
            -0.5, -0.5, 0.0f,
            0.5, -0.5, 0.0f,
            0.0f, 0.5f, 0.0f,
    };
    const std::vector<float> vertex2_vec {
            0.5, -0.5, 0.0f,
            1.0f,0.5f, 0.0f,
            0.0f, 0.5f, 0.0f,
    };

    unsigned int vbo[2], vao[2];
    glGenVertexArrays(2, vao);
    glGenBuffers(2, vbo);
    genVertexData(vertex1_vec, vao[0], vbo[0]);
    genVertexData(vertex2_vec, vao[1], vbo[1]);

    // shaders
    std::vector<unsigned int> shader_yellow_vec{}, shader_blue_vec{};
    unsigned int vertex_shader = 0;
    bool ret = genShaders(GL_VERTEX_SHADER, vertex_shader_source, vertex_shader);
    if (!ret) {
        glDeleteVertexArrays(2, vao);
        glDeleteBuffers(2, vbo);
        glfwTerminate();
        return -2;
    }
    shader_yellow_vec.push_back(vertex_shader);
    shader_blue_vec.push_back(vertex_shader);

    unsigned int fragment_shader_yellow = 0;
    ret = genShaders(GL_FRAGMENT_SHADER, fragment_shader_source_yellow, fragment_shader_yellow);
    SHOUD_RETURN_N3
    shader_yellow_vec.push_back(fragment_shader_yellow);

    unsigned int fragment_shader_blue = 0;
    ret = genShaders(GL_FRAGMENT_SHADER, fragment_shader_source_blue, fragment_shader_blue);
    SHOUD_RETURN_N3
    shader_blue_vec.push_back(fragment_shader_blue);

    // link
    unsigned int shader_yellow_program, shader_blue_program = 0;
    ret = genShaderProgram(shader_yellow_vec, shader_yellow_program);
    SHOUD_RETURN_N4
    ret = genShaderProgram(shader_blue_vec, shader_blue_program);
    SHOUD_RETURN_N4

    // render loop
    while(!glfwWindowShouldClose(window)){

        processInput(window);

        glClearColor(0.2f, 0.3f, 0.4f, 0.5f);
        glClear(GL_COLOR_BUFFER_BIT);

        drawTris(shader_yellow_program, vao[0]);
        drawTris(shader_blue_program, vao[1]);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteProgram(shader_yellow_program);
    glDeleteProgram(shader_blue_program);
    glDeleteVertexArrays(2, vao);
    glDeleteBuffers(2, vbo);
    glfwTerminate();
    return 0;
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

void genVertexData(const std::vector<float>& vertex_vec, unsigned int& vao, unsigned int& vbo) {
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * static_cast<GLsizeiptr>(vertex_vec.size()), vertex_vec.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)nullptr);
    glEnableVertexAttribArray(0);
}