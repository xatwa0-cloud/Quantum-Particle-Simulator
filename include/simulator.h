#ifndef QUANTUM_SIMULATOR_H
#define QUANTUM_SIMULATOR_H

#include "particle.h"
#include "config.h"
#include <vector>
#include <random>

// ============================================================================
// Quantum Simulator Class
// ============================================================================

class QuantumSimulator {
private:
    std::vector<QuantumParticle> particles;
    std::mt19937 rng;
    
    int active_mode;
    bool show_probability_field;
    bool paused;
    float simulation_time;
    
    // Measurement mode specific
    Rectangle measurement_zone;
    bool measurement_active;
    
public:
    // Constructor & Destructor
    QuantumSimulator();
    ~QuantumSimulator();
    
    // Initialization
    void InitializeParticles(int count = DEFAULT_PARTICLE_COUNT);
    void Reset();
    
    // Update & Render
    void Update(float dt);
    void Render() const;
    
    // Control Methods
    void SetMode(int mode);
    void TogglePause();
    void ToggleProbabilityField();
    void UpdateMeasurementZone(Vector2 mouse_pos);
    void ToggleHelp();
    
    // Getters
    bool IsPaused() const { return paused; }
    int GetMode() const { return active_mode; }
    const std::vector<QuantumParticle>& GetParticles() const { return particles; }
    bool ShowsProbabilityField() const { return show_probability_field; }
    bool ShowsHelp() const { return show_help; }
    
private:
    // Internal helpers
    void RenderProbabilityField() const;
    void RenderEntanglementLinks() const;
    void RenderMeasurementZone() const;

    bool show_help = false;
};

#endif // QUANTUM_SIMULATOR_H
