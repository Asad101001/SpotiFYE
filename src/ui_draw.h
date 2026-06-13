#pragma once
#include <raylib.h>
#include "ui.h"
#include "Library.h"
#include "Playlist.h"
#include "History.h"
#include "MostPlayed.h"

// ─── VIEW ────────────────────────────────────────────────────────────────────
enum View { VIEW_ALL = 0, VIEW_MOST_PLAYED = 1, VIEW_HISTORY = 2 };
View currentView = VIEW_ALL;

// ─── PLAYER STATE ─────────────────────────────────────────────────────────────
Music musicStream  = {0};
bool  musicLoaded  = false;
bool  isPlaying    = false;
float currentVolume = 1.0f;
bool  isMuted      = false;

// ─── SCROLL ──────────────────────────────────────────────────────────────────
int scrollOffset = 0;
const int ROW_H  = 72;

int visibleRows() { return (int)((rMain.height - 54) / ROW_H); }

// ─── AUDIO HELPERS ───────────────────────────────────────────────────────────
// Increment session play count and rebuild heap so Most Played stays accurate
void recordPlay(Song* s) {
    if (!s) return;
    s->playCount++;
    heapRebuild();
}

void loadAndPlay() {
    if (musicLoaded) { StopMusicStream(musicStream); UnloadMusicStream(musicStream); musicLoaded = false; }
    if (!currentSong) return;

    Song* s = currentSong->song;
    recordPlay(s);
    historyPush(s);

    if (!s->path.empty()) {
        musicStream = LoadMusicStream(s->path.c_str());
        SetMusicVolume(musicStream, isMuted ? 0.0f : currentVolume);
        PlayMusicStream(musicStream);
        musicLoaded = true;
        isPlaying   = true;
    }
}

void togglePause() {
    if (!musicLoaded) return;
    if (isPlaying) { PauseMusicStream(musicStream); isPlaying = false; }
    else           { ResumeMusicStream(musicStream); isPlaying = true;  }
}

void toggleMute() {
    isMuted = !isMuted;
    if (musicLoaded) SetMusicVolume(musicStream, isMuted ? 0.0f : currentVolume);
}

// ─── HELPER: draw cover art inside a dest rect ───────────────────────────────
static void DrawCover(Texture2D tex, Rectangle dest) {
    if (tex.id > 0) {
        Rectangle src = {0, 0, (float)tex.width, (float)tex.height};
        DrawTexturePro(tex, src, dest, {0, 0}, 0.0f, WHITE);
    } else {
        DrawRectangleRounded(dest, 0.1f, 6, C_GLASS_HOV);
    }
}

// ─── SIDE PANEL ──────────────────────────────────────────────────────────────
void DrawSidePanel() {
    DrawPanel(rSide);

    // Title
    DrawTextBold("SpotiFYE", (int)rSide.x + 14, (int)rSide.y + 12, 28, C_ACCENT);
    DrawText("Music Player",  (int)rSide.x + 14, (int)rSide.y + 46, 13, C_TXT3);
    DrawLineEx({rSide.x + 8, rSide.y + 68}, {rSide.x + rSide.width - 8, rSide.y + 68}, 0.5f, C_BORDER);

    // Nav buttons
    const char* lbls[] = {"All Songs", "Most Played", "History"};
    View        vws[]  = {VIEW_ALL, VIEW_MOST_PLAYED, VIEW_HISTORY};
    for (int i = 0; i < 3; i++) {
        Rectangle r = {rSide.x + 8, rSide.y + 78 + (float)(i * 50), rSide.width - 16, 42};
        if (FlatBtn(r, lbls[i], currentView == vws[i])) {
            currentView  = vws[i];
            scrollOffset = 0;
        }
    }

    // Bottom stats
    float sy = rSide.y + rSide.height - 110;
    DrawLineEx({rSide.x + 8, sy}, {rSide.x + rSide.width - 8, sy}, 0.5f, C_BORDER);
    DrawText("LIBRARY", (int)rSide.x + 14, (int)sy + 8,  12, C_TXT3);
    DrawText(TextFormat("%d songs", totalSongs), (int)rSide.x + 14, (int)sy + 26, 14, C_TXT2);

    // Current track index
    int idx = 0;
    Node* c = playlistHead;
    while (c && c != currentSong && c->next != playlistHead) { c = c->next; idx++; }
    DrawText(TextFormat("Track %d / %d", idx + 1, totalSongs), (int)rSide.x + 14, (int)sy + 48, 14, C_TXT2);

    // Repeat status
    Color rc = repeatAll ? C_ACCENT : C_TXT3;
    DrawText(repeatAll ? "REPEAT ON" : "REPEAT OFF", (int)rSide.x + 14, (int)sy + 70, 12, rc);

    // Equalizer
    DrawEqualizer(rSide.x + 14, rSide.y + rSide.height - 28, rSide.width - 28, 20, isPlaying);
}

