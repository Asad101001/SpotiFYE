#include <raylib.h>
#include "ui.h"
#include "ui_draw.h"
#include "Library.h"
#include "Playlist.h"
#include "History.h"
#include "MostPlayed.h"

int main(void) {
    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_WINDOW_HIGHDPI);
    InitWindow(SW, SH, "SpotiFYE");
    SetTargetFPS(60);
    InitAudioDevice();

    // ── Load library ─────────────────────────────────────────────────────────
    loadLibrary("assets/library.txt");

    // ── Build playlist (doubly LL) and max-heap simultaneously ───────────────
    for (int i = 0; i < totalSongs; i++) {
        insertIntoPlaylist(librarySongs[i]);
        heapInsert(librarySongs[i]);
    }

    // Auto-play first song
    if (currentSong) {
        historyPush(currentSong->song);
        loadAndPlay();
    }

    // ── Main loop ─────────────────────────────────────────────────────────────
    while (!WindowShouldClose()) {

        // Update audio stream
        if (musicLoaded) {
            UpdateMusicStream(musicStream);
            // Auto-advance when track ends
            float played = GetMusicTimePlayed(musicStream);
            float len    = GetMusicTimeLength(musicStream);
            if (isPlaying && len > 0.0f && played >= len - 0.15f) {
                historyPush(currentSong->song);
                nextSong();
                loadAndPlay();
            }
        }

        // Animate glow/pulse
        UpdateAnimations();

        // ── Keyboard ──────────────────────────────────────────────────────────
        if (IsKeyPressed(KEY_RIGHT)) { historyPush(currentSong ? currentSong->song : nullptr); nextSong(); loadAndPlay(); }
        if (IsKeyPressed(KEY_LEFT))  { prevSong(); loadAndPlay(); }
        if (IsKeyPressed(KEY_SPACE)) togglePause();
        if (IsKeyPressed(KEY_R))     toggleRepeat();

        // ── Draw ──────────────────────────────────────────────────────────────
        BeginDrawing();
            ClearBackground(C_BG);
            DrawSidePanel();
            DrawPlaylistPanel();
            DrawRightPanel();
            DrawPlayerBar();
        EndDrawing();
    }

    // ── Cleanup ───────────────────────────────────────────────────────────────
    if (albumArt.id > 0)  UnloadTexture(albumArt);
    if (musicLoaded) { StopMusicStream(musicStream); UnloadMusicStream(musicStream); }
    CloseAudioDevice();
    CloseWindow();
    return 0;
}
