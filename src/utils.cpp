#include "utils.h"
#include <cmath>
#include <algorithm>

namespace Utils {

std::mt19937 global_rng(std::random_device{}());

// random numbers and some tiny math logic

float RandomFloat(float min, float max) {
    std::uniform_real_distribution<float> dist(min, max);
    return dist(global_rng);
}

float RandomAngle() {
    return RandomFloat(0, 2 * PI);
}

int RandomInt(int min, int max) {
    if (min > max) std::swap(min, max);
    std::uniform_int_distribution<int> dist(min, max);
    return dist(global_rng);
}

// building vectors and clamping them

Vector2 RandomVector(float min_magnitude, float max_magnitude) {
    float angle = RandomAngle();
    float magnitude = RandomFloat(min_magnitude, max_magnitude);
    return {cos(angle) * magnitude, sin(angle) * magnitude};
}

Vector2 ClampVectorMagnitude(Vector2 v, float max_magnitude) {
    float magnitude = Vector2Length(v);
    if (magnitude > max_magnitude && magnitude > 0) {
        return Vector2Scale(v, max_magnitude / magnitude);
    }
    return v;
}

// color interpolation and a simple value to color mapping

Color LerpColor(Color a, Color b, float t) {
    t = std::max(0.0f, std::min(1.0f, t));
    return {
        (unsigned char)(a.r + (b.r - a.r) * t),
        (unsigned char)(a.g + (b.g - a.g) * t),
        (unsigned char)(a.b + (b.b - a.b) * t),
        (unsigned char)(a.a + (b.a - a.a) * t)
    };
}

Color ValueToHeatMapColor(float value, float min_val, float max_val) {
    float normalized = (value - min_val) / (max_val - min_val + 0.001f);
    normalized = std::max(0.0f, std::min(1.0f, normalized));
    
    if (normalized < 0.5f) {
        return LerpColor({255, 100, 100, 255}, {255, 255, 100, 255}, normalized * 2);
    } else {
        return LerpColor({255, 255, 100, 255}, {100, 255, 150, 255}, (normalized - 0.5f) * 2);
    }
}

// inverse law and a simple circle collision check

float InverseLaw(float distance, float strength) {
    return strength / (distance + 1.0f);
}

bool CheckCircleCircleCollision(Vector2 p1, float r1, Vector2 p2, float r2) {
    float dist = Vector2Distance(p1, p2);
    return dist < (r1 + r2);
}

// two dimensional geometry 

void WrapCoordinate(float& value, int screen_size) {
    if (value < 0) value = screen_size;
    if (value > screen_size) value = 0;
}

bool IsPointInRectangle(Vector2 point, Rectangle rect) {
    return point.x >= rect.x && point.x <= rect.x + rect.width &&
           point.y >= rect.y && point.y <= rect.y + rect.height;
}

float DistanceToRectangle(Vector2 point, Rectangle rect) {
    // computes the shortest distance from a point to an axis aligned rectangle
    float dx = 0.0f;
    if (point.x < rect.x) {
        dx = rect.x - point.x;
    } else if (point.x > rect.x + rect.width) {
        dx = point.x - (rect.x + rect.width);
    }

    float dy = 0.0f;
    if (point.y < rect.y) {
        dy = rect.y - point.y;
    } else if (point.y > rect.y + rect.height) {
        dy = point.y - (rect.y + rect.height);
    }

    return std::sqrt(dx * dx + dy * dy);
}

} 

