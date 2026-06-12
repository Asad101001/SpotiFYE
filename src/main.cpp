#include <raylib.h>

// Define Premium Colors for "Liquid Audio" Theme
const Color bgTopLeft = { 18, 15, 30, 255 };
const Color bgBottomRight = { 10, 35, 55, 255 };
const Color glassPanel = { 255, 255, 255, 12 };  // Highly transparent white
const Color glassPanelHover = { 255, 255, 255, 25 }; 
const Color glassBorder = { 255, 255, 255, 30 }; // Subtle border
const Color textPrimary = WHITE;
const Color textSecondary = { 180, 180, 200, 255 };
const Color accentNeon = { 200, 50, 255, 255 };

// UI Compartment Layout Settings
const int screenWidth = 1200;
const int screenHeight = 800;
const int padding = 20;

// Panel bounds
Rectangle sideMenu = { padding, padding, 250, screenHeight - 140 - padding * 2 };
Rectangle mainContent = { sideMenu.x + sideMenu.width + padding, padding, 550, sideMenu.height };
Rectangle rightPanel = { mainContent.x + mainContent.width + padding, padding, screenWidth - mainContent.x - mainContent.width - padding * 2, sideMenu.height };
Rectangle bottomPlayer = { padding, screenHeight - 140, screenWidth - padding * 2, 120 };

// Helper function to draw frosted glass panels
void DrawGlassPanel(Rectangle rect, const char* title) {
    DrawRectangleRounded(rect, 0.05f, 16, glassPanel);
    DrawRectangleRoundedLines(rect, 0.05f, 16, 1, glassBorder);
    if (title != nullptr) {
        DrawText(title, rect.x + 20, rect.y + 20, 20, textPrimary);
        DrawLine(rect.x + 20, rect.y + 50, rect.x + rect.width - 20, rect.y + 50, glassBorder);
    }
}

int main(void)
{
    // Initialization
    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_WINDOW_HIGHDPI);
    InitWindow(screenWidth, screenHeight, "Aura Music - Premium DSA Project");
    SetTargetFPS(60);

    // Main rendering loop
    while (!WindowShouldClose())
    {
        // Update logic...
        
        // Draw
        BeginDrawing();
            
            // 1. Beautiful Gradient Background
            DrawRectangleGradientEx((Rectangle){0, 0, (float)screenWidth, (float)screenHeight}, 
                                     bgTopLeft, bgBottomRight, bgTopLeft, bgBottomRight);

            // 2. Left Panel: Library & Navigation
            DrawGlassPanel(sideMenu, "Library");
            DrawText("All Songs", sideMenu.x + 20, sideMenu.y + 70, 16, textSecondary);
            DrawText("Playlists", sideMenu.x + 20, sideMenu.y + 110, 16, textSecondary);
            DrawText("Favorites", sideMenu.x + 20, sideMenu.y + 150, 16, textSecondary);

            // 3. Middle Panel: Playlist / Core View
            DrawGlassPanel(mainContent, "Current Playlist");
            // Placeholder song entries
            for (int i = 0; i < 8; i++) {
                DrawRectangleRounded((Rectangle){mainContent.x + 20, mainContent.y + 70 + (i * 60), mainContent.width - 40, 50}, 0.2f, 8, glassPanelHover);
                DrawText(TextFormat("Track %d - Synthwave Boy", i + 1), mainContent.x + 35, mainContent.y + 85 + (i * 60), 16, textPrimary);
                DrawText("03:45", mainContent.x + mainContent.width - 70, mainContent.y + 85 + (i * 60), 16, textSecondary);
            }

            // 4. Right Panel: Algorithms, Graph & Stats
            DrawGlassPanel(rightPanel, "Up Next / History");
            DrawText("Queue (Stack/Queue logic)", rightPanel.x + 20, rightPanel.y + 70, 16, textSecondary);
            DrawRectangleRounded((Rectangle){rightPanel.x + 20, rightPanel.y + 100, rightPanel.width - 40, 50}, 0.2f, 8, glassPanelHover);
            DrawText("Track 9", rightPanel.x + 35, rightPanel.y + 115, 16, textPrimary);
            
            DrawText("Similarity Graph (BFS/DFS)", rightPanel.x + 20, rightPanel.y + 300, 16, accentNeon);

            // 5. Bottom Panel: Media Player Controls
            DrawGlassPanel(bottomPlayer, nullptr);
            // Dummy Album Art Box
            DrawRectangleRounded((Rectangle){bottomPlayer.x + 20, bottomPlayer.y + 20, 80, 80}, 0.1f, 8, DARKGRAY);
            DrawText("Now Playing", bottomPlayer.x + 120, bottomPlayer.y + 30, 20, textPrimary);
            DrawText("Neon Nights - Synthwave Boy", bottomPlayer.x + 120, bottomPlayer.y + 60, 16, textSecondary);
            
            // Dummy Progress Bar
            DrawRectangleRounded((Rectangle){bottomPlayer.x + 400, bottomPlayer.y + 60, 400, 6}, 1.0f, 8, glassBorder);
            DrawRectangleRounded((Rectangle){bottomPlayer.x + 400, bottomPlayer.y + 60, 150, 6}, 1.0f, 8, accentNeon);
            DrawCircle(bottomPlayer.x + 550, bottomPlayer.y + 63, 8, WHITE);
            
            // Dummy Controls
            DrawText("|<", bottomPlayer.x + 560, bottomPlayer.y + 25, 24, textPrimary);
            DrawText("||", bottomPlayer.x + 600, bottomPlayer.y + 25, 24, textPrimary);
            DrawText(">|", bottomPlayer.x + 630, bottomPlayer.y + 25, 24, textPrimary);

        EndDrawing();
    }

    // De-Initialization
    CloseWindow();
    return 0;
}
