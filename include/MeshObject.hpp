#ifndef MESHOBJECT_HPP
#define MESHOBJECT_HPP

#include "Vector3.hpp"
#include "Particle.hpp"
#include <vector>

class MeshObject {
public:
    int id;
    Vec3 center_of_mass;
    Vec3 velocity;
    double total_mass;
    double radius; // Simplified bounding volume for collision detection

    MeshObject(int id, Vec3 com, Vec3 vel, double mass, double radius);

    // Apply external force (Newtonian mechanics)
    void applyForce(const Vec3& force, double dt);

    // Update position based on velocity
    void update(double dt);

    // Check collision with another object or external boundary
    bool checkCollision(const MeshObject& other) const;

    // Convert the local contact region into particles for detailed simulation
    std::vector<Particle> resolveCollisionToParticles(const MeshObject& other, int& particle_id_counter);
};

#endif // MESHOBJECT_HPP
