#include "simulator.h"
#include "quantum_effects.h"
#include "visualization.h"
#include "utils.h"
#include <cmath>

// construction teardown and one time setup

QuantumSimulator::QuantumSimulator()
    : active_mode(MODE_SUPERPOSITION),
      show_probability_field(false),
      paused(false),
      simulation_time(0.0f),
      measurement_active(false),
      rng(std::random_device{}()) {
    measurement_zone = {0, 0, 100, 100};
    InitializeParticles();
}

QuantumSimulator::~QuantumSimulator() {
    particles.clear();
}

// particle creation and simulation reset

void QuantumSimulator::InitializeParticles(int count) {
    particles.clear();
    
    std::uniform_real_distribution<float> dist_x(50, SCREEN_WIDTH - 50);
    std::uniform_real_distribution<float> dist_y(50, SCREEN_HEIGHT - 50);
    std::uniform_real_distribution<float> dist_spin(-1, 1);
    
    for (int i = 0; i < count; i++) {
        Vector2 pos = {dist_x(rng), dist_y(rng)};
        QuantumParticle p(pos);
        p.spin = (int)(dist_spin(rng) * 2) - 1;
        particles.push_back(p);
    }
}

void QuantumSimulator::Reset() {
    InitializeParticles();
    simulation_time = 0;
    show_probability_field = false;
}

// main simulation tick and drawing

void QuantumSimulator::Update(float dt) {
    if (paused) return;
    simulation_time += dt;
    switch (active_mode) {
        case MODE_SUPERPOSITION:
            QuantumEffects::UpdateSuperposition(particles, dt, simulation_time);
            break;
        case MODE_UNCERTAINTY:
            QuantumEffects::UpdateUncertainty(particles, dt);
            break;
        case MODE_ENTANGLEMENT:
            QuantumEffects::UpdateEntanglement(particles, dt, simulation_time);
            break;
        case MODE_WAVE_PARTICLE:
            QuantumEffects::UpdateWaveParticleDuality(particles, dt);
            break;
        case MODE_MEASUREMENT:
            QuantumEffects::UpdateMeasurement(particles, measurement_zone, dt);
            break;
    }
}
void QuantumSimulator::Render() const {
    if (active_mode == MODE_SUPERPOSITION && show_probability_field) {
        RenderProbabilityField();
    }
    for (const auto& p : particles) {
        p.Render();
    }
    RenderEntanglementLinks();
    if (active_mode == MODE_MEASUREMENT) {
        RenderMeasurementZone();
    }
    Visualization::DrawUI(*this);
}

// input driven state changes like modes pause and overlays

void QuantumSimulator::SetMode(int mode) {
    if (mode >= 0 && mode < MAX_MODES) {
        active_mode = mode;
        Reset();
    }
}

void QuantumSimulator::TogglePause() {
    paused = !paused;
}

void QuantumSimulator::ToggleProbabilityField() {
    if (active_mode == MODE_SUPERPOSITION) {
        show_probability_field = !show_probability_field;
    }
}

void QuantumSimulator::ToggleHelp() {
    show_help = !show_help;
}

void QuantumSimulator::UpdateMeasurementZone(Vector2 mouse_pos) {
    measurement_zone.x = mouse_pos.x - 50;
    measurement_zone.y = mouse_pos.y - 50;
}

// drawing extra effects for each mode

void QuantumSimulator::RenderProbabilityField() const {
    int cell_size = (int)PROBABILITY_FIELD_CELL_SIZE;
    std::vector<float> field;
    QuantumEffects::CalculateProbabilityField(particles, field, SCREEN_WIDTH, SCREEN_HEIGHT, cell_size);
    
    int cols = SCREEN_WIDTH / cell_size;
    for (int y = 0; y < SCREEN_HEIGHT; y += cell_size) {
        for (int x = 0; x < SCREEN_WIDTH; x += cell_size) {
            int idx = (y / cell_size) * cols + (x / cell_size);
            if (idx >= 0 && idx < (int)field.size()) {
                Color heat_color = Utils::ValueToHeatMapColor(field[idx]);
                heat_color.a = 50;
                DrawRectangle(x, y, cell_size, cell_size, heat_color);
            }
        }
    }
}

void QuantumSimulator::RenderEntanglementLinks() const {
    for (const auto& p : particles) {
        if (p.is_entangled && p.entangled_partner >= 0 && p.entangled_partner < (int)particles.size()) {
            const auto& partner = particles[p.entangled_partner];
            Color link_color = {255, 200, 100, (unsigned char)ENTANGLEMENT_LINK_ALPHA};
            DrawLine((int)p.position.x, (int)p.position.y,
                    (int)partner.position.x, (int)partner.position.y,
                    link_color);
        }
    }
}

void QuantumSimulator::RenderMeasurementZone() const {
    DrawRectangleLinesEx(measurement_zone, 2, YELLOW);
    DrawText("MEASUREMENT ZONE", (int)measurement_zone.x, (int)measurement_zone.y - 25, 14, YELLOW);
}
