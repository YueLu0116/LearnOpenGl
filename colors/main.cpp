#define STB_IMAGE_IMPLEMENTATION
#include <std_image.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader_m.h"
#include "camera.h"

#include <iostream>

void on_frame_buffer_size(GLFWwindow* window, int width, int height);
void on_mouse(GLFWwindow* window, double xpos_in, double ypos_in);
void on_scroll(GLFWwindow* window, double xoffset, double yoffset);
void process_input(GLFWwindow* window);
unsigned int load_texture(const char *path);

// const vars
const unsigned int kWidth = 800;
const unsigned int kHeight = 600;
const std::string  kCube_shader_path = "/Users/yuelu/develop/Graphics/LearnOpenGl/colors/";
const std::string  kLight_shader_path = "/Users/yuelu/develop/Graphics/LearnOpenGl/colors/";

Camera camera(glm::vec3(0.0f, 0.0f, 6.0f));
float lastX = kWidth / 2.0f;
float lastY = kHeight / 2.0f;
bool bFirst_mouse = true;

// timing
float delta_time = 0.0f;
float last_frame = 0.0f;

// lighting
glm::vec3 light_pos(1.2f, 1.0f, 2.0f);

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    auto window = glfwCreateWindow(kWidth, kHeight, "LearnOpenGL", nullptr, nullptr);
    if(!window){
        std::cerr << "[Error] failed to create window\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, on_frame_buffer_size);
    glfwSetCursorPosCallback(window, on_mouse);
    glfwSetScrollCallback(window, on_scroll);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "[Error] failed to initialize GLAD" << std::endl;
        return -1;
    }
    glEnable(GL_DEPTH_TEST);

    // shaders: object
    Shader lighting_shader((kLight_shader_path+"color.vs").c_str(),
                           (kLight_shader_path+"color.fs").c_str());
    // shader: light source cube
    Shader lightcube_shader((kCube_shader_path+"cube.vs").c_str(),
                            (kCube_shader_path+"cube.fs").c_str());
    float vertices[] = {
            // positions          // normals           // texture coords
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
            0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
            0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
            0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
            0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
            -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

            0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
            0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
            0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
            0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
            0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
            0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
    };

    // positions all containers
    glm::vec3 cube_positions[] = {
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

    unsigned int vbo, cube_vao;
    glGenVertexArrays(1, &cube_vao);
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindVertexArray(cube_vao);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texcoords
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    unsigned int light_cube_vao;
    glGenVertexArrays(1, &light_cube_vao);
    glBindVertexArray(light_cube_vao);
    // we only need to bind to the VBO (to link it with glVertexAttribPointer), no need to fill it;
    // the VBO's data already contains all we need (it's already bound, but we do it again for educational purposes)
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    unsigned int diffuse_map = load_texture("/Users/yuelu/develop/Graphics/LearnOpenGl/common/resources/container2.png");
    unsigned int specular_map = load_texture("/Users/yuelu/develop/Graphics/LearnOpenGl/common/resources/container2_specular.png");
    lighting_shader.use();
    lighting_shader.setInt("material.diffuse", 0);
    lighting_shader.setInt("material.specular", 1);

    while(!glfwWindowShouldClose(window)){
        float current_time = static_cast<float>(glfwGetTime());
        delta_time = current_time - last_frame;
        last_frame = current_time;

        process_input(window);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::vec3 light_color;
        light_color.x = sin(glfwGetTime() * 2.0f);
        light_color.y = sin(glfwGetTime() * 0.7f);
        light_color.z = sin(glfwGetTime() * 1.3f);
        glm::vec3 diffuse_color = light_color * glm::vec3(0.5f);
        glm::vec3 ambient_color = light_color * glm::vec3(0.2f);

        lighting_shader.use();
        // lighting_shader.setVec3("light.position", light_pos);
        lighting_shader.setVec3("light.direction", -0.2f, -1.0f, -0.3f);
        lighting_shader.setVec3("viewPos", camera.Position);
        lighting_shader.setFloat("material.shininess", 32.0f);
        lighting_shader.setVec3("light.ambient",  ambient_color);
        lighting_shader.setVec3("light.diffuse",  diffuse_color);
        lighting_shader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);

        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom),
                                                (float)kWidth / (float)kHeight,
                                                0.1f,
                                                100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        lighting_shader.setMat4("projection", projection);
        lighting_shader.setMat4("view", view);

        // world transformation
        glm::mat4 model = glm::mat4(1.0f);
        lighting_shader.setMat4("model", model);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuse_map);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specular_map);

        // render the cube
        // glBindVertexArray(cube_vao);
        // glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(cube_vao);
        for(unsigned int i=0;i<10;i++) {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cube_positions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            lighting_shader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // also draw the lamp object
//        lightcube_shader.use();
//        lightcube_shader.setMat4("projection", projection);
//        lightcube_shader.setMat4("view", view);
//        model = glm::mat4(1.0f);
//        model = glm::translate(model, light_pos);
//        model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
//        lightcube_shader.setMat4("model", model);
//
//        glBindVertexArray(light_cube_vao);
//        glDrawArrays(GL_TRIANGLES, 0, 36);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &cube_vao);
    glDeleteVertexArrays(1, &light_cube_vao);
    glDeleteBuffers(1, &vbo);
    glfwTerminate();

    return 0;
}

void process_input(GLFWwindow* window){
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, delta_time);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, delta_time);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, delta_time);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, delta_time);
}

void on_frame_buffer_size(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void on_mouse(GLFWwindow* window, double xpos_in, double ypos_in) {
    float xpos = static_cast<float>(xpos_in);
    float ypos = static_cast<float>(ypos_in);

    if (bFirst_mouse)
    {
        lastX = xpos;
        lastY = ypos;
        bFirst_mouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void on_scroll(GLFWwindow* window, double xoffset, double yoffset) {
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

unsigned int load_texture(const char *path) {
    unsigned int texture_id;
    glGenTextures(1, &texture_id);

    int width, height, nr_components;
    unsigned char *data = stbi_load(path, &width, &height, &nr_components, 0);
    if (data) {
        GLenum format;
        if (nr_components == 1)
            format = GL_RED;
        else if (nr_components == 3)
            format = GL_RGB;
        else if (nr_components == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, texture_id);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }
    return texture_id;
}
