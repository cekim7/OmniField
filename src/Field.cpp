#include "Field.hpp"
#include <cmath>

Field::Field(double spacing) : grid_spacing(spacing) {}

void Field::clear() {
    grid.clear();
}

GridIndex Field::getGridIndex(const Vec3& position) const {
    return {
        static_cast<long long>(std::floor(position.x / grid_spacing)),
        static_cast<long long>(std::floor(position.y / grid_spacing)),
        static_cast<long long>(std::floor(position.z / grid_spacing))
    };
}

Vec3 Field::getGridPosition(const GridIndex& index) const {
    return Vec3(
        (index.x + 0.5) * grid_spacing,
        (index.y + 0.5) * grid_spacing,
        (index.z + 0.5) * grid_spacing
    );
}

void Field::updateFromParticles(const std::vector<Particle>& particles) {
    // In a real PIC (Particle-in-Cell) simulation, we would scatter charge/current
    // to nearby grid points and solve Poisson/Maxwell equations.
    // Here we do a simplified deposition: each particle adds a static
    // Coulomb-like electric field to its nearest grid point.

    // Constant for simplified Coulomb field: k_e
    const double k_e = 8.9875517923e9; // N m^2 / C^2

    for (const auto& p : particles) {
        if (p.electric_charge == 0.0) continue;

        GridIndex center_idx = getGridIndex(p.position);

        // Smear the particle's field effect to a 3x3x3 grid neighborhood
        // to approximate local field contribution.
        for (int dx = -1; dx <= 1; ++dx) {
            for (int dy = -1; dy <= 1; ++dy) {
                for (int dz = -1; dz <= 1; ++dz) {
                    GridIndex neighbor_idx = {center_idx.x + dx, center_idx.y + dy, center_idx.z + dz};
                    Vec3 cell_center = getGridPosition(neighbor_idx);

                    Vec3 r_vec = cell_center - p.position;
                    double r2 = r_vec.x*r_vec.x + r_vec.y*r_vec.y + r_vec.z*r_vec.z;

                    // Prevent singularity by adding a small softening parameter
                    double softening = grid_spacing * 0.1;
                    r2 += softening * softening;

                    double r = std::sqrt(r2);

                    // E = k_e * q / r^2 * (r_vec / r) = k_e * q * r_vec / r^3
                    Vec3 e_field_contribution = r_vec * (k_e * p.electric_charge / (r2 * r));

                    grid[neighbor_idx].electric_field += e_field_contribution;
                }
            }
        }
    }
}

FieldPoint Field::getFieldAt(const Vec3& position) const {
    GridIndex idx = getGridIndex(position);
    auto it = grid.find(idx);
    if (it != grid.end()) {
        return it->second;
    }
    return FieldPoint(); // Return zero field if not found in sparse grid
}

void Field::applyToParticles(std::vector<Particle>& particles, double dt) const {
    // Apply Lorentz Force: F = q * (E + v x B)
    for (auto& p : particles) {
        FieldPoint fp = getFieldAt(p.position);

        // E-field force
        Vec3 force_e = fp.electric_field * p.electric_charge;

        // B-field force (v x B)
        Vec3 v_x_B(
            p.velocity.y * fp.magnetic_field.z - p.velocity.z * fp.magnetic_field.y,
            p.velocity.z * fp.magnetic_field.x - p.velocity.x * fp.magnetic_field.z,
            p.velocity.x * fp.magnetic_field.y - p.velocity.y * fp.magnetic_field.x
        );
        Vec3 force_b = v_x_B * p.electric_charge;

        Vec3 total_force = force_e + force_b;

        p.applyForce(total_force, dt);
    }
}
