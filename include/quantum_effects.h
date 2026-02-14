#ifndef QUANTUM_EFFECTS_H
#define QUANTUM_EFFECTS_H

#include "particle.h"
#include "config.h"
#include <vector>

// ============================================================================
// Quantum Effects Manager
// ============================================================================

namespace QuantumEffects {
    
    // Superposition: Particles in multiple states simultaneously
    void UpdateSuperposition(std::vector<QuantumParticle>& particles, float dt, float simulation_time);
    
    // Uncertainty Principle: Position/Momentum tradeoff visualization
    void UpdateUncertainty(std::vector<QuantumParticle>& particles, float dt);
    
    // Entanglement: Non-local particle correlation
    void UpdateEntanglement(std::vector<QuantumParticle>& particles, float dt, float simulation_time);
    
    // Wave-Particle Duality: Toggle between wave and particle behavior
    void UpdateWaveParticleDuality(std::vector<QuantumParticle>& particles, float dt);
    
    // Measurement: Observation collapses wave function
    void UpdateMeasurement(std::vector<QuantumParticle>& particles, const Rectangle& measurement_zone, float dt);
    
    // Utility: Calculate probability field
    void CalculateProbabilityField(const std::vector<QuantumParticle>& particles, 
                                   std::vector<float>& field, 
                                   int width, int height, int cell_size);
}

#endif // QUANTUM_EFFECTS_H
