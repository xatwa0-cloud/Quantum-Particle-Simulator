#ifndef QUANTUM_UTILS_H
#define QUANTUM_UTILS_H

#include "raylib.h"
#include "raymath.h"
#include <random>

// ============================================================================
// Utility Functions
// ============================================================================

namespace Utils {
    
    // Random number generation
    extern std::mt19937 global_rng;
    
    float RandomFloat(float min, float max);
    float RandomAngle();
    int RandomInt(int min, int max);
    
    // Vector operations
    Vector2 RandomVector(float min_magnitude, float max_magnitude);
    Vector2 ClampVectorMagnitude(Vector2 v, float max_magnitude);
    
    // Type conversions
    Color LerpColor(Color a, Color b, float t);
    Color ValueToHeatMapColor(float value, float min_val = 0.0f, float max_val = 1.0f);
    
    // Physics helpers
    float InverseLaw(float distance, float strength = 1.0f);
    bool CheckCircleCircleCollision(Vector2 p1, float r1, Vector2 p2, float r2);
    
    // Geometry
    void WrapCoordinate(float& value, int screen_size);
    bool IsPointInRectangle(Vector2 point, Rectangle rect);
    float DistanceToRectangle(Vector2 point, Rectangle rect);
}

#endif // QUANTUM_UTILS_H
