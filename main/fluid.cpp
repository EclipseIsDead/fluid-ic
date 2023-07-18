/* Ask for an OpenGL Core Context */
#define GLFW_INCLUDE_NONE  // Macro Redefinition
#define GL_SILENCE_DEPRECATION  // THERE IS A REASON I AM USING 3.3

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <iostream>

#include "physics.h"

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

// Constants and Globals
const int NUM_PARTICLES = 1;

std::vector<Particle> particles(NUM_PARTICLES);

// Callbacks
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}  

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

int main(int argc, char** argv)
{
  GLFWwindow* window;
  glfwMakeContextCurrent(window);

  /* Initialize the library */
  if ( !glfwInit() )
  {
     return -1;
  }

#ifdef __APPLE__
  /* We need to explicitly ask for a 3.3 context on OS X */
  glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif

  /* Create a windowed mode window and its OpenGL context */
  window = glfwCreateWindow( 800, 600, "fluid-ic", NULL, NULL );
  if (!window)
  {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }

  /* Make the window's context current */
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);  

  /* Loop until the user closes the window */
  while (!glfwWindowShouldClose(window))
  {
    processInput(window);
    glfwPollEvents();

    /* Render here */
    glClear(GL_COLOR_BUFFER_BIT);  // Clear the color buffer

    unsigned int VBO;
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(particles) * NUM_PARTICLES, particles.data(), GL_STATIC_DRAW);

    // Render particles as points
    glPointSize(3);
    glDrawArrays(GL_POINTS, 0, 1);
    glColor3f(1.0f, 1.0f, 1.0f); // Set particle color
    glVertex2f(particles[0].Position.x, particles[0].Position.y);

    // Update particles
    updateParticles(particles);

    glFinish();  // Flush the rendering pipeline

    glfwSwapBuffers(window);
  }

  // clear up resources
  glfwTerminate();
  return 0;
}