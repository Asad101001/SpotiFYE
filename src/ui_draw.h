#pragma once
#include <raylib.h>
#include "ui.h"
#include "Library.h"
#include "Playlist.h"
#include "History.h"
#include "MostPlayed.h"

// ─── VIEW ────────────────────────────────────────────────────────────────────
enum View { VIEW_ALL=0, VIEW_MOST_PLAYED=1, VIEW_HISTORY=2 };
View currentView = VIEW_ALL;

// ─── PLAYER STATE ────────────────────────────────────────────────────────────
Texture2D albumArt    = {0};
Music     musicStream = {0};
bool      musicLoaded = false;
bool      isPlaying   = false;

// ─── PLAYLIST SCROLL ─────────────────────────────────────────────────────────
int scrollOffset = 0;
const int ROW_H  = 52;

int visibleRows() { return (int)((rMain.height - 56) / ROW_H); }

// ─── AUDIO ───────────────────────────────────────────────────────────────────
// Increment play count and rebuild heap so Most Played stays accurate
void recordPlay(Song* s) {
    if (!s) return;
    s->playCount++;
    heapRebuild();
}

void loadAndPlay() {
    if (albumArt.id > 0) { UnloadTexture(albumArt); albumArt = {0}; }
    if (musicLoaded)     { StopMusicStream(musicStream); UnloadMusicStream(musicStream); musicLoaded = false; }
    if (!currentSong)    return;

    Song* s = currentSong->song;
    recordPlay(s);          // ← dynamic play-count increment
    historyPush(s);         // ← push to stack

    if (!s->coverPath.empty())
        albumArt = LoadTexture(s->coverPath.c_str());

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

    // Title
    DrawTextBold("SpotiFYE", (int)rSide.x+14, (int)rSide.y+12, 28, C_ACCENT);
    DrawText("Music Player", (int)rSide.x+14, (int)rSide.y+42, 12, C_TXT3);
    DrawLineEx({rSide.x+8, rSide.y+54}, {rSide.x+rSide.width-8, rSide.y+54}, 0.5f, C_BORDER);

    // Nav buttons
    const char* lbls[] = {"All Songs","Most Played","History"};
    View        vws[]  = {VIEW_ALL,VIEW_MOST_PLAYED,VIEW_HISTORY};
    for (int i=0;i<3;i++) {
        Rectangle r = {rSide.x+8, rSide.y+62+(float)(i*46), rSide.width-16, 38};
        if (FlatBtn(r, lbls[i], currentView==vws[i])) {
            currentView  = vws[i];
            scrollOffset = 0;
        }
    }

    // Stats
    float sy = rSide.y + rSide.height - 114;
    DrawLineEx({rSide.x+8,sy},{rSide.x+rSide.width-8,sy},0.5f,C_BORDER);
    DrawText("LIBRARY", (int)rSide.x+14, (int)sy+10, 10, C_TXT3);
    DrawText(TextFormat("%d songs", totalSongs), (int)rSide.x+14, (int)sy+28, 13, C_TXT2);

    // Current track index
    int idx=0; Node* c=playlistHead;
    while (c && c!=currentSong && c->next!=playlistHead) { c=c->next; idx++; }
    DrawText(TextFormat("Track %d / %d", idx+1, totalSongs), (int)rSide.x+14, (int)sy+48, 13, C_TXT2);

    // Repeat status with accent if on
    Color rc = repeatAll ? C_ACCENT : C_TXT3;
    DrawText(repeatAll ? "REPEAT  ON" : "REPEAT  OFF", (int)rSide.x+14, (int)sy+70, 11, rc);

    // Mini equalizer in sidebar when playing
    if (isPlaying)
        DrawEqualizer(rSide.x+14, rSide.y+rSide.height-34, rSide.width-28, 22, isPlaying);
}

