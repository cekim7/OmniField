#include "System.hpp"
#include <iostream>
#include <vector>

int main() {
    std::cout << "Starting Particle-Field Simulation..." << std::endl;
    std::cout << "Example: Interface is treated as particles, bulk treated as mesh." << std::endl;

    // Initialize the system with a field grid spacing of 1.0 units
    SimulationSystem sim(1.0);

    // Add some initial particles
    // Particle(id, position, velocity, mass, electric_charge, magnetic_charge)
    sim.addParticle(Particle(0, Vec3(0, 0, 0), Vec3(1, 0, 0), 1.0, 1e-6));
    sim.addParticle(Particle(0, Vec3(2, 0, 0), Vec3(-1, 0, 0), 1.0, -1e-6));

    // Add macroscopic mesh objects (the 'bulk')
    // They will collide and their contact interface will be converted to discrete particles.
    // MeshObject(id, center_of_mass, velocity, mass, radius)
    sim.addMeshObject(MeshObject(1, Vec3(-2, 0, 0), Vec3(2, 0, 0), 1000.0, 2.0));
    sim.addMeshObject(MeshObject(2, Vec3(2, 0, 0), Vec3(-2, 0, 0), 1000.0, 2.0));

    // Run the simulation for a few steps
    int num_steps = 10;
    double dt = 0.1;

    for (int step = 0; step < num_steps; ++step) {
        std::cout << "--- Step " << step << " ---" << std::endl;
        sim.runStep(dt);
        std::cout << "Total Particles: " << sim.getParticleCount() << std::endl;
    }

    std::cout << "Simulation completed successfully." << std::endl;

    return 0;
}
