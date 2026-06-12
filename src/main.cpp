#include <raylib.h>
#include "ui.h"
#include "ui_draw.h"
#include "Library.h"
#include "Playlist.h"

int main(void) {
    // ── Window ───────────────────────────────────────────────────────────────
    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_WINDOW_HIGHDPI);
    InitWindow(SW, SH, "Aura Music");
    SetTargetFPS(60);
    InitAudioDevice();

    // ── Data ─────────────────────────────────────────────────────────────────
    loadLibrary("assets/library.txt");
    for (int i = 0; i < totalSongs; i++) {
        insertIntoPlaylist(librarySongs[i]);
    }

    // Auto-load the first song on startup
    if (currentSong) loadAndPlayCurrent();

    // ── Main Loop ─────────────────────────────────────────────────────────────
    while (!WindowShouldClose()) {

        // Update audio stream every frame
        if (musicLoaded) {
            UpdateMusicStream(musicStream);

            // Auto advance when track ends
            if (isPlaying && GetMusicTimePlayed(musicStream) >= GetMusicTimeLength(musicStream) - 0.1f) {
                nextSong();
                loadAndPlayCurrent();
            }
        }

        // ── Keyboard shortcuts ────────────────────────────────────────────────
        if (IsKeyPressed(KEY_RIGHT)) { nextSong(); loadAndPlayCurrent(); }
        if (IsKeyPressed(KEY_LEFT))  { prevSong(); loadAndPlayCurrent(); }
        if (IsKeyPressed(KEY_SPACE)) { togglePause(); }

        // ── Draw ─────────────────────────────────────────────────────────────
        BeginDrawing();
            // Background gradient
            DrawRectangleGradientEx({0, 0, (float)SW, (float)SH}, BG_TOP, BG_BOT, BG_BOT, BG_TOP);

            DrawSidePanel();
            DrawPlaylistPanel();
            DrawRightPanel();
            DrawPlayerBar();

            // FPS counter (dev aid, remove for submission)
            DrawText(TextFormat("FPS: %d", GetFPS()), SW - 70, 6, 12, TXT_DIM);
        EndDrawing();
    }

    // ── Cleanup ───────────────────────────────────────────────────────────────
    if (albumArt.id > 0) UnloadTexture(albumArt);
    if (musicLoaded) { StopMusicStream(musicStream); UnloadMusicStream(musicStream); }
    CloseAudioDevice();
    CloseWindow();
    return 0;
}