// ─── PLAYLIST PANEL ──────────────────────────────────────────────────────────
void DrawPlaylistPanel() {
    DrawPanel(rMain);

    const char* titles[] = {"All Songs","Most Played","Playback History"};
    DrawTextBold(titles[currentView], (int)rMain.x+14, (int)rMain.y+10, 20, C_TXT1);
    DrawLineEx({rMain.x+8, rMain.y+36}, {rMain.x+rMain.width-8, rMain.y+36}, 0.5f, C_BORDER);

    int vr = visibleRows();
    Vector2 mouse = GetMousePosition();

    if (currentView == VIEW_ALL) {
        if (CheckCollisionPointRec(mouse, rMain))
            scrollOffset -= (int)GetMouseWheelMove();
        int maxSc = totalSongs - vr;
        if (scrollOffset < 0) scrollOffset = 0;
        if (maxSc>0 && scrollOffset>maxSc) scrollOffset=maxSc;

        Node* cur = playlistHead;
        for (int s=0; s<scrollOffset && cur && cur->next!=playlistHead; s++) cur=cur->next;

        int row=0;
        while (cur && row<vr && cur!=nullptr) {
            // Stop if we wrapped back to head in repeat mode
            if (row>0 && cur==playlistHead) break;

            bool active = (cur==currentSong);
            Rectangle rr = {rMain.x+8, rMain.y+42+(float)(row*ROW_H), rMain.width-16, (float)(ROW_H-4)};
            bool hov = CheckCollisionPointRec(mouse, rr);

            DrawCard(rr, active, hov && !active);

            if (hov && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
                SpawnRipple(mouse.x, mouse.y, 90.0f);
                jumpToNode(cur);
                loadAndPlay();
            }

            // Track number
            DrawText(TextFormat("%d", scrollOffset+row+1),(int)rr.x+10,(int)rr.y+17,11,C_TXT3);

            // Title + artist
            std::string title = Clamp(cur->song->title,14,(int)rMain.width-165);
            DrawTextBold(title.c_str(),(int)rr.x+36,(int)rr.y+6,16,C_TXT1);
            DrawText(cur->song->artist.c_str(),(int)rr.x+36,(int)rr.y+26,12,active?C_ACCENT:C_TXT3);

            // Duration right-aligned
            int mm=cur->song->duration/60, ss=cur->song->duration%60;
            const char* dur=TextFormat("%02d:%02d",mm,ss);
            int dw=MeasureText(dur,11);
            DrawText(dur,(int)(rr.x+rr.width-dw-10),(int)rr.y+17,11,C_TXT3);

            // Play count indicator for active song
            if (active) {
                const char* pc = TextFormat("%d plays", cur->song->playCount);
                int pw = MeasureText(pc,10);
                DrawText(pc,(int)(rr.x+rr.width-dw-pw-20),(int)rr.y+17,10,C_ACCENT);
            }

            if (!repeatAll && cur->next == nullptr) break;
            cur = cur->next;
            if (cur == playlistHead) break;
            row++;
        }

        // Scrollbar
        if (totalSongs > vr) {
            float sbH = rMain.height-46, barH=sbH*vr/totalSongs;
            float barY = rMain.y+42 + sbH*scrollOffset/totalSongs;
            DrawRectangle((int)(rMain.x+rMain.width-4),(int)(rMain.y+42),2,(int)sbH,C_GLASS_HOV);
            DrawRectangle((int)(rMain.x+rMain.width-4),(int)barY,2,(int)barH,C_ACCENT);
        }

    } else if (currentView == VIEW_MOST_PLAYED) {
        Song* top[15]; int topCount=0;
        heapTopN(top, 15, topCount);
        for (int i=0;i<topCount&&i<vr;i++) {
            Rectangle rr={rMain.x+8, rMain.y+42+(float)(i*ROW_H), rMain.width-16,(float)(ROW_H-4)};
            bool hov=CheckCollisionPointRec(mouse,rr);
            DrawCard(rr,false,hov);

            // Rank badge
            const char* rank=TextFormat("#%d",i+1);
            DrawText(rank,(int)rr.x+10,(int)rr.y+17,11,C_ACCENT);

            std::string title=Clamp(top[i]->title,14,(int)rMain.width-160);
            DrawTextBold(title.c_str(),(int)rr.x+42,(int)rr.y+6,16,C_TXT1);
            DrawText(top[i]->artist.c_str(),(int)rr.x+42,(int)rr.y+26,12,C_TXT3);

            const char* plays=TextFormat("%d plays",top[i]->playCount);
            int pw=MeasureText(plays,11);
            DrawText(plays,(int)(rr.x+rr.width-pw-10),(int)rr.y+17,11,C_ACCENT2);
        }

    } else { // VIEW_HISTORY
        StackNode* cur=historyTop;
        int row=0;
        while (cur && row<vr) {
            Rectangle rr={rMain.x+8,rMain.y+42+(float)(row*ROW_H),rMain.width-16,(float)(ROW_H-4)};
            DrawCard(rr,false,false);
            DrawText(TextFormat("%d",row+1),(int)rr.x+10,(int)rr.y+17,11,C_TXT3);
            std::string title=Clamp(cur->song->title,14,(int)rMain.width-120);
            DrawTextBold(title.c_str(),(int)rr.x+36,(int)rr.y+6,16,C_TXT1);
            DrawText(cur->song->artist.c_str(),(int)rr.x+36,(int)rr.y+26,12,C_TXT3);
            cur=cur->next; row++;
        }
        if (row==0) DrawText("No history yet — play a song!",(int)rMain.x+16,(int)rMain.y+60,13,C_TXT3);
    }
}

