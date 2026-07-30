#include "Particle.hpp"

Particle::Particle(int id, Vec3 pos, Vec3 vel, double mass, double q_e, double q_m, std::string calculator)
    : id(id), position(pos), velocity(vel), mass(mass), electric_charge(q_e), magnetic_charge(q_m), calculator(calculator) {}

void Particle::applyForce(const Vec3& force, double dt) {
    if (mass <= 0) return;

    // F = m * a  => a = F / m
    Vec3 acceleration = force / mass;

    // v = v_0 + a * dt
    velocity += acceleration * dt;
}

void Particle::update(double dt) {
    // p = p_0 + v * dt
    position += velocity * dt;
}
