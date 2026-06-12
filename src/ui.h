#pragma once
#include <raylib.h>
#include <string>
#include <cstring>
#include <cmath>

// ─── AMOLED HOT-PINK THEME ───────────────────────────────────────────────────
const Color C_BG        = {  0,  0,  0, 255 };
const Color C_SURF      = {  9,  9,  9, 255 };
const Color C_SURF2     = { 15, 15, 15, 255 };
const Color C_GLASS     = { 255,255,255,  9 };
const Color C_GLASS_HOV = { 255,255,255, 20 };
const Color C_GLASS_SEL = { 220, 30, 95, 48 };
const Color C_BORDER    = { 255,255,255, 18 };
const Color C_BORD_ACC  = { 220, 30, 95,160 };
const Color C_ACCENT    = { 220, 30, 95,255 };   // hot pink
const Color C_ACCENT2   = { 255, 65,130,255 };   // lighter pink
const Color C_ACCENT_DIM= { 110, 14, 46,255 };
const Color C_WHITE     = WHITE;
const Color C_TXT1      = WHITE;
const Color C_TXT2      = { 185,185,200,255 };
const Color C_TXT3      = {  80, 80, 95,255 };

// ─── LAYOUT (dynamic, updated each frame) ────────────────────────────────────
const int PAD      = 10;
const int SIDE_W   = 195;
const int RIGHT_W  = 215;
const int PLAYER_H = 106;

Rectangle rSide, rMain, rRight, rPlayer;

void UpdateLayout() {
    int sw = GetScreenWidth(), sh = GetScreenHeight();
    int topH = sh - PLAYER_H - PAD*3;
    rSide   = {(float)PAD,                          (float)PAD, (float)SIDE_W,  (float)topH};
    rMain   = {(float)(PAD+SIDE_W+PAD),              (float)PAD, (float)(sw-PAD*4-SIDE_W-RIGHT_W), (float)topH};
    rRight  = {(float)(sw-PAD-RIGHT_W),              (float)PAD, (float)RIGHT_W, (float)topH};
    rPlayer = {(float)PAD, (float)(sh-PLAYER_H-PAD), (float)(sw-PAD*2), (float)PLAYER_H};
}

// ─── ANIMATION STATE ─────────────────────────────────────────────────────────
float glowT = 0.0f;          // sinusoidal time driver
float eqBars[12];            // equalizer bar heights (0..1)

void UpdateAnimations() {
    glowT = (float)GetTime();
    float dt = GetFrameTime();
    for (int i = 0; i < 12; i++) {
        float target = 0.15f + 0.85f * (float)(0.5 + 0.5*sin(glowT*(1.8f+i*0.55f) + i*0.9f));
        eqBars[i] += (target - eqBars[i]) * (1.0f - expf(-14.0f * dt)); // smooth lerp
    }
}

// ─── RIPPLE SYSTEM ───────────────────────────────────────────────────────────
struct Ripple { float x,y,r,maxR,alpha; bool active; };
const int MAX_RIPPLES = 10;
Ripple ripples[MAX_RIPPLES];

void SpawnRipple(float x, float y, float maxR = 70.0f) {
    for (int i = 0; i < MAX_RIPPLES; i++) {
        if (!ripples[i].active) {
            ripples[i] = {x,y,0,maxR,1.0f,true};
            return;
        }
    }
}

void UpdateRipples() {
    float dt = GetFrameTime();
    for (int i = 0; i < MAX_RIPPLES; i++) {
        if (!ripples[i].active) continue;
        ripples[i].r     += ripples[i].maxR * 3.5f * dt;
        ripples[i].alpha -= 3.0f * dt;
        if (ripples[i].alpha <= 0.0f) ripples[i].active = false;
    }
}

void DrawRipples() {
    for (int i = 0; i < MAX_RIPPLES; i++) {
        if (!ripples[i].active) continue;
        unsigned char a = (unsigned char)(255 * ripples[i].alpha * 0.28f);
        DrawCircleLinesV({ripples[i].x, ripples[i].y}, ripples[i].r, {220,30,95,a});
    }
}

