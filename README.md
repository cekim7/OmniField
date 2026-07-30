# Particle Field Simulation

A C++17 simulation engine for particle and electromagnetic field mutual interactive mapping.

## Features

- Adaptive Octree spatial structure for length-scale agnostic field resolution.
- Macroscopic mesh objects that switch to discrete particle interactions upon contact.
- Particle and electromagnetic field mutual interactive mapping.

## Building

The project uses CMake for its build system. To build the project, run the following commands:

```bash
mkdir build
cd build
cmake ..
make
```

## Running

The compiled simulation executable is named `ParticleFieldSim`. You can run it from within the `build` directory:

```bash
./ParticleFieldSim
```
