#include "visualization.h"

static const Color CYAN_COLOR = {0, 255, 255, 255};
static const Color LIGHTBLUE_COLOR = {173, 216, 230, 255};
static const Color DARKBLUE_COLOR = {15, 15, 35, 255};
static const Color ACCENT_COLOR = {100, 200, 255, 255};

namespace Visualization {

std::string GetModeName(int mode) {
    const char* mode_names[] = {
        "SUPERPOSITION",
        "UNCERTAINTY",
        "ENTANGLEMENT",
        "WAVE-PARTICLE DUALITY",
        "MEASUREMENT"
    };
    if (mode >= 0 && mode < MAX_MODES) return mode_names[mode];
    return "UNKNOWN";
}

std::string GetModeDescription(int mode) {
    const char* mode_descs[] = {
        "Particles exist in multiple states simultaneously",
        "Higher momentum = lower position certainty",
        "Entangled particles affect each other instantly",
        "Toggle between wave-like and particle-like behavior",
        "Observation collapses the wave function"
    };
    if (mode >= 0 && mode < MAX_MODES) return mode_descs[mode];
    return "Unknown mode description";
}

Color GetModeColor(int mode) {
    Color mode_colors[] = {
        {100, 200, 255, 255},
        {255, 150, 100, 255},
        {255, 100, 200, 255},
        {100, 255, 150, 255},
        {255, 255, 100, 255}
    };
    if (mode >= 0 && mode < MAX_MODES) return mode_colors[mode];
    return WHITE;
}

Color GetParticleColor(int mode) {
    return GetModeColor(mode);
}

// drawing gradients, borders and panels

void DrawGradientBackground(int x, int y, int width, int height, Color top, Color bottom) {
    for (int i = 0; i < height; i++) {
        float lerp_val = (float)i / height;
        Color new_color = {
            (unsigned char)(top.r * (1.0f - lerp_val) + bottom.r * lerp_val),
            (unsigned char)(top.g * (1.0f - lerp_val) + bottom.g * lerp_val),
            (unsigned char)(top.b * (1.0f - lerp_val) + bottom.b * lerp_val),
            (unsigned char)(top.a * (1.0f - lerp_val) + bottom.a * lerp_val)
        };
        DrawRectangle(x, y + i, width, 1, new_color);
    }
}

void DrawBorder(int x, int y, int width, int height, Color color, int thickness) {
    DrawRectangle(x, y, width, thickness, color);
    DrawRectangle(x, y + height - thickness, width, thickness, color);
    DrawRectangle(x, y, thickness, height, color);
    DrawRectangle(x + width - thickness, y, thickness, height, color);
}

void DrawDoubleBorder(int x, int y, int width, int height, Color outer, Color inner) {
    DrawBorder(x, y, width, height, outer, 2);
    DrawBorder(x + 3, y + 3, width - 6, height - 6, inner, 1);
}

void DrawPanel(int x, int y, int width, int height, const char* title, Color titleColor) {
    DrawGradientBackground(x, y, width, height, {35, 35, 60, 255}, {20, 20, 40, 255});
    
    DrawDoubleBorder(x, y, width, height, titleColor, {100, 100, 120, 150});
    
    DrawGradientBackground(x + 1, y + 1, width - 2, 26, titleColor, {30, 30, 50, 255});
    
    DrawText(title, x + 13, y + 8, 14, {0, 0, 0, 100});
    DrawText(title, x + 12, y + 7, 14, titleColor);
}

// right hand panel that shows and highlights the current mode

void DrawModeSelector(int current_mode) {
    int panel_x = SCREEN_WIDTH - 260;
    int panel_y = 70;
    int panel_width = 240;
    int panel_height = 200;
    
    DrawPanel(panel_x, panel_y, panel_width, panel_height, "QUANTUM MODES", LIGHTBLUE_COLOR);
    
    int button_height = 28;
    int spacing = 6;
    int start_y = panel_y + 35;
    
    for (int i = 0; i < MAX_MODES; i++) {
        int btn_y = start_y + (i * (button_height + spacing));
        Color mode_color = GetModeColor(i);
        bool is_current = (i == current_mode);
        
        if (is_current) {
            DrawGradientBackground(panel_x + 12, btn_y, panel_width - 24, button_height, mode_color, {20, 20, 40, 255});
        } else {
            DrawRectangle(panel_x + 12, btn_y, panel_width - 24, button_height, {40, 40, 70, 200});
        }
        
        if (is_current) {
            DrawBorder(panel_x + 12, btn_y, panel_width - 24, button_height, mode_color, 2);
        } else {
            DrawBorder(panel_x + 12, btn_y, panel_width - 24, button_height, {70, 70, 90, 100}, 1);
        }
        
        Color text_color_inactive = {150, 150, 170, 255};
        Color text_color = is_current ? WHITE : text_color_inactive;
        DrawText(TextFormat("[%d]", i + 1), panel_x + 18, btn_y + 5, 12, text_color);
        
        const char* short_names[] = {"SUPER", "UNCERT", "ENTANG", "WAVE", "MEASUR"};
        DrawText(short_names[i], panel_x + 55, btn_y + 6, 11, text_color);
        
        if (is_current) {
            DrawText("<<", panel_x + 195, btn_y + 5, 13, mode_color);
        }
    }
}

// left hand panel with pause state fps and a few stats

void DrawStatusPanel(const QuantumSimulator& sim) {
    int panel_x = 20;
    int panel_y = 70;
    int panel_width = 240;
    int panel_height = 200;
    
    DrawPanel(panel_x, panel_y, panel_width, panel_height, "SIMULATION STATUS", CYAN_COLOR);
    
    int line_height = 28;
    int start_y = panel_y + 40;
    
    bool paused = sim.IsPaused();
    Color status_color = paused ? RED : GREEN;
    
    DrawRectangle(panel_x + 15, start_y, panel_width - 30, 24, {40, 40, 70, 200});
    DrawBorder(panel_x + 15, start_y, panel_width - 30, 24, status_color, 2);
    
    if (paused) {
        DrawText("||  PAUSED", panel_x + 25, start_y + 3, 13, RED);
    } else {
        DrawText(">  RUNNING", panel_x + 25, start_y + 3, 13, GREEN);
    }
    

    int stat_y = start_y + line_height;
    DrawText("Particles:", panel_x + 20, stat_y, 11, {200, 200, 200, 255});
    DrawText(TextFormat("%d", (int)sim.GetParticles().size()), 
             panel_x + 140, stat_y, 13, LIGHTBLUE_COLOR);
    
    DrawRectangle(panel_x + 15, stat_y + 17, panel_width - 30, 2, {100, 100, 150, 100});
    
    stat_y += line_height;
    DrawText("FPS:", panel_x + 20, stat_y, 11, {200, 200, 200, 255});
    int fps = GetFPS();
    Color fps_color = fps >= 55 ? GREEN : (fps >= 30 ? YELLOW : RED);
    DrawText(TextFormat("%d", fps), 
             panel_x + 140, stat_y, 13, fps_color);
    
    DrawRectangle(panel_x + 15, stat_y + 17, panel_width - 30, 2, {100, 100, 150, 100});
    
    stat_y += line_height;
    DrawText("Frame:", panel_x + 20, stat_y, 11, {200, 200, 200, 255});
    DrawText(TextFormat("%.1f ms", GetFrameTime() * 1000), 
             panel_x + 140, stat_y, 13, LIGHTBLUE_COLOR);
}

// keyboard shortcut cheat sheet

void DrawInstructionsPanel() {
    int panel_x = 20;
    int panel_y = 280;
    int panel_width = 240;
    int panel_height = 190;
    
    DrawPanel(panel_x, panel_y, panel_width, panel_height, "KEYBOARD CONTROLS", ACCENT_COLOR);
    
    int line_height = 22;
    int start_y = panel_y + 40;
    
    struct Control { const char* key; const char* action; };
    Control controls[] = {
        {"1-5", "Mode Select"},
        {"SPACE", "Pause/Play"},
        {"  R", "Reset Sim"},
        {"  P", "Show Field"},
        {"ESC", "Exit App"}
    };
    
    for (int i = 0; i < 5; i++) {
        int y = start_y + (i * line_height);
        
        DrawRectangle(panel_x + 15, y, 40, 18, {50, 50, 80, 200});
        DrawBorder(panel_x + 15, y, 40, 18, ACCENT_COLOR, 1);
        DrawText(controls[i].key, panel_x + 20, y + 2, 10, ACCENT_COLOR);
        
        DrawText(controls[i].action, panel_x + 65, y + 3, 11, {200, 200, 200, 255});
    }
}

// bottom bar that explains what the active mode represents

void DrawModeDescriptionPanel(int mode) {
    int panel_x = 270;
    int panel_y = SCREEN_HEIGHT - 150;
    int panel_width = SCREEN_WIDTH - 300;
    int panel_height = 130;
    
    Color mode_color = GetModeColor(mode);

    std::string mode_name = GetModeName(mode);
    DrawPanel(panel_x, panel_y, panel_width, panel_height, mode_name.c_str(), mode_color);
    
    DrawRectangle(panel_x + 15, panel_y + 33, panel_width - 30, 2, mode_color);
    
    std::string desc = GetModeDescription(mode);
    DrawText(desc.c_str(), panel_x + 20, panel_y + 45, 11, {220, 220, 220, 255});
    
    DrawText(TextFormat("[Mode %d of %d]", mode + 1, MAX_MODES), 
             panel_x + panel_width - 140, panel_y + panel_height - 20, 9, mode_color);
}

// main ui rendering function

static void DrawHelpOverlay();

void DrawUI(const QuantumSimulator& sim) {
    DrawGradientBackground(0, 0, SCREEN_WIDTH, 50, {50, 100, 150, 255}, {20, 40, 80, 255});
    DrawRectangle(0, 48, SCREEN_WIDTH, 2, ACCENT_COLOR);
    
    int title_width = MeasureText("QUANTUM PARTICLE SIMULATOR", 26);
    DrawText("QUANTUM PARTICLE SIMULATOR", (SCREEN_WIDTH - title_width) / 2, 12, 26, WHITE);
    
    DrawStatusPanel(sim);
    DrawInstructionsPanel();
    DrawModeSelector(sim.GetMode());
    DrawModeDescriptionPanel(sim.GetMode());
    
    DrawRectangle(0, SCREEN_HEIGHT - 2, SCREEN_WIDTH, 2, {100, 150, 200, 200});
    DrawText("Press 1-5 to switch modes | SPACE to pause | H for help | ESC to exit", 
             20, SCREEN_HEIGHT - 22, 10, {150, 150, 170, 200});

    if (sim.ShowsHelp()) {
        DrawHelpOverlay();
    }
}

// full screen overlay toggled with h

static void DrawHelpOverlay()
{
    const int overlayWidth  = SCREEN_WIDTH - 200;
    const int overlayHeight = 220;
    const int overlayX = (SCREEN_WIDTH  - overlayWidth) / 2;
    const int overlayY = (SCREEN_HEIGHT - overlayHeight) / 2;

    DrawRectangle(overlayX, overlayY, overlayWidth, overlayHeight, {10, 10, 30, 230});
    DrawDoubleBorder(overlayX, overlayY, overlayWidth, overlayHeight, ACCENT_COLOR, {120, 120, 180, 180});

    DrawTextCentered("HELP / QUICK REFERENCE", SCREEN_WIDTH / 2, overlayY + 16, 18, ACCENT_COLOR);

    int textY = overlayY + 50;
    const int line = 18;

    DrawText("1-5 : Switch between quantum modes", overlayX + 30, textY, 14, LIGHTGRAY); textY += line;
    DrawText("SPACE : Pause / resume simulation",  overlayX + 30, textY, 14, LIGHTGRAY); textY += line;
    DrawText("R : Reset particles",               overlayX + 30, textY, 14, LIGHTGRAY); textY += line;
    DrawText("P : Toggle probability field (superposition mode)", overlayX + 30, textY, 14, LIGHTGRAY); textY += line;
    DrawText("Mouse : Move measurement zone in MEASUREMENT mode", overlayX + 30, textY, 14, LIGHTGRAY); textY += line;

    textY += line;
    DrawText("H : Toggle this help overlay", overlayX + 30, textY, 14, ACCENT_COLOR); textY += line * 2;

    DrawText("Each mode visualizes a different quantum idea:", overlayX + 30, textY, 14, LIGHTGRAY); textY += line;
    DrawText("- Superposition, Uncertainty, Entanglement, Wave/Particle, Measurement", overlayX + 40, textY, 14, LIGHTGRAY);
}

void DrawTextCentered(const char* text, int x, int y, int font_size, Color color) {
    int text_width = MeasureText(text, font_size);
    DrawText(text, x - text_width / 2, y, font_size, color);
}

void DrawStatistics(size_t particle_count) {
    DrawText(TextFormat("Particles: %llu", particle_count), 20, SCREEN_HEIGHT - 30, 12, LIGHTGRAY);
}

}