// ─── RIGHT PANEL ─────────────────────────────────────────────────────────────
void DrawRightPanel() {
    DrawPanel(rRight);
    DrawTextBold("Up Next",(int)rRight.x+12,(int)rRight.y+10,18,C_TXT1);
    DrawLineEx({rRight.x+8,rRight.y+34},{rRight.x+rRight.width-8,rRight.y+34},0.5f,C_BORDER);

    Node* c = currentSong ? currentSong->next : nullptr;
    if (c == playlistHead) c = nullptr; // avoid wrap in non-repeat
    int row=0;
    const int UNR_H = 48;
    Vector2 mouse=GetMousePosition();

    while (c && c!=playlistHead && row<9) {
        Rectangle rr={rRight.x+6, rRight.y+40+(float)(row*UNR_H), rRight.width-12, (float)(UNR_H-4)};
        bool hov=CheckCollisionPointRec(mouse,rr);
        DrawCard(rr,false,hov);

        std::string title=Clamp(c->song->title,12,(int)rRight.width-24);
        DrawTextBold(title.c_str(),(int)rr.x+10,(int)rr.y+4,14,C_TXT1);
        DrawText(c->song->artist.c_str(),(int)rr.x+10,(int)rr.y+22,11,C_TXT3);

        if (hov && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
            SpawnRipple(mouse.x,mouse.y,70.0f);
            jumpToNode(c);
            loadAndPlay();
        }
        c=c->next;
        if (!repeatAll && c==nullptr) break;
        if (c==playlistHead) break;
        row++;
    }
    if (row==0)
        DrawText(repeatAll?"Looping playlist":"End of playlist",(int)rRight.x+12,(int)rRight.y+52,11,C_TXT3);
}

