#include "System.hpp"
#include <iostream>

SimulationSystem::SimulationSystem(double field_spacing)
    : em_field(field_spacing), particle_id_counter(0) {}

void SimulationSystem::addParticle(const Particle& p) {
    Particle new_p = p;
    new_p.id = particle_id_counter++;
    particles.push_back(new_p);
}

void SimulationSystem::addMeshObject(const MeshObject& m) {
    meshes.push_back(m);
}

void SimulationSystem::runStep(double dt) {
    // 1. Clear the electromagnetic field for the current step
    em_field.clear();

    // 2. Particle-to-Field Mapping (P2F)
    em_field.updateFromParticles(particles);

    // 3. Field-to-Particle Mapping (F2P) and Particle Update
    em_field.applyToParticles(particles, dt);

    for (auto& p : particles) {
        p.update(dt);
    }

    // 4. Update Mesh Objects (Newtonian Mechanics)
    for (auto& m : meshes) {
        // Assume gravity or some external force if applicable
        // m.applyForce(Vec3(0, -9.81 * m.total_mass, 0), dt);
        m.update(dt);
    }

    // 5. Check Collisions among Mesh Objects and decompose to particles if colliding
    std::vector<Particle> new_particles;
    for (size_t i = 0; i < meshes.size(); ++i) {
        for (size_t j = i + 1; j < meshes.size(); ++j) {
            if (meshes[i].checkCollision(meshes[j])) {
                std::cout << "Collision detected between Mesh " << meshes[i].id << " and Mesh " << meshes[j].id << std::endl;

                auto p1 = meshes[i].resolveCollisionToParticles(meshes[j], particle_id_counter);
                auto p2 = meshes[j].resolveCollisionToParticles(meshes[i], particle_id_counter);

                new_particles.insert(new_particles.end(), p1.begin(), p1.end());
                new_particles.insert(new_particles.end(), p2.begin(), p2.end());
            }
        }
    }

    // Add newly generated collision particles to the main pool
    for (const auto& p : new_particles) {
        particles.push_back(p);
    }
}

size_t SimulationSystem::getParticleCount() const {
    return particles.size();
}
