#ifndef physics_h
#define physics_h
#include <glm/glm.hpp>
struct Particle {
    glm::vec2 Position, Velocity;
    glm::vec4 Color;

    Particle() 
      : Position(0.0f), Velocity(0.0f), Color(1.0f) { }
};    
void updateParticles(std::vector<Particle>& particles);
#endif
