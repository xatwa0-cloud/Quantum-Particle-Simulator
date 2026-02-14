#include "particle.h"
#include "config.h"

// basic constructor that wires the particle state

QuantumParticle::QuantumParticle(Vector2 pos, float prob)
    : position(pos), 
      probability(prob), 
      superposition_phase(0.0f),
      spin(0.0f), 
      radius(PARTICLE_BASE_RADIUS), 
      color(BLUE), 
      measured(false),
      is_entangled(false), 
      entangled_partner(-1), 
      movement_mode(0),
      wave_amplitude(DEFAULT_WAVE_AMPLITUDE), 
      wave_frequency(DEFAULT_WAVE_FREQUENCY), 
      wave_time(0.0f) {
    velocity = {0.0f, 0.0f};
    acceleration = {0.0f, 0.0f};
}

// simple integration and rendering for a single particle

void QuantumParticle::Update(float dt) {
    velocity = Vector2Add(velocity, Vector2Scale(acceleration, dt));
    position = Vector2Add(position, Vector2Scale(velocity, dt));
    acceleration = {0.0f, 0.0f};
}

void QuantumParticle::Render() const {
    DrawCircle((int)position.x, (int)position.y, radius, color);
}

void QuantumParticle::WrapPosition(int screen_width, int screen_height) {
    if (position.x < 0) position.x = screen_width;
    if (position.x > screen_width) position.x = 0;
    if (position.y < 0) position.y = screen_height;
    if (position.y > screen_height) position.y = 0;
}

