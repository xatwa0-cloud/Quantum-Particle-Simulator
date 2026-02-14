#include "quantum_effects.h"
#include "utils.h"
#include <cmath>

namespace QuantumEffects {

// superposition mode where particles wander based on an oscillating probability value

void UpdateSuperposition(std::vector<QuantumParticle>& particles, float dt, float simulation_time) {
    for (auto& p : particles) {
        p.superposition_phase += dt * 2.0f;
        p.probability = 0.5f + 0.5f * sin(p.superposition_phase);
        float angle = Utils::RandomAngle();
        float speed = 50.0f * p.probability;
        p.velocity = {cos(angle) * speed, sin(angle) * speed};
        int intensity = (int)(100 + 155 * p.probability);
        p.color = {(unsigned char)intensity, (unsigned char)(intensity * 0.5f), 255, 255};
        p.position = Vector2Add(p.position, Vector2Scale(p.velocity, dt));
        p.WrapPosition(SCREEN_WIDTH, SCREEN_HEIGHT);
    }
}

// uncertainty mode a rough sketch of position momentum tradeoff

void UpdateUncertainty(std::vector<QuantumParticle>& particles, float dt) {
    for (auto& p : particles) {
        float speed = Vector2Length(p.velocity);
        float momentum = speed;
        float position_uncertainty = 30.0f / (1.0f + momentum / 100.0f);
        float angle = Utils::RandomAngle();
        float uncertainty_force = Utils::RandomFloat(0, 1) * 100.0f;
        
        p.acceleration = {cos(angle) * uncertainty_force, sin(angle) * uncertainty_force};
        p.velocity = Vector2Add(p.velocity, Vector2Scale(p.acceleration, dt));
        p.velocity = Vector2Scale(p.velocity, DRAG_COEFFICIENT);
        p.position = Vector2Add(p.position, Vector2Scale(p.velocity, dt));
        
        p.radius = 2.0f + position_uncertainty / 10.0f;
        int momentum_intensity = (int)(50 + std::min(205.0f, momentum * 2.0f));
        p.color = {255, (unsigned char)momentum_intensity, 100, 255};
        
        p.WrapPosition(SCREEN_WIDTH, SCREEN_HEIGHT);
        p.velocity = Utils::ClampVectorMagnitude(p.velocity, MAX_VELOCITY);
    }
}

// entanglement mode where occasionally particles pair and keeps their spins in sync

void UpdateEntanglement(std::vector<QuantumParticle>& particles, float dt, float simulation_time) {
    if (particles.empty()) {
        return;
    }

    if ((int)(simulation_time * 2) % 60 == 0) {
        int p1 = Utils::RandomInt(0, particles.size() - 1);
        int p2 = Utils::RandomInt(0, particles.size() - 1);
        if (p1 != p2 && Utils::RandomFloat(0, 1) < ENTANGLEMENT_PROBABILITY) {
            particles[p1].is_entangled = true;
            particles[p1].entangled_partner = p2;
            particles[p2].is_entangled = true;
            particles[p2].entangled_partner = p1;
        }
    }
    
    for (size_t i = 0; i < particles.size(); i++) {
        auto& p = particles[i];
        
        Vector2 center = {SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f};
        Vector2 to_center = Vector2Subtract(center, p.position);
        float dist = Vector2Length(to_center);
        float force = FORCE_SCALE;
        
        p.acceleration = Vector2Scale(to_center, force / (dist + 1.0f));
        p.velocity = Vector2Add(p.velocity, Vector2Scale(p.acceleration, dt));
        p.velocity = Vector2Scale(p.velocity, 0.95f);
        p.position = Vector2Add(p.position, Vector2Scale(p.velocity, dt));
        
        if (p.is_entangled && p.entangled_partner >= 0 && p.entangled_partner < (int)particles.size()) {
            p.color = {255, 200, 100, 255};
            p.radius = 6.0f;
            auto& partner = particles[p.entangled_partner];
            p.spin = partner.spin;
        } else {
            p.color = {100, 150, 255, 255};
            p.radius = 4.0f;
        }
    }
}

// wave particle duality mode switches particles between smooth wave paths and more jumpy particles

void UpdateWaveParticleDuality(std::vector<QuantumParticle>& particles, float dt) {
    for (auto& p : particles) {
        p.wave_time += dt;
        
        if ((int)(p.wave_time * 3) % 200 < 100) {
            p.movement_mode = 1;
        } else {
            p.movement_mode = 0;
        }
        
        if (p.movement_mode == 1) {
            p.position.x += 100.0f * dt;
            p.position.y += p.wave_amplitude * sin(p.position.x * p.wave_frequency);
            p.color = {100, 255, 150, 255};
            p.radius = 3.0f + 2.0f * fabs(sin(p.wave_time * 4));
        } else {
            if (Utils::RandomFloat(0, 1) < 0.1f) {
                p.velocity.x = (Utils::RandomFloat(0, 1) - 0.5f) * 200;
                p.velocity.y = (Utils::RandomFloat(0, 1) - 0.5f) * 200;
            }
            
            p.position = Vector2Add(p.position, Vector2Scale(p.velocity, dt));
            p.color = {255, 100, 100, 255};
            p.radius = 5.0f;
        }
        
        p.WrapPosition(SCREEN_WIDTH, SCREEN_HEIGHT);
    }
}

// measurement mode slows and highlights particles that pass through a region

void UpdateMeasurement(std::vector<QuantumParticle>& particles, const Rectangle& measurement_zone, float dt) {
    for (auto& p : particles) {
        Vector2 zone_center = {measurement_zone.x + measurement_zone.width / 2,
                               measurement_zone.y + measurement_zone.height / 2};
        float dist_to_zone = Vector2Distance(p.position, zone_center);
        bool in_zone = dist_to_zone < sqrt(measurement_zone.width * measurement_zone.width +
                                           measurement_zone.height * measurement_zone.height) / 2;
        
        if (in_zone && !p.measured) {
            p.measured = true;
            p.velocity = Vector2Zero();
            p.spin = (int)(Utils::RandomFloat(0, 1) * 3) - 1;
            p.color = {255, 100, 100, 255};
            p.radius = 7.0f;
        } else if (!in_zone) {
            p.measured = false;
            p.color = {100, 200, 255, 255};
            p.radius = 3.0f;
            
            p.wave_time += dt;
            p.position.x += 80.0f * dt;
            Vector2 zone_direction = Vector2Subtract(zone_center, p.position);
            float zone_dist = Vector2Length(zone_direction);
            
            if (zone_dist < 300) {
                zone_direction = Vector2Normalize(zone_direction);
                p.position = Vector2Add(p.position, Vector2Scale(zone_direction, 20.0f * dt));
            }
        }
        
        p.WrapPosition(SCREEN_WIDTH, SCREEN_HEIGHT);
    }
}

// builds a probability heatmap from the current particle cloud (aka elecron cloud, and i qoute "SCHRODINGERRR")

void CalculateProbabilityField(const std::vector<QuantumParticle>& particles,
                        std::vector<float>& field,
                        int width, int height, int cell_size) {
    field.clear();
    int cols = width / cell_size;
    int rows = height / cell_size;
    field.resize(cols * rows, 0.0f);
    
    for (int y = 0; y < rows; y++) {
        for (int x = 0; x < cols; x++) {
            float prob_sum = 0;
            int cell_x = x * cell_size;
            int cell_y = y * cell_size;
            
            for (const auto& p : particles) {
                float dist = sqrt(pow(p.position.x - cell_x, 2) + pow(p.position.y - cell_y, 2));
                prob_sum += p.probability / (1 + dist / 50);
            }
            
            field[y * cols + x] = std::min(1.0f, prob_sum);
        }
    }
}

} 
