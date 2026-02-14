#ifndef QUANTUM_VISUALIZATION_H
#define QUANTUM_VISUALIZATION_H

#include "config.h"
#include "simulator.h"
#include "raylib.h"
#include <string>

// ============================================================================
// Visualization & UI Rendering
// ============================================================================

namespace Visualization {
    
    // Panel Drawing
    void DrawPanel(int x, int y, int width, int height, const char* title, Color titleColor);
    void DrawBorder(int x, int y, int width, int height, Color color, int thickness);
    
    // UI Panels
    void DrawStatusPanel(const QuantumSimulator& sim);
    void DrawInstructionsPanel();
    void DrawModeDescriptionPanel(int mode);
    void DrawModeSelector(int current_mode);
    void DrawUI(const QuantumSimulator& sim);
    
    // Color Management
    Color GetModeColor(int mode);
    Color GetParticleColor(int mode);
    std::string GetModeName(int mode);
    std::string GetModeDescription(int mode);
    
    // Debug/Info
    void DrawStatistics(size_t particle_count);
    
    // Font Rendering Helper
    void DrawTextCentered(const char* text, int x, int y, int font_size, Color color);
}

#endif // QUANTUM_VISUALIZATION_H
