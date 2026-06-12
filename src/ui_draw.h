#include <raylib.h>
#include "ui.h"
#include "Library.h"
#include "Playlist.h"

// ─── PLAYER STATE ─────────────────────────────────────────────────────────────
Texture2D albumArt    = { 0 };
Music     musicStream = { 0 };
bool      musicLoaded = false;
bool      isPlaying   = false;
Node*     lastNode    = nullptr;

// ─── UI STATE ─────────────────────────────────────────────────────────────────
int  playlistScroll = 0;       // top visible row index
const int ROWS_VISIBLE = 9;    // how many rows fit in the playlist panel
const int ROW_H        = 52;

// ─── AUDIO HELPERS ────────────────────────────────────────────────────────────
void loadAndPlayCurrent() {
    // Unload old
    if (albumArt.id > 0)  { UnloadTexture(albumArt); albumArt = {0}; }
    if (musicLoaded)      { StopMusicStream(musicStream); UnloadMusicStream(musicStream); musicLoaded = false; }

    if (!currentSong) return;

    Song* s = currentSong->song;
    if (!s->coverPath.empty()) albumArt = LoadTexture(s->coverPath.c_str());
    if (!s->path.empty()) {
        musicStream = LoadMusicStream(s->path.c_str());
        PlayMusicStream(musicStream);
        musicLoaded = true;
        isPlaying   = true;
    }
}

void togglePause() {
    if (!musicLoaded) return;
    if (isPlaying) { PauseMusicStream(musicStream); isPlaying = false; }
    else           { ResumeMusicStream(musicStream); isPlaying = true; }
}

// ─── SIDE PANEL ───────────────────────────────────────────────────────────────
void DrawSidePanel() {
    DrawPanel(rSide, "Library");

    const char* items[] = { "All Songs", "Playlists", "Favourites" };
    for (int i = 0; i < 3; i++) {
        Rectangle r = { rSide.x + 10, rSide.y + 54 + i * 44.0f, rSide.width - 20, 36 };
        if (DrawButton(r, items[i])) { /* future: switch view */ }
    }

    // --- Stats block at bottom of side panel ---
    float sy = rSide.y + rSide.height - 130;
    DrawLineEx({rSide.x+14, sy}, {rSide.x+rSide.width-14, sy}, 0.8f, BORDER);
    DrawText("Library Stats", (int)rSide.x+16, (int)sy+10, 14, TXT_DIM);
    DrawText(TextFormat("%d songs loaded", totalSongs), (int)rSide.x+16, (int)sy+30, 14, TXT_SEC);
    int idx = 1;
    Node* c = playlistHead;
    while (c && c != currentSong) { c = c->next; idx++; }
    DrawText(TextFormat("Track %d / %d", idx, totalSongs), (int)rSide.x+16, (int)sy+50, 14, TXT_SEC);
}

