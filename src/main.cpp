#include <raylib.h>
#include <iostream>
#include "Library.h"
#include "Playlist.h"

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

    // Initialize Audio Device
    InitAudioDevice();

    // Load Data Structures procedurally
    loadLibrary("assets/library.txt");

    // Push all loaded songs into the doubly linked list playlist
    for (int i = 0; i < totalSongs; i++) {
        insertIntoPlaylist(librarySongs[i]);
    }

    // Texture state for dynamic Album Art
    Texture2D currentAlbumArt = { 0 };
    Node* lastTrackedSong = nullptr;

    // Main rendering loop
    while (!WindowShouldClose())
    {
        // Check if the song has changed, update texture
        if (currentSong != lastTrackedSong) {
            if (currentAlbumArt.id > 0) {
                UnloadTexture(currentAlbumArt); // Free old image from VRAM
                currentAlbumArt.id = 0;
            }
            if (currentSong != nullptr && !currentSong->song->coverPath.empty()) {
                currentAlbumArt = LoadTexture(currentSong->song->coverPath.c_str());
            }
            lastTrackedSong = currentSong;
        }

        // Update logic (Keyboard Input for Playlist Navigation)
        if (IsKeyPressed(KEY_RIGHT)) {
            nextSong();
        }
        if (IsKeyPressed(KEY_LEFT)) {
            prevSong();
        }
        
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
            
            Node* cur = playlistHead;
            int i = 0;
            while (cur != nullptr) {
                // If it's the currently playing song, highlight it strongly
                Color panelColor = (cur == currentSong) ? glassPanelHover : glassPanel;
                Color textColor = (cur == currentSong) ? accentNeon : textPrimary;

                DrawRectangleRounded((Rectangle){mainContent.x + 20, mainContent.y + 70 + (float)(i * 60), mainContent.width - 40, 50}, 0.2f, 8, panelColor);
                DrawText(TextFormat("%d. %s - %s", i + 1, cur->song->title.c_str(), cur->song->artist.c_str()), mainContent.x + 35, mainContent.y + 85 + (i * 60), 16, textColor);
                
                int mins = cur->song->duration / 60;
                int secs = cur->song->duration % 60;
                DrawText(TextFormat("%02d:%02d", mins, secs), mainContent.x + mainContent.width - 70, mainContent.y + 85 + (i * 60), 16, textSecondary);
                
                cur = cur->next;
                i++;
            }

            // 4. Right Panel: Algorithms, Graph & Stats
            DrawGlassPanel(rightPanel, "Up Next / History");
            DrawText("Queue (Stack/Queue logic)", rightPanel.x + 20, rightPanel.y + 70, 16, textSecondary);
            DrawRectangleRounded((Rectangle){rightPanel.x + 20, rightPanel.y + 100, rightPanel.width - 40, 50}, 0.2f, 8, glassPanelHover);
            DrawText("Track 9", rightPanel.x + 35, rightPanel.y + 115, 16, textPrimary);
            
            DrawText("Similarity Graph (BFS/DFS)", rightPanel.x + 20, rightPanel.y + 300, 16, accentNeon);

            // 5. Bottom Panel: Media Player Controls
            DrawGlassPanel(bottomPlayer, nullptr);
            
            // Album Art Box
            if (currentAlbumArt.id > 0) {
                // Scale texture to exactly 80x80
                Rectangle sourceRec = { 0.0f, 0.0f, (float)currentAlbumArt.width, (float)currentAlbumArt.height };
                Rectangle destRec = { bottomPlayer.x + 20, bottomPlayer.y + 20, 80.0f, 80.0f };
                Vector2 origin = { 0.0f, 0.0f };
                DrawTexturePro(currentAlbumArt, sourceRec, destRec, origin, 0.0f, WHITE);
            } else {
                // Fallback Placeholder
                DrawRectangleRounded((Rectangle){bottomPlayer.x + 20, bottomPlayer.y + 20, 80, 80}, 0.1f, 8, DARKGRAY);
            }
            
            if (currentSong != nullptr) {
                DrawText("Now Playing", bottomPlayer.x + 120, bottomPlayer.y + 30, 20, textPrimary);
                DrawText(TextFormat("%s - %s", currentSong->song->title.c_str(), currentSong->song->artist.c_str()), bottomPlayer.x + 120, bottomPlayer.y + 60, 16, textSecondary);
                DrawText(TextFormat("Genre: %s | Rating: %.1f", currentSong->song->genre.c_str(), currentSong->song->rating), bottomPlayer.x + 120, bottomPlayer.y + 80, 14, accentNeon);
            } else {
                DrawText("No Song Playing", bottomPlayer.x + 120, bottomPlayer.y + 45, 20, textSecondary);
            }
            
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

    if (currentAlbumArt.id > 0) {
        UnloadTexture(currentAlbumArt);
    }

    // De-Initialization
    CloseAudioDevice();
    CloseWindow();
    return 0;
}
