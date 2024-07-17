/* Ask for an OpenGL Core Context */
#define GLFW_INCLUDE_NONE  // Macro Redefinition
#define GL_SILENCE_DEPRECATION  // THERE IS A REASON I AM USING 3.3

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "shader.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>
#include <cmath>
#include <random>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const int NUM_PARTICLES = 10000;
const float PARTICLE_SIZE = 2.0f;

struct Particle {
    glm::vec2 position;
    glm::vec2 velocity;
};

std::vector<Particle> particles(NUM_PARTICLES);

// Create a random number generator
std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<> dis(-1.0, 1.0);

// abuse auto because i'm lazy
void initializeParticles() {
    for (auto& particle : particles) {
        particle.position = glm::vec2(dis(gen), dis(gen));
        particle.velocity = glm::vec2(dis(gen), dis(gen)) * 0.01f;
    }
}

void updateParticles() {
    for (auto& particle : particles) {
        particle.position += particle.velocity;

        // Simple boundary conditions
        if (particle.position.x < -1.0f || particle.position.x > 1.0f) {
            particle.velocity.x *= -1.0f;
        }
        if (particle.position.y < -1.0f || particle.position.y > 1.0f) {
            particle.velocity.y *= -1.0f;
        }

        // Apply some "fluid-like" behavior
        particle.velocity += glm::vec2(0.0f, -0.001f); // Gravity
        particle.velocity *= 0.99f; // Damping
    }
}

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Fluid Simulator", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Create and compile shaders
    Shader particleShader("/Users/sid/Documents/Workspace/fluid-ic/src/shaders/vertex.glsl", "/Users/sid/Documents/Workspace/fluid-ic/src/shaders/fragment.glsl");

    // Generate a VBO and VAO for particle positions
    unsigned int VBO, VAO;
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * NUM_PARTICLES, NULL, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    initializeParticles();

    // Enable point size
    glEnable(GL_PROGRAM_POINT_SIZE);

    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        // Clear the screen
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Update particle positions
        updateParticles();

        // Update VBO with new particle positions
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec2) * NUM_PARTICLES, &particles[0].position);

        // Render particles
        particleShader.use();
        particleShader.setFloat("pointSize", PARTICLE_SIZE);

        glBindVertexArray(VAO);
        glDrawArrays(GL_POINTS, 0, NUM_PARTICLES);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}

// CALLBACKS

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
