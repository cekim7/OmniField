#include "MeshObject.hpp"
#include <cmath>
#include <cstdlib>

MeshObject::MeshObject(int id, Vec3 com, Vec3 vel, double mass, double radius)
    : id(id), center_of_mass(com), velocity(vel), total_mass(mass), radius(radius) {}

void MeshObject::applyForce(const Vec3& force, double dt) {
    if (total_mass <= 0) return;
    Vec3 acceleration = force / total_mass;
    velocity += acceleration * dt;
}

void MeshObject::update(double dt) {
    center_of_mass += velocity * dt;
}

bool MeshObject::checkCollision(const MeshObject& other) const {
    Vec3 diff = center_of_mass - other.center_of_mass;
    double dist2 = diff.x*diff.x + diff.y*diff.y + diff.z*diff.z;
    double min_dist = radius + other.radius;
    return dist2 <= (min_dist * min_dist);
}

std::vector<Particle> MeshObject::resolveCollisionToParticles(const MeshObject& other, int& particle_id_counter) {
    std::vector<Particle> generated_particles;

    if (!checkCollision(other)) {
        return generated_particles;
    }

    // Determine the collision midpoint
    Vec3 contact_point = (center_of_mass + other.center_of_mass) * 0.5;

    // Simulate breaking off chunks of the mesh as individual particles at the contact region
    int num_particles_to_generate = 10;
    double mass_per_particle = (total_mass * 0.01); // 1% of mass breaks off
    double charge_per_particle = 1e-6; // Mock charge

    for (int i = 0; i < num_particles_to_generate; ++i) {
        // Scatter around contact point slightly
        double offset_x = (static_cast<double>(rand()) / RAND_MAX - 0.5) * radius * 0.2;
        double offset_y = (static_cast<double>(rand()) / RAND_MAX - 0.5) * radius * 0.2;
        double offset_z = (static_cast<double>(rand()) / RAND_MAX - 0.5) * radius * 0.2;

        Vec3 p_pos = contact_point + Vec3(offset_x, offset_y, offset_z);

        // Inherit some velocity, scatter the rest
        Vec3 p_vel = velocity * 0.5 + Vec3(offset_x, offset_y, offset_z) * 10.0;

        Particle p(particle_id_counter++, p_pos, p_vel, mass_per_particle, charge_per_particle);
        generated_particles.push_back(p);
    }

    // Decrease mass of parent mesh
    total_mass -= mass_per_particle * num_particles_to_generate;

    return generated_particles;
}