// ─── PLAYER BAR ──────────────────────────────────────────────────────────────
void DrawPlayerBar() {
    // Background + top border line
    DrawRectangleRec(rPlayer, C_SURF);
    unsigned char la=(unsigned char)(80+70*(float)(0.5+0.5*sin(glowT*2.1f)));
    DrawLineEx({rPlayer.x,rPlayer.y},{rPlayer.x+rPlayer.width,rPlayer.y},2.0f,{220,30,95,la});

    int sw = GetScreenWidth();

    // ── Album art ──
    Rectangle artR={rPlayer.x+10, rPlayer.y+8, 86, 86};
    if (albumArt.id>0) {
        Rectangle src={0,0,(float)albumArt.width,(float)albumArt.height};
        DrawTexturePro(albumArt,src,artR,{0,0},0.0f,WHITE);
        DrawRectangleRoundedLines(artR,0.08f,6,1.2f,C_BORDER);
    } else {
        DrawRectangleRounded(artR,0.08f,6,C_GLASS_HOV);
        int tw=MeasureText("♪",26); DrawText("♪",(int)(artR.x+artR.width/2-tw/2),(int)(artR.y+28),26,C_TXT3);
    }

    // ── Song info ──
    if (currentSong) {
        Song* s=currentSong->song;
        std::string title=Clamp(s->title,17,270);
        DrawTextBold(title.c_str(),(int)rPlayer.x+106,(int)rPlayer.y+10,19,C_TXT1);
        DrawText(s->artist.c_str(),(int)rPlayer.x+106,(int)rPlayer.y+32,13,C_TXT3);
        DrawText(TextFormat("Genre: %s  •  %d plays",s->genre.c_str(),s->playCount),
                 (int)rPlayer.x+106,(int)rPlayer.y+52,11,C_TXT3);
    } else {
        DrawText("Select a track",(int)rPlayer.x+106,(int)rPlayer.y+35,15,C_TXT3);
    }

    // ── Controls (horizontally centred) ──
    float cx = rPlayer.x + rPlayer.width/2.0f;
    float cy = rPlayer.y + 40;

    if (CircleBtn({cx-82,cy},17,"|<")) { prevSong(); loadAndPlay(); }
    if (CircleBtn({cx,   cy},24,isPlaying?"||":">",isPlaying)) togglePause();
    if (CircleBtn({cx+82,cy},17,">|")) {
        Node* prev=currentSong;
        nextSong();
        if (currentSong!=prev || repeatAll) loadAndPlay();
    }

    // Repeat toggle (R key also works)
    Vector2 repC={cx+128,cy};
    bool rHov=CheckCollisionPointCircle(GetMousePosition(),repC,14);
    if (rHov && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) { SpawnRipple(repC.x,repC.y,28.0f); toggleRepeat(); }
    Color rBdr=repeatAll?C_ACCENT:(rHov?C_BORD_ACC:C_BORDER);
    DrawCircleLinesV(repC,14,rBdr);
    int rw=MeasureText("R",11); DrawText("R",(int)(repC.x-rw/2),(int)(repC.y-6),11,repeatAll?C_ACCENT:C_TXT3);

    // ── Progress bar ──
    float progress=0,timePlayed=0,timeLen=1;
    if (musicLoaded) {
        timePlayed=GetMusicTimePlayed(musicStream);
        timeLen   =GetMusicTimeLength(musicStream);
        if (timeLen>0) progress=timePlayed/timeLen;
    }

    float barW=440, barX=rPlayer.x+rPlayer.width/2.0f - barW/2.0f, barY=rPlayer.y+78;
    Rectangle seekArea={barX-6,barY-10,barW+12,22};
    if (musicLoaded && CheckCollisionPointRec(GetMousePosition(),seekArea) && IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
        float t=(GetMousePosition().x-barX)/barW;
        if(t<0)t=0; if(t>1)t=1;
        SeekMusicStream(musicStream,t*timeLen);
    }

    DrawRectangleRounded({barX,barY,barW,4},1,4,C_SURF2);
    if (progress>0.002f)
        DrawRectangleRounded({barX,barY,barW*progress,4},1,4,C_ACCENT);
    float kx=barX+barW*progress;
    DrawCircleV({kx,barY+2},7,C_WHITE);
    DrawCircleLinesV({kx,barY+2},7,C_ACCENT);

    int mp=(int)timePlayed/60,sp=(int)timePlayed%60;
    int ml=(int)timeLen/60,sl=(int)timeLen%60;
    DrawText(TextFormat("%02d:%02d",mp,sp),(int)(barX-42),(int)barY-2,11,C_TXT3);
    DrawText(TextFormat("%02d:%02d",ml,sl),(int)(barX+barW+8),(int)barY-2,11,C_TXT3);

    // ── Mini equalizer beside controls ──
    DrawEqualizer(cx-200, rPlayer.y+10, 60, 30, isPlaying);
    DrawEqualizer(cx+140, rPlayer.y+10, 60, 30, isPlaying);
}
