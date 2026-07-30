#ifndef SYSTEM_HPP
#define SYSTEM_HPP

#include "Particle.hpp"
#include "Field.hpp"
#include "MeshObject.hpp"
#include <vector>
#include <memory>

class SimulationSystem {
private:
    std::vector<Particle> particles;
    std::vector<MeshObject> meshes;
    Field em_field;
    int particle_id_counter;

public:
    // Initialize system with field spacing parameter
    SimulationSystem(double field_spacing);

    void addParticle(const Particle& p);
    void addMeshObject(const MeshObject& m);

    // Main simulation loop step
    void runStep(double dt);
};

#endif // SYSTEM_HPP
