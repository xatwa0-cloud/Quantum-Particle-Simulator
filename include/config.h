#ifndef QUANTUM_CONFIG_H
#define QUANTUM_CONFIG_H

// ============================================================================
// Global Configuration & Constants
// ============================================================================

// Window Configuration
const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 800;
const float FPS = 60.0f;

// Simulation Configuration
const int DEFAULT_PARTICLE_COUNT = 30;
const float DEFAULT_WAVE_AMPLITUDE = 20.0f;
const float DEFAULT_WAVE_FREQUENCY = 0.05f;

// Physics Scaling Factors
const float MOMENTUM_SCALE = 2.0f;
const float FORCE_SCALE = 30.0f;
const float DRAG_COEFFICIENT = 0.95f;
const float MAX_VELOCITY = 300.0f;
const float ENTANGLEMENT_PROBABILITY = 0.3f;

// Visualization
const float PROBABILITY_FIELD_CELL_SIZE = 40;
const float ENTANGLEMENT_LINK_ALPHA = 100;
const float PARTICLE_BASE_RADIUS = 4.0f;

// Quantum Modes
enum QuantumMode {
    MODE_SUPERPOSITION = 0,
    MODE_UNCERTAINTY = 1,
    MODE_ENTANGLEMENT = 2,
    MODE_WAVE_PARTICLE = 3,
    MODE_MEASUREMENT = 4,
    MAX_MODES = 5
};

#endif // QUANTUM_CONFIG_H
