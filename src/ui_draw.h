#pragma once
#include <raylib.h>
#include "ui.h"
#include "Library.h"
#include "Playlist.h"
#include "History.h"
#include "MostPlayed.h"

// ─── VIEW ENUM ───────────────────────────────────────────────────────────────
enum View { VIEW_ALL=0, VIEW_MOST_PLAYED=1, VIEW_HISTORY=2 };
View   currentView   = VIEW_ALL;

// ─── PLAYER STATE ────────────────────────────────────────────────────────────
Texture2D albumArt    = { 0 };
Music     musicStream = { 0 };
bool      musicLoaded = false;
bool      isPlaying   = false;
Node*     lastNode    = nullptr;

// ─── PLAYLIST SCROLL ─────────────────────────────────────────────────────────
int  scrollOffset = 0;
const int ROW_H   = 50;

// Compute how many rows fit in the playlist panel
int visibleRows() {
    return (int)((rMain.height - 52) / ROW_H);
}

// ─── AUDIO ───────────────────────────────────────────────────────────────────
void loadAndPlay() {
    if (albumArt.id > 0)  { UnloadTexture(albumArt); albumArt = {0}; }
    if (musicLoaded)      { StopMusicStream(musicStream); UnloadMusicStream(musicStream); musicLoaded = false; }
    if (!currentSong)     return;

    Song* s = currentSong->song;
    if (!s->coverPath.empty()) {
        albumArt = LoadTexture(s->coverPath.c_str());
        // If load failed (bad path), albumArt.id stays 0
    }
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

// ─── SIDE PANEL ──────────────────────────────────────────────────────────────
void DrawSidePanel() {
    DrawPanel(rSide);

    // App title
    DrawText("SpotiFYE", (int)rSide.x + 14, (int)rSide.y + 14, 22, C_ACCENT);
    DrawText("Music Player", (int)rSide.x + 14, (int)rSide.y + 38, 11, C_TXT3);
    DrawLineEx({rSide.x+10, rSide.y+56}, {rSide.x+rSide.width-10, rSide.y+56}, 0.6f, C_BORDER);

    const char* labels[] = { "All Songs", "Most Played", "History" };
    View        views[]  = { VIEW_ALL, VIEW_MOST_PLAYED, VIEW_HISTORY };
    for (int i = 0; i < 3; i++) {
        Rectangle r = { rSide.x+10, rSide.y+68 + i*44.0f, rSide.width-20, 36 };
        if (DrawFlatBtn(r, labels[i], currentView == views[i])) {
            currentView = views[i];
            scrollOffset = 0;
        }
    }

    // Stats block
    DrawLineEx({rSide.x+10, rSide.y+rSide.height-110}, {rSide.x+rSide.width-10, rSide.y+rSide.height-110}, 0.6f, C_BORDER);
    DrawSectionHeader(rSide.x+14, rSide.y+rSide.height-100, rSide.width-28, "LIBRARY");
    DrawText(TextFormat("%d songs", totalSongs), (int)rSide.x+14, (int)rSide.y+rSide.height-76, 13, C_TXT2);

    // Current track index
    int idx = 0;
    Node* c = playlistHead;
    while (c && c != currentSong && c->next != playlistHead) { c = c->next; idx++; }
    DrawText(TextFormat("Track %d / %d", idx+1, totalSongs), (int)rSide.x+14, (int)rSide.y+rSide.height-56, 13, C_TXT2);

    // Repeat status
    DrawText(repeatAll ? "REPEAT: ON" : "REPEAT: OFF", (int)rSide.x+14, (int)rSide.y+rSide.height-36, 11, repeatAll ? C_ACCENT : C_TXT3);
}

// ─── PLAYLIST PANEL ──────────────────────────────────────────────────────────
void DrawPlaylistPanel() {
    DrawPanel(rMain);

    // Header
    const char* viewTitle = (currentView==VIEW_ALL) ? "All Songs" :
                            (currentView==VIEW_MOST_PLAYED) ? "Most Played" : "Playback History";
    DrawText(viewTitle, (int)rMain.x+14, (int)rMain.y+12, 16, C_TXT1);
    DrawLineEx({rMain.x+10, rMain.y+36}, {rMain.x+rMain.width-10, rMain.y+36}, 0.6f, C_BORDER);

    int vr = visibleRows();

    if (currentView == VIEW_ALL) {
        // Mouse wheel scroll
        if (CheckCollisionPointRec(GetMousePosition(), rMain))
            scrollOffset -= (int)GetMouseWheelMove();
        int maxScroll = totalSongs - vr;
        if (scrollOffset < 0) scrollOffset = 0;
        if (maxScroll > 0 && scrollOffset > maxScroll) scrollOffset = maxScroll;

        // Walk linked list to scroll offset
        Node* cur = playlistHead;
        for (int s = 0; s < scrollOffset && cur && cur->next != playlistHead; s++)
            cur = cur->next;

        int row = 0;
        while (cur && row < vr && cur->next != playlistHead) {
            bool active  = (cur == currentSong);
            bool hovered = CheckCollisionPointRec(GetMousePosition(),
                           {rMain.x+8, rMain.y+42+(float)(row*ROW_H), rMain.width-16, (float)(ROW_H-3)});
            DrawCard({rMain.x+8, rMain.y+42+(float)(row*ROW_H), rMain.width-16, (float)(ROW_H-3)}, active, hovered && !active);

            if (hovered && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
                jumpToNode(cur);
                historyPush(currentSong->song);
                loadAndPlay();
            }

            // Track number
            DrawText(TextFormat("%d", scrollOffset+row+1),
                     (int)rMain.x+14, (int)(rMain.y+42+row*ROW_H+9), 11, C_TXT3);

            // Title
            std::string title = ClampStr(cur->song->title, 14, (int)rMain.width - 150);
            DrawText(title.c_str(), (int)rMain.x+38, (int)(rMain.y+42+row*ROW_H+6), 14, active ? C_WHITE : C_TXT1);
            DrawText(cur->song->artist.c_str(), (int)rMain.x+38, (int)(rMain.y+42+row*ROW_H+24), 11, active ? C_ACCENT : C_TXT3);

            // Duration
            int mm = cur->song->duration/60, ss = cur->song->duration%60;
            const char* dur = TextFormat("%02d:%02d", mm, ss);
            int dw = MeasureText(dur, 11);
            DrawText(dur, (int)(rMain.x+rMain.width-dw-14), (int)(rMain.y+42+row*ROW_H+18), 11, C_TXT3);

            cur = cur->next;
            row++;
        }

        // Scrollbar
        if (totalSongs > vr) {
            float sbH  = rMain.height - 46;
            float barH = sbH * vr / totalSongs;
            float barY = rMain.y + 42 + sbH * scrollOffset / totalSongs;
            DrawRectangle((int)(rMain.x+rMain.width-5), (int)(rMain.y+42), 3, (int)sbH, C_GLASS_HOV);
            DrawRectangle((int)(rMain.x+rMain.width-5), (int)barY, 3, (int)barH, C_ACCENT);
        }

    } else if (currentView == VIEW_MOST_PLAYED) {
        Song* top[10]; int topCount = 0;
        heapTopN(top, 10, topCount);
        for (int i = 0; i < topCount && i < vr; i++) {
            bool hovered = CheckCollisionPointRec(GetMousePosition(),
                           {rMain.x+8, rMain.y+42+(float)(i*ROW_H), rMain.width-16, (float)(ROW_H-3)});
            DrawCard({rMain.x+8, rMain.y+42+(float)(i*ROW_H), rMain.width-16, (float)(ROW_H-3)}, false, hovered);

            // Rank badge
            DrawText(TextFormat("#%d", i+1), (int)rMain.x+14, (int)(rMain.y+42+i*ROW_H+16), 11, C_ACCENT);
            std::string title = ClampStr(top[i]->title, 14, (int)rMain.width-160);
            DrawText(title.c_str(), (int)rMain.x+42, (int)(rMain.y+42+i*ROW_H+6), 14, C_TXT1);
            DrawText(top[i]->artist.c_str(), (int)rMain.x+42, (int)(rMain.y+42+i*ROW_H+24), 11, C_TXT3);
            DrawText(TextFormat("%d plays", top[i]->playCount),
                     (int)(rMain.x+rMain.width-80), (int)(rMain.y+42+i*ROW_H+16), 11, C_TXT3);
        }

    } else { // VIEW_HISTORY
        StackNode* cur = historyTop;
        int row = 0;
        while (cur && row < vr) {
            DrawCard({rMain.x+8, rMain.y+42+(float)(row*ROW_H), rMain.width-16, (float)(ROW_H-3)}, false, false);
            DrawText(TextFormat("%d", row+1), (int)rMain.x+14, (int)(rMain.y+42+row*ROW_H+16), 11, C_TXT3);
            std::string title = ClampStr(cur->song->title, 14, (int)rMain.width-120);
            DrawText(title.c_str(), (int)rMain.x+38, (int)(rMain.y+42+row*ROW_H+6), 14, C_TXT1);
            DrawText(cur->song->artist.c_str(), (int)rMain.x+38, (int)(rMain.y+42+row*ROW_H+24), 11, C_TXT3);
            cur = cur->next;
            row++;
        }
        if (row == 0)
            DrawText("No history yet — start playing!", (int)rMain.x+14, (int)rMain.y+60, 13, C_TXT3);
    }
}

// ─── RIGHT PANEL: UP NEXT ────────────────────────────────────────────────────
void DrawRightPanel() {
    DrawPanel(rRight);
    DrawText("Up Next", (int)rRight.x+12, (int)rRight.y+12, 15, C_TXT1);
    DrawLineEx({rRight.x+8, rRight.y+34}, {rRight.x+rRight.width-8, rRight.y+34}, 0.6f, C_BORDER);

    Node* c = currentSong ? currentSong->next : nullptr;
    int row = 0;
    while (c && c != playlistHead && row < 8) {
        bool hov = CheckCollisionPointRec(GetMousePosition(),
                   {rRight.x+6, rRight.y+40+(float)(row*48), rRight.width-12, 44});
        DrawCard({rRight.x+6, rRight.y+40+(float)(row*48), rRight.width-12, 44}, false, hov);

        std::string title = ClampStr(c->song->title, 12, (int)rRight.width-26);
        DrawText(title.c_str(), (int)rRight.x+12, (int)(rRight.y+42+row*48), 12, C_TXT1);
        DrawText(c->song->artist.c_str(), (int)rRight.x+12, (int)(rRight.y+58+row*48), 10, C_TXT3);

        if (hov && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
            historyPush(currentSong->song);
            jumpToNode(c);
            loadAndPlay();
        }
        c = c->next;
        row++;
    }
    if (row == 0) {
        DrawText(repeatAll ? "Repeat ON - loops" : "End of playlist",
                 (int)rRight.x+12, (int)rRight.y+50, 11, C_TXT3);
    }
}

// ─── PLAYER BAR ──────────────────────────────────────────────────────────────
void DrawPlayerBar() {
    // Full-width glass bar
    DrawRectangleRec(rPlayer, C_SURFACE);
    // top highlight line
    DrawLineEx({rPlayer.x, rPlayer.y}, {rPlayer.x+rPlayer.width, rPlayer.y}, 1.0f, C_BORDER);

    // Glowing accent underline that pulses with music
    float lineAlpha = (float)(80 + 100 * glowPulse);
    DrawLineEx({rPlayer.x, rPlayer.y}, {rPlayer.x+rPlayer.width, rPlayer.y},
               2.0f, {220, 30, 100, (unsigned char)lineAlpha});

    // ── Album art ──
    Rectangle artR = {rPlayer.x+10, rPlayer.y+8, 80, 80};
    if (albumArt.id > 0) {
        Rectangle src = {0, 0, (float)albumArt.width, (float)albumArt.height};
        DrawTexturePro(albumArt, src, artR, {0,0}, 0.0f, WHITE);
        // rounded clip overlay
        DrawRectangleRoundedLines(artR, 0.1f, 6, 1.5f, C_BORDER);
    } else {
        DrawRectangleRounded(artR, 0.1f, 6, C_GLASS_HOV);
        int tw = MeasureText("♪", 22); DrawText("♪", (int)(artR.x+artR.width/2-tw/2), (int)(artR.y+28), 22, C_TXT3);
    }

    // ── Song info ──
    if (currentSong) {
        Song* s = currentSong->song;
        std::string title = ClampStr(s->title, 16, 260);
        DrawText(title.c_str(),    (int)rPlayer.x+100, (int)rPlayer.y+12, 16, C_TXT1);
        DrawText(s->artist.c_str(),(int)rPlayer.x+100, (int)rPlayer.y+33, 12, C_TXT3);
        DrawText(TextFormat("Genre: %s", s->genre.c_str()), (int)rPlayer.x+100, (int)rPlayer.y+50, 11, C_TXT3);
    } else {
        DrawText("Select a song to play", (int)rPlayer.x+100, (int)rPlayer.y+30, 14, C_TXT3);
    }

    // ── Controls (centred) ──
    float cx = rPlayer.x + rPlayer.width / 2.0f;
    float cy = rPlayer.y + 38;

    if (DrawCircleBtn({cx-80, cy}, 16, "|<"))  { prevSong(); historyPush(currentSong->song); loadAndPlay(); }
    if (DrawCircleBtn({cx,    cy}, 22, isPlaying ? "||" : ">", isPlaying)) togglePause();
    if (DrawCircleBtn({cx+80, cy}, 16, ">|"))  { nextSong(); historyPush(currentSong ? currentSong->song : nullptr); loadAndPlay(); }

    // Repeat toggle button
    bool repHov = CheckCollisionPointCircle(GetMousePosition(), {cx+120, cy}, 14);
    if (repHov && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) toggleRepeat();
    Color repBdr = repeatAll ? C_ACCENT : (repHov ? C_BORDER_ACC : C_BORDER);
    DrawCircleLinesV({cx+120, cy}, 14, repBdr);
    int rw = MeasureText("R", 11); DrawText("R", (int)(cx+120-rw/2), (int)(cy-6), 11, repeatAll ? C_ACCENT : C_TXT3);

    // ── Progress bar ──
    float progress = 0.0f, timePlayed = 0.0f, timeLen = 1.0f;
    if (musicLoaded) {
        timePlayed = GetMusicTimePlayed(musicStream);
        timeLen    = GetMusicTimeLength(musicStream);
        if (timeLen > 0.0f) progress = timePlayed / timeLen;
    }

    float barX = rPlayer.x + rPlayer.width/2.0f - 220;
    float barY = rPlayer.y + 72;
    float barW = 440.0f;

    // Seekable
    Rectangle seekArea = {barX-4, barY-8, barW+8, 20};
    if (musicLoaded && CheckCollisionPointRec(GetMousePosition(), seekArea) && IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
        float t = (GetMousePosition().x - barX) / barW;
        if (t < 0) t = 0; if (t > 1) t = 1;
        SeekMusicStream(musicStream, t * timeLen);
    }

    DrawRectangleRounded({barX, barY, barW, 4}, 1.0f, 4, C_SURFACE2);
    if (progress > 0.005f)
        DrawRectangleRounded({barX, barY, barW * progress, 4}, 1.0f, 4, C_ACCENT);
    // Scrubber knob
    float kx = barX + barW * progress;
    DrawCircleV({kx, barY+2}, 7, C_WHITE);
    DrawCircleLinesV({kx, barY+2}, 7, C_ACCENT);

    // Timestamps
    int mp=(int)timePlayed/60, sp=(int)timePlayed%60;
    int ml=(int)timeLen/60,    sl=(int)timeLen%60;
    DrawText(TextFormat("%02d:%02d",mp,sp), (int)(barX-40),(int)(barY-2), 11, C_TXT3);
    DrawText(TextFormat("%02d:%02d",ml,sl), (int)(barX+barW+8),(int)(barY-2), 11, C_TXT3);
}