// ─── PLAYLIST PANEL ──────────────────────────────────────────────────────────
void DrawPlaylistPanel() {
    DrawPanel(rMain);

    const char* titles[] = {"All Songs", "Most Played", "Playback History"};
    DrawTextBold(titles[currentView], (int)rMain.x + 14, (int)rMain.y + 10, 22, C_TXT1);
    DrawLineEx({rMain.x + 8, rMain.y + 42}, {rMain.x + rMain.width - 8, rMain.y + 42}, 0.5f, C_BORDER);

    int     vr    = visibleRows();
    Vector2 mouse = GetMousePosition();

    // ── All Songs ──────────────────────────────────────────────────────────
    if (currentView == VIEW_ALL) {
        if (CheckCollisionPointRec(mouse, rMain))
            scrollOffset -= (int)GetMouseWheelMove();
        int maxSc = totalSongs - vr;
        if (scrollOffset < 0)          scrollOffset = 0;
        if (maxSc > 0 && scrollOffset > maxSc) scrollOffset = maxSc;

        // Advance to scroll start
        Node* cur = playlistHead;
        for (int s = 0; s < scrollOffset && cur && cur->next != playlistHead; s++)
            cur = cur->next;

        int row = 0;
        while (cur && row < vr) {
            if (row > 0 && cur == playlistHead) break;

            bool active = (cur == currentSong);
            Rectangle rr = {rMain.x + 8, rMain.y + 50 + (float)(row * ROW_H), rMain.width - 18, (float)(ROW_H - 5)};
            bool hov = CheckCollisionPointRec(mouse, rr);

            DrawCard(rr, active, hov && !active);

            if (hov && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
                SpawnRipple(mouse.x, mouse.y, 80.0f);
                jumpToNode(cur);
                loadAndPlay();
            }

            // Track number
            DrawText(TextFormat("%d", scrollOffset + row + 1), (int)rr.x + 10, (int)rr.y + (ROW_H - 5) / 2 - 8, 13, C_TXT3);

            // Cover art (44x44)
            DrawCover(cur->song->coverTexture, {rr.x + 38, rr.y + 5, 44, 44});

            // Title + artist — max width keeps them from overlapping the duration
            float infoX   = rr.x + 90;
            float maxInfoW = rr.width - 90 - 80; // leave 80px for duration on right
            std::string title = ClampBold(cur->song->title,  18, (int)maxInfoW);
            std::string art   = Clamp(cur->song->artist,    13, (int)maxInfoW);
            DrawTextBold(title.c_str(), (int)infoX, (int)rr.y + 8,  18, C_TXT1);
            DrawText(art.c_str(),       (int)infoX, (int)rr.y + 32, 13, active ? C_ACCENT : C_TXT3);

            // Duration — right-aligned
            int mm = cur->song->duration / 60, ss = cur->song->duration % 60;
            const char* dur = TextFormat("%02d:%02d", mm, ss);
            int dw = MeasureText(dur, 13);
            DrawText(dur, (int)(rr.x + rr.width - dw - 10), (int)rr.y + (ROW_H - 5) / 2 - 7, 13, C_TXT3);

            // Play count badge on active track — sits left of duration
            if (active && cur->song->playCount > 0) {
                const char* pc = TextFormat("%d plays", cur->song->playCount);
                int pw = MeasureText(pc, 12);
                DrawText(pc, (int)(rr.x + rr.width - dw - pw - 22), (int)rr.y + (ROW_H - 5) / 2 - 6, 12, C_ACCENT);
            }

            if (!repeatAll && cur->next == nullptr) break;
            cur = cur->next;
            if (cur == playlistHead) break;
            row++;
        }

        // Scrollbar
        if (totalSongs > vr) {
            float sbH  = rMain.height - 50;
            float barH = sbH * vr / totalSongs;
            float barY = rMain.y + 50 + sbH * scrollOffset / totalSongs;
            DrawRectangle((int)(rMain.x + rMain.width - 5), (int)(rMain.y + 50), 3, (int)sbH, C_GLASS_HOV);
            DrawRectangle((int)(rMain.x + rMain.width - 5), (int)barY,            3, (int)barH, C_ACCENT);
        }

    // ── Most Played ────────────────────────────────────────────────────────
    } else if (currentView == VIEW_MOST_PLAYED) {
        Song* top[15]; int topCount = 0;
        heapTopN(top, 15, topCount);

        if (topCount == 0 || top[0]->playCount == 0) {
            DrawText("Play some songs to see your top tracks!", (int)rMain.x + 16, (int)rMain.y + 70, 15, C_TXT3);
            return;
        }

        for (int i = 0; i < topCount && i < vr; i++) {
            Rectangle rr = {rMain.x + 8, rMain.y + 50 + (float)(i * ROW_H), rMain.width - 18, (float)(ROW_H - 5)};
            bool hov = CheckCollisionPointRec(mouse, rr);
            DrawCard(rr, false, hov);

            // Rank badge
            DrawText(TextFormat("#%d", i + 1), (int)rr.x + 10, (int)rr.y + (ROW_H - 5) / 2 - 7, 14, C_ACCENT);

            DrawCover(top[i]->coverTexture, {rr.x + 38, rr.y + 5, 44, 44});

            float infoX   = rr.x + 90;
            float maxInfoW = rr.width - 90 - 90;
            std::string title = ClampBold(top[i]->title,  18, (int)maxInfoW);
            std::string art   = Clamp(top[i]->artist,    13, (int)maxInfoW);
            DrawTextBold(title.c_str(), (int)infoX, (int)rr.y + 8,  18, C_TXT1);
            DrawText(art.c_str(),       (int)infoX, (int)rr.y + 32, 13, C_TXT3);

            // Play count — right side
            const char* plays = TextFormat("%dx", top[i]->playCount);
            int pw = MeasureText(plays, 15);
            DrawText(plays, (int)(rr.x + rr.width - pw - 12), (int)rr.y + (ROW_H - 5) / 2 - 8, 15, C_ACCENT2);
        }

    // ── History ────────────────────────────────────────────────────────────
    } else {
        StackNode* cur = historyTop;
        int row = 0;
        while (cur && row < vr) {
            Rectangle rr = {rMain.x + 8, rMain.y + 50 + (float)(row * ROW_H), rMain.width - 18, (float)(ROW_H - 5)};
            DrawCard(rr, false, false);

            DrawText(TextFormat("%d", row + 1), (int)rr.x + 10, (int)rr.y + (ROW_H - 5) / 2 - 7, 13, C_TXT3);
            DrawCover(cur->song->coverTexture, {rr.x + 38, rr.y + 5, 44, 44});

            float infoX   = rr.x + 90;
            float maxInfoW = rr.width - 90 - 20;
            std::string title = ClampBold(cur->song->title,  18, (int)maxInfoW);
            std::string art   = Clamp(cur->song->artist,    13, (int)maxInfoW);
            DrawTextBold(title.c_str(), (int)infoX, (int)rr.y + 8,  18, C_TXT1);
            DrawText(art.c_str(),       (int)infoX, (int)rr.y + 32, 13, C_TXT3);

            cur = cur->next;
            row++;
        }
        if (row == 0)
            DrawText("No history yet — play a song!", (int)rMain.x + 16, (int)rMain.y + 70, 15, C_TXT3);
    }
}

