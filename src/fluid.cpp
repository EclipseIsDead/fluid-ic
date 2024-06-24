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
#include <algorithm>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const int WIDTH = 80;
const int HEIGHT = 60;
const int ITERATIONS = 20;

struct FluidCell {
    double density;
    double vx, vy;
};

std::vector<FluidCell> fluid(WIDTH * HEIGHT);
std::vector<char> asciiGrid(WIDTH * HEIGHT);

const char ASCII_CHARS[] = " .:-=+*#%@";
const int ASCII_LEVELS = sizeof(ASCII_CHARS) / sizeof(char) - 1;

void initializeFluid() {
    for (int i = 0; i < WIDTH * HEIGHT; i++) {
        fluid[i].density = 1.0;
        fluid[i].vx = 0.0;
        fluid[i].vy = 0.0;
    }
}

void addForce(int x, int y, double fx, double fy) {
    int index = y * WIDTH + x;
    fluid[index].vx += fx;
    fluid[index].vy += fy;
}

void updateFluid() {
    std::vector<FluidCell> newFluid(WIDTH * HEIGHT);

    for (int y = 1; y < HEIGHT - 1; y++) {
        for (int x = 1; x < WIDTH - 1; x++) {
            int index = y * WIDTH + x;
            double density = fluid[index].density;
            double vx = fluid[index].vx;
            double vy = fluid[index].vy;

            // should upgrade this later
            for (int dy = -1; dy <= 1; dy++) {
                for (int dx = -1; dx <= 1; dx++) {
                    int ni = (y + dy) * WIDTH + (x + dx);
                    newFluid[index].density += fluid[ni].density * 0.2;
                    newFluid[index].vx += fluid[ni].vx * 0.2;
                    newFluid[index].vy += fluid[ni].vy * 0.2;
                }
            }

            newFluid[index].density /= 9.0;
            newFluid[index].vx /= 9.0;
            newFluid[index].vy /= 9.0;

            // apply delta v's
            int targetX = x + static_cast<int>(vx);
            int targetY = y + static_cast<int>(vy);
            targetX = std::max(0, std::min(WIDTH - 1, targetX));
            targetY = std::max(0, std::min(HEIGHT - 1, targetY));
            int targetIndex = targetY * WIDTH + targetX;

            newFluid[targetIndex].density += density * 0.8;
            newFluid[targetIndex].vx += vx * 0.8;
            newFluid[targetIndex].vy += vy * 0.8;
        }
    }

    fluid = newFluid;
}

void updateAsciiGrid() {
    for (int i = 0; i < WIDTH * HEIGHT; i++) {
        double value = std::sqrt(fluid[i].vx * fluid[i].vx + fluid[i].vy * fluid[i].vy);
        int charIndex = static_cast<int>(value * (ASCII_LEVELS - 1));
        charIndex = std::max(0, std::min(ASCII_LEVELS - 1, charIndex));
        asciiGrid[i] = ASCII_CHARS[charIndex];

        if (i == 0) {
            std::cout << "ASCII char at (0,0): " << asciiGrid[i] << " (int: " << static_cast<int>(asciiGrid[i]) << ")" << std::endl;
        }
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

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
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

    // grab shaders
    Shader mainShader("/Users/sid/Documents/Workspace/fluid-ic/src/shaders/vertex.glsl", "/Users/sid/Documents/Workspace/fluid-ic/src/shaders/fragment.glsl");
    float vertices[] = {
            -0.5f, -0.5f,
             0.5f, -0.5f,
             0.5f,  0.5f,
            -0.5f,  0.5f
        };

    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    initializeFluid();

    while (!glfwWindowShouldClose(window)) {
        processInput(window);
        glClear(GL_COLOR_BUFFER_BIT);

        // Update fluid simulation
        for (int i = 0; i < ITERATIONS; ++i) {
            updateFluid();
        }
        updateAsciiGrid();

        mainShader.use();

        glBindVertexArray(VAO);

        for (int y = 0; y < HEIGHT; y++) {
            for (int x = 0; x < WIDTH; x++) {
                int index = y * WIDTH + x;
                char c = asciiGrid[index];

                float ndcX = (2.0f * x / WIDTH) - 1.0f + (1.0f / WIDTH);
                float ndcY = 1.0f - (2.0f * y / HEIGHT) - (1.0f / HEIGHT);

                mainShader.setVec2("charPosition", ndcX, ndcY);
                mainShader.setInt("asciiChar", static_cast<int>(c));

                glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

                if (x == 0 && y == 0) {
                    std::cout << "Rendering point at (" << ndcX << ", " << ndcY << ")" << std::endl;
                }
            }
        }

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
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
