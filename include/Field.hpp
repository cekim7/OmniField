#ifndef FIELD_HPP
#define FIELD_HPP

#include <unordered_map>
#include <vector>
#include <cmath>
#include <functional>
#include "Vector3.hpp"
#include "Particle.hpp"

// A simple structure to hold field values at a specific grid point
struct FieldPoint {
    Vec3 electric_field;
    Vec3 magnetic_field;

    FieldPoint() : electric_field(0,0,0), magnetic_field(0,0,0) {}
};

// Spatial hash to handle memory scalability regardless of domain size
struct GridIndex {
    long long x, y, z;

    bool operator==(const GridIndex& other) const {
        return x == other.x && y == other.y && z == other.z;
    }
};

// Custom hash function for GridIndex
struct GridIndexHash {
    std::size_t operator()(const GridIndex& k) const {
        // Simple hash combining x, y, z
        return std::hash<long long>()(k.x) ^
               (std::hash<long long>()(k.y) << 1) ^
               (std::hash<long long>()(k.z) << 2);
    }
};

class Field {
private:
    double grid_spacing; // Resolution of the field
    std::unordered_map<GridIndex, FieldPoint, GridIndexHash> grid;

    GridIndex getGridIndex(const Vec3& position) const;
    Vec3 getGridPosition(const GridIndex& index) const;

public:
    Field(double spacing);

    // Clears the field (often done at start of sweep)
    void clear();

    // Mapping from Particles to Field
    // Each particle deposits its influence (e.g. charge, current) onto the grid
    void updateFromParticles(const std::vector<Particle>& particles);

    // Mapping from Field to Particles
    // Calculates the forces exerted by the field on each particle
    void applyToParticles(std::vector<Particle>& particles, double dt) const;

    // Get field at exact position (interpolated or nearest neighbor)
    FieldPoint getFieldAt(const Vec3& position) const;
};

#endif // FIELD_HPP
