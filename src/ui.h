#pragma once
#include <raylib.h>
#include <string>
#include <cstring>
#include <cmath>

extern Font fontBold;
extern Font fontReg;

// ─── CUSTOM FONT DRAWING MACROS ──────────────────────────────────────────────
// Always use these instead of raw DrawText/MeasureText so the custom font
// is applied consistently everywhere in the UI.
#define DrawTextBold(text,x,y,s,c)  DrawTextEx(fontBold, text, {(float)(x),(float)(y)}, (float)(s), 0.5f, c)
#define MeasureTextBold(text,s)     ((int)MeasureTextEx(fontBold, text, (float)(s), 0.5f).x)
#define DrawText(text,x,y,s,c)      DrawTextEx(fontReg,  text, {(float)(x),(float)(y)}, (float)(s), 0.5f, c)
#define MeasureText(text,s)         ((int)MeasureTextEx(fontReg,  text, (float)(s), 0.5f).x)

// ─── AMOLED HOT-PINK THEME ───────────────────────────────────────────────────
const Color C_BG        = {  0,  0,  0, 255 };
const Color C_SURF      = {  9,  9,  9, 255 };
const Color C_SURF2     = { 18, 18, 18, 255 };
const Color C_GLASS     = { 255,255,255,  8  };
const Color C_GLASS_HOV = { 255,255,255, 20  };
const Color C_GLASS_SEL = { 220, 30, 95, 44  };
const Color C_BORDER    = { 255,255,255, 22  };
const Color C_BORD_ACC  = { 220, 30, 95,160  };
const Color C_ACCENT    = { 220, 30, 95, 255 };   // hot pink
const Color C_ACCENT2   = { 255, 65,130, 255 };   // lighter pink
const Color C_ACCENT_DIM= { 100, 12, 42, 255 };
const Color C_WHITE     = WHITE;
const Color C_TXT1      = { 245,245,255, 255 };
const Color C_TXT2      = { 180,180,195, 255 };
const Color C_TXT3      = {  90, 90,105, 255 };

// ─── LAYOUT ──────────────────────────────────────────────────────────────────
const int PAD      = 10;
const int SIDE_W   = 200;
const int RIGHT_W  = 220;
const int PLAYER_H = 110;

Rectangle rSide, rMain, rRight, rPlayer;

void UpdateLayout() {
    int sw = GetScreenWidth(), sh = GetScreenHeight();
    int topH = sh - PLAYER_H - PAD * 3;
    rSide   = { (float)PAD,                               (float)PAD, (float)SIDE_W,                           (float)topH };
    rMain   = { (float)(PAD + SIDE_W + PAD),              (float)PAD, (float)(sw - PAD*4 - SIDE_W - RIGHT_W), (float)topH };
    rRight  = { (float)(sw - PAD - RIGHT_W),              (float)PAD, (float)RIGHT_W,                          (float)topH };
    rPlayer = { (float)PAD, (float)(sh - PLAYER_H - PAD), (float)(sw - PAD*2),                                 (float)PLAYER_H };
}

// ─── ANIMATION STATE ─────────────────────────────────────────────────────────
float glowT    = 0.0f;
float eqBars[12] = {};

void UpdateAnimations() {
    glowT = (float)GetTime();
    float dt = GetFrameTime();
    for (int i = 0; i < 12; i++) {
        float target = 0.15f + 0.85f * (float)(0.5 + 0.5 * sin(glowT * (1.8f + i * 0.55f) + i * 0.9f));
        eqBars[i] += (target - eqBars[i]) * (1.0f - expf(-14.0f * dt));
    }
}

// ─── RIPPLE SYSTEM ───────────────────────────────────────────────────────────
struct Ripple { float x, y, r, maxR, alpha; bool active; };
const int MAX_RIPPLES = 10;
Ripple ripples[MAX_RIPPLES] = {};

