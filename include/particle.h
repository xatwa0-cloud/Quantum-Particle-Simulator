#ifndef QUANTUM_PARTICLE_H
#define QUANTUM_PARTICLE_H

#include "raylib.h"
#include "raymath.h"

// ============================================================================
// Quantum Particle Structure
// ============================================================================

struct QuantumParticle {
    // Physical properties
    Vector2 position;
    Vector2 velocity;
    Vector2 acceleration;
    
    // Quantum properties
    float probability;              // Probability of being in this state (0-1)
    float superposition_phase;      // Wave phase for superposition
    float spin;                     // Spin state (-1, 0, or 1)
    
    // Visual properties
    float radius;
    Color color;
    
    // State tracking
    bool measured;                  // Has measurement occurred?
    bool is_entangled;              // Is particle entangled?
    int entangled_partner;          // Index of partner particle
    
    // Wave behavior properties
    int movement_mode;              // 0 = particle, 1 = wave-like
    float wave_amplitude;
    float wave_frequency;
    float wave_time;
    
    // Constructor
    QuantumParticle(Vector2 pos = {0, 0}, float prob = 1.0f);
    
    // Methods
    void Update(float dt);
    void Render() const;
    void WrapPosition(int screen_width, int screen_height);
};

#endif // QUANTUM_PARTICLE_H
