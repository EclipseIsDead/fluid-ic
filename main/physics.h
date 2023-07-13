#ifndef physics_h
#define physics_h
struct Particle {
    float x, y;  // Position
    float vx, vy;  // Velocity
    // Other properties like mass, density, etc. can be added as per SPH requirements
};
void updateParticles(std::vector<Particle>& particles);
#endif