void SpawnRipple(float x, float y, float maxR = 70.0f) {
    for (int i = 0; i < MAX_RIPPLES; i++) {
        if (!ripples[i].active) {
            ripples[i] = { x, y, 0, maxR, 1.0f, true };
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
        unsigned char a = (unsigned char)(255 * ripples[i].alpha * 0.25f);
        DrawCircleLinesV({ ripples[i].x, ripples[i].y }, ripples[i].r, { 220,30,95,a });
    }
}

// ─── HELPERS ─────────────────────────────────────────────────────────────────

// Frosted surface panel with thin border
void DrawPanel(Rectangle r) {
    DrawRectangleRec(r, C_SURF);
    DrawRectangleRoundedLines(r, 0.02f, 6, 0.8f, C_BORDER);
}

// Glass row card — active gets pink glow border
void DrawCard(Rectangle r, bool active, bool hov) {
    Color fill = active ? C_GLASS_SEL : (hov ? C_GLASS_HOV : C_GLASS);
    DrawRectangleRounded(r, 0.12f, 8, fill);
    if (active) {
        unsigned char ga = (unsigned char)(90 + 55 * (float)(0.5 + 0.5 * sin(glowT * 2.4f)));
        DrawRectangleRoundedLines(r, 0.12f, 8, 1.5f, { 220,30,95,ga });
    } else if (hov) {
        DrawRectangleRoundedLines(r, 0.12f, 8, 0.8f, C_BORD_ACC);
    }
}

// Clickable circle button — returns true if clicked this frame
bool CircleBtn(Vector2 c, float r, const char* icon, bool active = false) {
    Vector2 m   = GetMousePosition();
    bool hov = CheckCollisionPointCircle(m, c, r);
    bool clk = hov && IsMouseButtonReleased(MOUSE_LEFT_BUTTON);
    if (clk) SpawnRipple(c.x, c.y, r * 2.2f);

    Color bg  = active ? C_ACCENT : (hov ? C_GLASS_HOV : C_GLASS);
    Color bdr = hov ? C_ACCENT2 : (active ? C_ACCENT : C_BORDER);
    DrawCircleV(c, r, bg);
    DrawCircleLinesV(c, r, bdr);

    int fs = (int)(r * 0.65f);
    if (fs < 10) fs = 10;
    int tw = MeasureText(icon, fs);
    DrawText(icon, (int)(c.x - tw / 2), (int)(c.y - fs / 2), fs, C_TXT1);
    return clk;
}

// Flat pill button for sidebar nav
bool FlatBtn(Rectangle r, const char* lbl, bool active = false) {
    Vector2 m   = GetMousePosition();
    bool hov = CheckCollisionPointRec(m, r);
    bool clk = hov && IsMouseButtonReleased(MOUSE_LEFT_BUTTON);
    if (clk) SpawnRipple(m.x, m.y, 60.0f);

    Color bg  = active ? C_ACCENT     : (hov ? C_GLASS_HOV : C_GLASS);
    Color bdr = active ? C_ACCENT2    : (hov ? C_BORD_ACC   : C_BORDER);
    Color tc  = (active || hov) ? C_TXT1 : C_TXT2;

    DrawRectangleRounded(r, 0.30f, 8, bg);
    DrawRectangleRoundedLines(r, 0.30f, 8, 0.8f, bdr);

    int fs = 14;
    int tw = MeasureText(lbl, fs);
    DrawText(lbl, (int)(r.x + r.width / 2 - tw / 2), (int)(r.y + r.height / 2 - fs / 2), fs, tc);
    return clk;
}

// ── Text clamping helpers ─────────────────────────────────────────────────
// Truncates string to fit within maxPx pixels at given font size.
// Appends ".." when truncation occurs.

std::string Clamp(const std::string& s, int fs, int maxPx) {
    if (MeasureText(s.c_str(), fs) <= maxPx) return s;
    std::string o = s;
    while (o.size() > 2 && MeasureText(o.c_str(), fs) > maxPx)
        o.resize(o.size() - 1);
    return o + "..";
}

std::string ClampBold(const std::string& s, int fs, int maxPx) {
    if (MeasureTextBold(s.c_str(), fs) <= maxPx) return s;
    std::string o = s;
    while (o.size() > 2 && MeasureTextBold(o.c_str(), fs) > maxPx)
        o.resize(o.size() - 1);
    return o + "..";
}

// ── Equalizer animation ──────────────────────────────────────────────────
void DrawEqualizer(float x, float y, float w, float h, bool playing) {
    const int bars = 12;
    float bw = w / (bars * 2.0f - 1.0f);
    for (int i = 0; i < bars; i++) {
        float bh = h * (playing ? eqBars[i] : 0.05f);
        unsigned char a = playing ? 200 : 50;
        DrawRectangleRounded({ x + i*(bw*2), y + h - bh, bw, bh }, 0.6f, 4, { 220,30,95,a });
    }
}

// ── Repeat / loop icon ────────────────────────────────────────────────────
// Draws the standard "two arrows in a rectangle loop" icon at centre c, radius r.
// Matches the reference image: rectangular path with arrowheads on each end.
void DrawLoopIcon(Vector2 c, float r, Color col) {
    float hw = r * 0.72f;   // half-width of the rect path
    float hh = r * 0.44f;   // half-height
    float aw = r * 0.28f;   // arrowhead size
    float thick = r * 0.18f;

    // Top horizontal bar
    DrawRectangle((int)(c.x - hw),     (int)(c.y - hh - thick * 0.5f), (int)(hw * 2), (int)thick, col);
    // Bottom horizontal bar
    DrawRectangle((int)(c.x - hw),     (int)(c.y + hh - thick * 0.5f), (int)(hw * 2), (int)thick, col);
    // Left vertical bar (connecting top-left to bottom-left)
    DrawRectangle((int)(c.x - hw),     (int)(c.y - hh), (int)thick, (int)(hh * 2), col);
    // Right vertical bar
    DrawRectangle((int)(c.x + hw - thick), (int)(c.y - hh), (int)thick, (int)(hh * 2), col);

    // Right arrowhead (top bar — points right)
    DrawTriangle(
        { c.x + hw - aw * 0.4f,  c.y - hh - aw * 0.8f },  // top tip
        { c.x + hw + aw * 0.6f,  c.y - hh + thick * 0.5f },// right tip
        { c.x + hw - aw * 0.4f,  c.y - hh + aw * 0.8f + thick }, // bottom
        col
    );

    // Left arrowhead (bottom bar — points left)
    DrawTriangle(
        { c.x - hw + aw * 0.4f,  c.y + hh - aw * 0.8f },
        { c.x - hw - aw * 0.6f,  c.y + hh + thick * 0.5f },
        { c.x - hw + aw * 0.4f,  c.y + hh + aw * 0.8f + thick },
        col
    );
}
