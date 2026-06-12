#include <raylib.h>

Font fontBold = {0};
Font fontReg  = {0};

#include "ui.h"
#include "ui_draw.h"
#include "Library.h"
#include "Playlist.h"
#include "History.h"
#include "MostPlayed.h"

int main(void) {
    // ── Window setup: borderless fullscreen ───────────────────────────────────
    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_WINDOW_HIGHDPI | FLAG_FULLSCREEN_MODE);
    InitWindow(GetMonitorWidth(0), GetMonitorHeight(0), "SpotiFYE");
    SetTargetFPS(60);
    InitAudioDevice();

    ChangeDirectory(GetApplicationDirectory());
    if (!DirectoryExists("assets")) {
        ChangeDirectory("../");
    }

    fontBold = LoadFontEx("assets/Poppins-Bold.ttf", 64, 0, 0);
    SetTextureFilter(fontBold.texture, TEXTURE_FILTER_BILINEAR);
    fontReg = LoadFontEx("assets/Poppins-Regular.ttf", 64, 0, 0);
    SetTextureFilter(fontReg.texture, TEXTURE_FILTER_BILINEAR);

    // ── Load data ─────────────────────────────────────────────────────────────
    loadLibrary("assets/library.txt");
    for (int i = 0; i < totalSongs; i++) {
        insertIntoPlaylist(librarySongs[i]);
        heapInsert(librarySongs[i]);
    }

    // Auto-play first song
    if (currentSong) loadAndPlay();

    // ── Main loop ─────────────────────────────────────────────────────────────
    while (!WindowShouldClose()) {

        // Recompute layout every frame (fullscreen-aware)
        UpdateLayout();
        UpdateAnimations();
        UpdateRipples();

        // Update audio stream
        if (musicLoaded) {
            UpdateMusicStream(musicStream);

            // Auto-advance: only if there's a next song to go to
            float played = GetMusicTimePlayed(musicStream);
            float len    = GetMusicTimeLength(musicStream);
            if (isPlaying && len > 0.0f && played >= len - 0.15f) {
                Node* was = currentSong;
                nextSong();
                if (currentSong != was) {
                    loadAndPlay();
                } else {
                    // End of playlist, no repeat — just stop
                    StopMusicStream(musicStream);
                    isPlaying = false;
                }
            }
        }

        // ── Keyboard shortcuts ────────────────────────────────────────────────
        if (IsKeyPressed(KEY_RIGHT)) {
            Node* was = currentSong;
            nextSong();
            if (currentSong != was || repeatAll) loadAndPlay();
        }
        if (IsKeyPressed(KEY_LEFT))  { prevSong(); loadAndPlay(); }
        if (IsKeyPressed(KEY_SPACE)) togglePause();
        if (IsKeyPressed(KEY_R))     toggleRepeat();
        if (IsKeyPressed(KEY_F11))   ToggleFullscreen();

        // ── Draw ──────────────────────────────────────────────────────────────
        BeginDrawing();
            ClearBackground(C_BG);
            DrawSidePanel();
            DrawPlaylistPanel();
            DrawRightPanel();
            DrawPlayerBar();
            DrawRipples();
        EndDrawing();
    }

    // ── Cleanup ───────────────────────────────────────────────────────────────
    if (fontBold.texture.id > 0) UnloadFont(fontBold);
    if (fontReg.texture.id > 0)  UnloadFont(fontReg);
    if (albumArt.id > 0)  UnloadTexture(albumArt);
    if (musicLoaded) { StopMusicStream(musicStream); UnloadMusicStream(musicStream); }
    CloseAudioDevice();
    CloseWindow();
    return 0;
}
