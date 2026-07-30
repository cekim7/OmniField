#ifndef PARTICLE_HPP
#define PARTICLE_HPP

#include "Vector3.hpp"

class Particle {
public:
    int id;
    Vec3 position;
    Vec3 velocity;
    double mass;
    double electric_charge;
    double magnetic_charge; // Can be used for magnetic poles if applicable, or magnetic dipole moment

    Particle(int id, Vec3 pos, Vec3 vel, double mass, double q_e, double q_m = 0.0);

    // Apply a force to the particle, updating its velocity and then position
    void applyForce(const Vec3& force, double dt);

    // Simple Euler integration
    void update(double dt);
};

#endif // PARTICLE_HPP