// ─── PLAYLIST PANEL ───────────────────────────────────────────────────────────
void DrawPlaylistPanel() {
    DrawPanel(rMain, "Playlist");

    // Scroll with mouse wheel if hovering
    if (CheckCollisionPointRec(GetMousePosition(), rMain)) {
        playlistScroll -= (int)GetMouseWheelMove();
    }
    int maxScroll = totalSongs - ROWS_VISIBLE;
    if (playlistScroll < 0) playlistScroll = 0;
    if (playlistScroll > maxScroll && maxScroll > 0) playlistScroll = maxScroll;

    // Walk list to the scroll offset
    Node* cur = playlistHead;
    for (int s = 0; s < playlistScroll && cur; s++) cur = cur->next;

    int row = 0;
    while (cur && row < ROWS_VISIBLE) {
        bool active = (cur == currentSong);
        float ry = rMain.y + 52 + row * ROW_H;
        Rectangle rRow = { rMain.x + 10, ry, rMain.width - 20, ROW_H - 4 };

        // Row background
        Color rowBg = active ? GLASS_SEL : GLASS;
        DrawRectangleRounded(rRow, 0.18f, 8, rowBg);
        if (active) DrawRectangleRoundedLines(rRow, 0.18f, 8, 1.2f, ACCENT);

        // Click to select & play
        if (CheckCollisionPointRec(GetMousePosition(), rRow)) {
            if (!active) DrawRectangleRounded(rRow, 0.18f, 8, GLASS_HOV);
            if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
                currentSong = cur;
                loadAndPlayCurrent();
            }
        }

        // Track number
        DrawText(TextFormat("%d", playlistScroll + row + 1),
                 (int)rRow.x + 8, (int)ry + 16, 13, TXT_DIM);

        // Title (clamp to ~260px)
        std::string displayTitle = cur->song->title;
        while (MeasureText(displayTitle.c_str(), 15) > 255 && displayTitle.size() > 3)
            displayTitle.resize(displayTitle.size() - 1);
        if (displayTitle.size() < cur->song->title.size()) displayTitle += "..";

        DrawText(displayTitle.c_str(), (int)rRow.x + 36, (int)ry + 8, 15, active ? TXT_PRI : TXT_PRI);
        DrawText(cur->song->artist.c_str(), (int)rRow.x + 36, (int)ry + 27, 12, active ? ACCENT : TXT_SEC);

        // Duration (right-aligned)
        int mins = cur->song->duration / 60;
        int secs = cur->song->duration % 60;
        const char* dur = TextFormat("%02d:%02d", mins, secs);
        int dw = MeasureText(dur, 13);
        DrawText(dur, (int)(rRow.x + rRow.width - dw - 10), (int)ry + 16, 13, TXT_DIM);

        cur = cur->next;
        row++;
    }

    // Scrollbar hint
    if (totalSongs > ROWS_VISIBLE) {
        float sbH  = rMain.height - 56;
        float barH = sbH * ROWS_VISIBLE / totalSongs;
        float barY = rMain.y + 52 + sbH * playlistScroll / totalSongs;
        DrawRectangle((int)(rMain.x + rMain.width - 6), (int)(rMain.y + 52), 3, (int)sbH, GLASS_HOV);
        DrawRectangle((int)(rMain.x + rMain.width - 6), (int)barY, 3, (int)barH, ACCENT);
    }
}

// ─── RIGHT PANEL ─────────────────────────────────────────────────────────────
void DrawRightPanel() {
    DrawPanel(rRight, "Up Next");

    // Show next 5 songs from currentSong
    Node* c = currentSong ? currentSong->next : nullptr;
    int row = 0;
    while (c && row < 5) {
        float ry = rRight.y + 54 + row * 46.0f;
        Rectangle rRow = { rRight.x + 10, ry, rRight.width - 20, 38 };
        DrawRectangleRounded(rRow, 0.2f, 8, GLASS);

        std::string t = c->song->title;
        while (MeasureText(t.c_str(), 13) > (int)rRight.width - 30 && t.size() > 3)
            t.resize(t.size() - 1);
        if (t.size() < c->song->title.size()) t += "..";

        DrawText(t.c_str(), (int)rRow.x + 10, (int)ry + 4,  13, TXT_PRI);
        DrawText(c->song->artist.c_str(), (int)rRow.x + 10, (int)ry + 20, 11, TXT_SEC);

        // Click to jump
        if (CheckCollisionPointRec(GetMousePosition(), rRow)) {
            DrawRectangleRounded(rRow, 0.2f, 8, GLASS_HOV);
            if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
                currentSong = c;
                loadAndPlayCurrent();
            }
        }
        c = c->next;
        row++;
    }
    if (row == 0) DrawText("End of playlist", (int)rRight.x+14, (int)rRight.y+60, 13, TXT_DIM);
}