// ─── RIGHT PANEL  (Up Next) ──────────────────────────────────────────────────
void DrawRightPanel() {
    DrawPanel(rRight);
    DrawTextBold("Up Next", (int)rRight.x + 12, (int)rRight.y + 10, 20, C_TXT1);
    DrawLineEx({rRight.x + 8, rRight.y + 38}, {rRight.x + rRight.width - 8, rRight.y + 38}, 0.5f, C_BORDER);

    Node*     c     = currentSong ? currentSong->next : nullptr;
    if (c == playlistHead) c = nullptr;
    int       row   = 0;
    const int UNR_H = 66;
    Vector2   mouse = GetMousePosition();

    while (c && c != playlistHead && row < 9) {
        Rectangle rr = {rRight.x + 6, rRight.y + 44 + (float)(row * UNR_H), rRight.width - 12, (float)(UNR_H - 5)};
        bool hov = CheckCollisionPointRec(mouse, rr);
        DrawCard(rr, false, hov);

        // Cover — small square
        DrawCover(c->song->coverTexture, {rr.x + 6, rr.y + 5, 46, 46});

        // Title + artist — clamp to remaining width
        float infoX   = rr.x + 58;
        float maxW    = rr.width - 58 - 8;
        std::string title = ClampBold(c->song->title,  14, (int)maxW);
        std::string art   = Clamp(c->song->artist,    12, (int)maxW);
        DrawTextBold(title.c_str(), (int)infoX, (int)rr.y + 8,  14, C_TXT1);
        DrawText(art.c_str(),       (int)infoX, (int)rr.y + 28, 12, C_TXT3);

        if (hov && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
            SpawnRipple(mouse.x, mouse.y, 60.0f);
            jumpToNode(c);
            loadAndPlay();
        }

        c = c->next;
        if (!repeatAll && c == nullptr) break;
        if (c == playlistHead) break;
        row++;
    }

    if (row == 0)
        DrawText(repeatAll ? "Looping playlist" : "End of playlist",
                 (int)rRight.x + 12, (int)rRight.y + 56, 13, C_TXT3);
}