// ─── HOVER STATE (smooth glow alpha per row) ─────────────────────────────────
// Lightweight — we just store the last hovered row and lerp alpha globally
float hoverAlpha = 0.0f;

// ─── HELPERS ─────────────────────────────────────────────────────────────────

// Frosted surface panel
void DrawPanel(Rectangle r) {
    DrawRectangleRec(r, C_SURF);
    DrawRectangleRoundedLines(r, 0.02f, 6, 0.8f, C_BORDER);
}

// Glass row card
void DrawCard(Rectangle r, bool active, bool hov) {
    Color fill = active ? C_GLASS_SEL : (hov ? C_GLASS_HOV : C_GLASS);
    DrawRectangleRounded(r, 0.14f, 8, fill);
    if (active) {
        unsigned char ga = (unsigned char)(100 + 60*(float)(0.5+0.5*sin(glowT*2.4f)));
        DrawRectangleRoundedLines(r, 0.14f, 8, 1.4f, {220,30,95,ga});
    } else if (hov) {
        DrawRectangleRoundedLines(r, 0.14f, 8, 0.8f, C_BORD_ACC);
    }
}

// Clickable circle button — returns true if clicked
bool CircleBtn(Vector2 c, float r, const char* icon, bool active=false) {
    Vector2 m = GetMousePosition();
    bool hov = CheckCollisionPointCircle(m, c, r);
    bool clk = hov && IsMouseButtonReleased(MOUSE_LEFT_BUTTON);
    if (clk) SpawnRipple(c.x, c.y, r*2.2f);

    Color bg  = active ? C_ACCENT : (hov ? C_GLASS_HOV : C_GLASS);
    Color bdr = hov ? C_ACCENT2 : (active ? C_ACCENT : C_BORDER);
    DrawCircleV(c, r, bg);
    DrawCircleLinesV(c, r, bdr);
    int tw = MeasureText(icon,14); DrawText(icon,(int)(c.x-tw/2),(int)(c.y-7),14,C_TXT1);
    return clk;
}

// Flat pill button
bool FlatBtn(Rectangle r, const char* lbl, bool active=false) {
    Vector2 m = GetMousePosition();
    bool hov = CheckCollisionPointRec(m,r);
    bool clk = hov && IsMouseButtonReleased(MOUSE_LEFT_BUTTON);
    if (clk) SpawnRipple(m.x, m.y, 60.0f);

    Color bg  = active ? C_ACCENT : (hov ? C_GLASS_HOV : C_GLASS);
    Color bdr = active ? C_ACCENT2 : (hov ? C_BORD_ACC : C_BORDER);
    Color tc  = (active||hov) ? C_TXT1 : C_TXT2;
    DrawRectangleRounded(r, 0.3f, 8, bg);
    DrawRectangleRoundedLines(r, 0.3f, 8, 0.8f, bdr);
    int tw = MeasureText(lbl,13); DrawText(lbl,(int)(r.x+r.width/2-tw/2),(int)(r.y+r.height/2-7),13,tc);
    return clk;
}

// Clamp string to pixel width
std::string Clamp(const std::string& s, int fs, int maxPx) {
    if (MeasureText(s.c_str(),fs) <= maxPx) return s;
    std::string o=s;
    while (o.size()>2 && MeasureText(o.c_str(),fs)>maxPx) o.resize(o.size()-1);
    return o+"..";
}

// Equalizer bars animation (drawn at given rect, only when playing)
void DrawEqualizer(float x, float y, float w, float h, bool playing) {
    int bars = 12;
    float bw = w / (bars*2.0f - 1.0f);
    for (int i = 0; i < bars; i++) {
        float bh = h * (playing ? eqBars[i] : 0.05f);
        unsigned char a = playing ? 210 : 60;
        DrawRectangleRounded({x + i*(bw*2), y+h-bh, bw, bh}, 0.5f, 4, {220,30,95,a});
    }
}