// ─── PLAYER BAR ───────────────────────────────────────────────────────────────
void DrawPlayerBar() {
    DrawPanel(rPlayer, nullptr);

    // Album art
    Rectangle artDest = { rPlayer.x + 10, rPlayer.y + 10, 84, 84 };
    if (albumArt.id > 0) {
        Rectangle src = { 0, 0, (float)albumArt.width, (float)albumArt.height };
        DrawTexturePro(albumArt, src, artDest, {0,0}, 0.0f, WHITE);
    } else {
        DrawRectangleRounded(artDest, 0.12f, 8, GLASS_HOV);
        int tw = MeasureText("?", 28); DrawText("?", (int)(artDest.x+artDest.width/2-tw/2), (int)(artDest.y+28), 28, TXT_DIM);
    }

    // Song info
    if (currentSong) {
        Song* s = currentSong->song;
        // Clamp title
        std::string title = s->title;
        while (MeasureText(title.c_str(), 17) > 260 && title.size() > 3) title.resize(title.size()-1);
        if (title.size() < s->title.size()) title += "..";

        DrawText(title.c_str(),    (int)rPlayer.x+104, (int)rPlayer.y+16, 17, TXT_PRI);
        DrawText(s->artist.c_str(),(int)rPlayer.x+104, (int)rPlayer.y+38, 13, TXT_SEC);
        DrawText(TextFormat("Genre: %s", s->genre.c_str()), (int)rPlayer.x+104, (int)rPlayer.y+56, 11, TXT_DIM);
    } else {
        DrawText("No track selected", (int)rPlayer.x+104, (int)rPlayer.y+30, 16, TXT_DIM);
    }

    // ── Controls (centred) ──
    float cx   = rPlayer.x + rPlayer.width / 2.0f;
    float cy   = rPlayer.y + 36;
    float btnR = 18;

    // Prev
    if (DrawIconBtn({cx - 70, cy}, btnR, "|<")) { prevSong(); loadAndPlayCurrent(); }
    // Play/Pause
    const char* ppIcon = isPlaying ? "||" : ">";
    if (DrawIconBtn({cx, cy}, btnR + 4, ppIcon, isPlaying)) { togglePause(); }
    // Next
    if (DrawIconBtn({cx + 70, cy}, btnR, ">|")) { nextSong(); loadAndPlayCurrent(); }

    // ── Progress bar ──
    float progress = 0.0f;
    float timePlayed = 0.0f, timeLen = 1.0f;
    if (musicLoaded) {
        timePlayed = GetMusicTimePlayed(musicStream);
        timeLen    = GetMusicTimeLength(musicStream);
        if (timeLen > 0.0f) progress = timePlayed / timeLen;
    }

    float barX = rPlayer.x + rPlayer.width / 2.0f - 240;
    float barY = rPlayer.y + 72;
    float barW = 480.0f;
    float barH = 5.0f;

    // Clickable seek
    Rectangle barRect = { barX, barY - 8, barW, 20 };
    if (musicLoaded && CheckCollisionPointRec(GetMousePosition(), barRect) && IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
        float t = (GetMousePosition().x - barX) / barW;
        if (t < 0) t = 0; if (t > 1) t = 1;
        SeekMusicStream(musicStream, t * timeLen);
    }

    DrawRectangleRounded({barX, barY, barW, barH}, 1.0f, 6, GLASS_HOV);
    if (progress > 0.0f) DrawRectangleRounded({barX, barY, barW * progress, barH}, 1.0f, 6, ACCENT);
    DrawCircleV({barX + barW * progress, barY + barH/2}, 7, WHITE);

    // Time stamps
    int mp = (int)timePlayed/60, sp = (int)timePlayed%60;
    int ml = (int)timeLen/60,    sl = (int)timeLen%60;
    DrawText(TextFormat("%02d:%02d", mp, sp), (int)barX - 46, (int)barY - 2, 12, TXT_DIM);
    DrawText(TextFormat("%02d:%02d", ml, sl), (int)(barX + barW + 8), (int)barY - 2, 12, TXT_DIM);

    // Volume indicator (right side)
    DrawText("VOL", (int)(rPlayer.x + rPlayer.width - 80), (int)rPlayer.y + 18, 11, TXT_DIM);
    // Static bar for now
    DrawRectangleRounded({(float)(rPlayer.x + rPlayer.width - 80), (float)rPlayer.y + 34, 60, 4}, 1.0f, 4, GLASS_HOV);
    DrawRectangleRounded({(float)(rPlayer.x + rPlayer.width - 80), (float)rPlayer.y + 34, 48, 4}, 1.0f, 4, ACCENT_DIM);
}
