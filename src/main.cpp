#include <iostream>
#include <string>

#include "stb_image.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "camera.h"

const int WIDTH = 1024;
const int HEIGHT = 720;

float lastx = WIDTH / 2, lasty = HEIGHT / 2;
bool first_mouse = true;

void framebuffer_size_callback(GLFWwindow *, int, int);
void process_input(GLFWwindow *);
void mouse_callback(GLFWwindow *, double, double);

GLFWwindow *init_window(int, int, const std::string &);
Camera camera(glm::vec3(0.0, 0.0, 3.0));

float delta_time = 0.0f;
float last_frame = 0.0f;

const int VERTICES_SIZE = 6 * 3;
float vertices[VERTICES_SIZE] = {
    0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
    0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f};

int main(void)
{
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = init_window(WIDTH, HEIGHT, "something");

    glfwMakeContextCurrent(window);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "failed to initialize glad" << std::endl;
        exit(-1);
    }

    Shader shader("shaders/vertex.glsl", "shaders/fragment.glsl");

    GLuint triangle_VBO, triangle_VAO;
    glGenBuffers(1, &triangle_VBO);
    glGenVertexArrays(1, &triangle_VAO);

    glBindVertexArray(triangle_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, triangle_VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * VERTICES_SIZE, &vertices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void *)(sizeof(float) * 3));
    glEnableVertexAttribArray(1);

    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window))
    {
        process_input(window);

        glClearColor(.0f, .0f, .0f, .0f);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 view = camera.get_view_matrix();
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
        glm::mat4 model = glm::mat4(1.0f);

        shader.use();
        shader.set_mat4("projection", projection);
        shader.set_mat4("view", view);
        shader.set_mat4("model", model);

        glBindVertexArray(triangle_VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();

    return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    (void)window;
    glViewport(0, 0, width, height);
}

void process_input(GLFWwindow *window)
{
    float current_frame = glfwGetTime();
    delta_time = current_frame - last_frame;
    last_frame = current_frame;

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        camera.move(Direction::FORWARD, delta_time);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        camera.move(Direction::BACKWARD, delta_time);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        camera.move(Direction::LEFT, delta_time);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        camera.move(Direction::RIGHT, delta_time);
    }
}

GLFWwindow *init_window(int width, int height, const std::string &title)
{
    GLFWwindow *window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

    if (window == nullptr)
    {
        std::cout << "failed to initialize window" << std::endl;
        glfwTerminate();
        exit(-1);
    }
    return window;
}

void mouse_callback(GLFWwindow *window, double x_pos, double y_pos)
{
    (void)window;
    if (first_mouse)
    {
        lastx = x_pos;
        lasty = y_pos;
        first_mouse = false;
    }

    float xoffset = x_pos - lastx;
    float yoffset = lasty - y_pos;

    lastx = x_pos;
    lasty = y_pos;

    camera.process_mouse(xoffset, yoffset);
}
