#include "raylib.h"
#include "simulator.h"
#include "config.h"

// raylib main loop handing everything to the simulator

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Quantum Particle Simulator");
    SetTargetFPS((int)FPS);
    
    QuantumSimulator simulator;
    
    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_ONE)) simulator.SetMode(MODE_SUPERPOSITION);
        if (IsKeyPressed(KEY_TWO)) simulator.SetMode(MODE_UNCERTAINTY);
        if (IsKeyPressed(KEY_THREE)) simulator.SetMode(MODE_ENTANGLEMENT);
        if (IsKeyPressed(KEY_FOUR)) simulator.SetMode(MODE_WAVE_PARTICLE);
        if (IsKeyPressed(KEY_FIVE)) simulator.SetMode(MODE_MEASUREMENT);
        
        if (IsKeyPressed(KEY_SPACE)) simulator.TogglePause();
        if (IsKeyPressed(KEY_R)) simulator.Reset();
        if (IsKeyPressed(KEY_P)) simulator.ToggleProbabilityField();
        if (IsKeyPressed(KEY_H)) simulator.ToggleHelp();
        
        if (simulator.GetMode() == MODE_MEASUREMENT) {
            simulator.UpdateMeasurementZone(GetMousePosition());
        }
        
        simulator.Update(GetFrameTime());
        
        BeginDrawing();
        ClearBackground(Color{10, 10, 25, 255});
        
        simulator.Render();
        
        EndDrawing();
    }
    
    CloseWindow();
    return 0;
}