// ─── PLAYER BAR ──────────────────────────────────────────────────────────────
void DrawPlayerBar() {
    DrawRectangleRec(rPlayer, C_SURF);

    // Pink accent line at top
    unsigned char la = (unsigned char)(70 + 70 * (float)(0.5 + 0.5 * sin(glowT * 2.1f)));
    DrawLineEx({rPlayer.x, rPlayer.y}, {rPlayer.x + rPlayer.width, rPlayer.y}, 1.5f, {220, 30, 95, la});

    // ── Album art (left) ─────────────────────────────────────────────────────
    Rectangle artR = {rPlayer.x + 10, rPlayer.y + 8, 88, 88};
    if (currentSong) {
        DrawCover(currentSong->song->coverTexture, artR);
        DrawRectangleRoundedLines(artR, 0.07f, 6, 1.0f, C_BORDER);
    } else {
        DrawRectangleRounded(artR, 0.07f, 6, C_GLASS_HOV);
        int tw = MeasureText("no song", 13);
        DrawText("no song", (int)(artR.x + artR.width / 2 - tw / 2), (int)(artR.y + 36), 13, C_TXT3);
    }

    // ── Song info (left, next to art) ────────────────────────────────────────
    float infoX = rPlayer.x + 108;
    float infoMaxW = 260.0f;   // hard cap before controls begin

    if (currentSong) {
        Song* s = currentSong->song;
        std::string title = ClampBold(s->title,  20, (int)infoMaxW);
        std::string genre_plays = Clamp(
            std::string(s->genre) + "  •  " + std::to_string(s->playCount) + " plays",
            12, (int)infoMaxW);
        std::string artist = Clamp(s->artist, 13, (int)infoMaxW);

        DrawTextBold(title.c_str(),       (int)infoX, (int)rPlayer.y + 10, 20, C_TXT1);
        DrawText(artist.c_str(),          (int)infoX, (int)rPlayer.y + 36, 13, C_TXT3);
        DrawText(genre_plays.c_str(),     (int)infoX, (int)rPlayer.y + 54, 12, C_TXT3);
    } else {
        DrawText("Select a track", (int)infoX, (int)rPlayer.y + 36, 16, C_TXT3);
    }

    // ── Transport controls (centred) ──────────────────────────────────────────
    float cx = rPlayer.x + rPlayer.width / 2.0f;
    float cy = rPlayer.y + 36;

    // Prev
    if (CircleBtn({cx - 76, cy}, 16, "|<")) { prevSong(); loadAndPlay(); }

    // Play / Pause
    Vector2 ppC = {cx, cy};
    bool ppHov = CheckCollisionPointCircle(GetMousePosition(), ppC, 24);
    if (ppHov && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) { SpawnRipple(ppC.x, ppC.y, 48.0f); togglePause(); }
    Color ppBg = isPlaying ? C_ACCENT : (ppHov ? C_GLASS_HOV : C_GLASS);
    Color ppBdr = ppHov ? C_ACCENT2 : (isPlaying ? C_ACCENT : C_BORDER);
    DrawCircleV(ppC, 24, ppBg);
    DrawCircleLinesV(ppC, 24, ppBdr);
    
    Color iCol = isPlaying ? C_WHITE : C_TXT1;
    if (isPlaying) {
        DrawRectangle((int)ppC.x - 6, (int)ppC.y - 7, 4, 14, iCol);
        DrawRectangle((int)ppC.x + 2, (int)ppC.y - 7, 4, 14, iCol);
    } else {
        DrawTriangle({ppC.x - 4, ppC.y - 8}, {ppC.x - 4, ppC.y + 8}, {ppC.x + 7, ppC.y}, iCol);
    }

    // Next
    if (CircleBtn({cx + 76, cy}, 16, ">|")) {
        Node* was = currentSong;
        nextSong();
        if (currentSong != was || repeatAll) loadAndPlay();
    }

    // ── Loop button (right of next) ───────────────────────────────────────────
    Vector2 repC = {cx + 120, cy};
    bool rHov = CheckCollisionPointCircle(GetMousePosition(), repC, 16);
    if (rHov && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) { SpawnRipple(repC.x, repC.y, 32.0f); toggleRepeat(); }
    Color rBdr = repeatAll ? C_ACCENT : (rHov ? C_BORD_ACC : C_BORDER);
    DrawCircleV(repC, 16, rHov ? C_GLASS_HOV : C_GLASS);
    DrawCircleLinesV(repC, 16, rBdr);
    DrawLoopIcon(repC, 11.0f, repeatAll ? C_ACCENT : C_TXT2);

    // ── Progress bar (centred, below controls) ────────────────────────────────
    float progress = 0, timePlayed = 0, timeLen = 1;
    if (musicLoaded) {
        timePlayed = GetMusicTimePlayed(musicStream);
        timeLen    = GetMusicTimeLength(musicStream);
        if (timeLen > 0) progress = timePlayed / timeLen;
    }

    float barW = rPlayer.width * 0.38f;
    float barX = cx - barW / 2.0f;
    float barY = rPlayer.y + 76;

    // Seek on click
    Rectangle seekArea = {barX - 8, barY - 10, barW + 16, 22};
    if (musicLoaded && CheckCollisionPointRec(GetMousePosition(), seekArea) && IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
        float t = (GetMousePosition().x - barX) / barW;
        if (t < 0) t = 0; if (t > 1) t = 1;
        SeekMusicStream(musicStream, t * timeLen);
    }

    DrawRectangleRounded({barX, barY, barW, 5}, 1, 4, C_SURF2);
    if (progress > 0.002f)
        DrawRectangleRounded({barX, barY, barW * progress, 5}, 1, 4, C_ACCENT);
    float kx = barX + barW * progress;
    DrawCircleV({kx, barY + 2.5f}, 7, C_WHITE);
    DrawCircleLinesV({kx, barY + 2.5f}, 7, C_ACCENT);

    // Time labels
    int mp = (int)timePlayed / 60, sp = (int)timePlayed % 60;
    int ml = (int)timeLen    / 60, sl = (int)timeLen    % 60;
    DrawText(TextFormat("%02d:%02d", mp, sp), (int)(barX - 46), (int)barY - 5, 12, C_TXT3);
    DrawText(TextFormat("%02d:%02d", ml, sl), (int)(barX + barW + 10), (int)barY - 5, 12, C_TXT3);

    // ── Volume (right side) ───────────────────────────────────────────────────
    float volX  = rPlayer.x + rPlayer.width - 185;
    float volCY = cy;

    // Mute button
    Vector2 muteC = {volX, volCY};
    bool muteHov = CheckCollisionPointCircle(GetMousePosition(), muteC, 15);
    if (muteHov && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) { SpawnRipple(muteC.x, muteC.y, 30.0f); toggleMute(); }
    DrawCircleV(muteC, 15, muteHov ? C_GLASS_HOV : C_GLASS);
    DrawCircleLinesV(muteC, 15, muteHov ? C_BORD_ACC : C_BORDER);

    // Speaker icon (body + cone)
    Color vCol = isMuted ? C_TXT3 : C_ACCENT;
    DrawRectangle((int)muteC.x - 6, (int)muteC.y - 3, 4, 6, vCol);
    DrawTriangle({muteC.x - 2, muteC.y - 4}, {muteC.x - 2, muteC.y + 4}, {muteC.x + 5, muteC.y + 7}, vCol);
    DrawTriangle({muteC.x - 2, muteC.y - 4}, {muteC.x + 5, muteC.y + 7}, {muteC.x + 5, muteC.y - 7}, vCol);
    if (isMuted)
        DrawLineEx({muteC.x - 7, muteC.y - 7}, {muteC.x + 7, muteC.y + 7}, 2.0f, {240, 60, 60, 255});

    // Volume slider
    float vBarX = volX + 22;
    float vBarW = 110;
    Rectangle vArea = {vBarX - 6, volCY - 10, vBarW + 12, 22};
    if (CheckCollisionPointRec(GetMousePosition(), vArea) && IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
        float vt = (GetMousePosition().x - vBarX) / vBarW;
        if (vt < 0) vt = 0; if (vt > 1) vt = 1;
        currentVolume = vt;
        isMuted = false;
        if (musicLoaded) SetMusicVolume(musicStream, currentVolume);
    }

    float dispVol = isMuted ? 0.0f : currentVolume;
    DrawRectangleRounded({vBarX, volCY - 2, vBarW, 4}, 1, 4, C_SURF2);
    if (dispVol > 0)
        DrawRectangleRounded({vBarX, volCY - 2, vBarW * dispVol, 4}, 1, 4, C_ACCENT);
    DrawCircleV({vBarX + vBarW * dispVol, volCY}, 6, C_WHITE);
    DrawCircleLinesV({vBarX + vBarW * dispVol, volCY}, 6, C_ACCENT);
}
