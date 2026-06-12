#pragma once
#include <raylib.h>
#include <string>
#include <cstring>
#include <cmath>

// ─── SPOTIFYE AMOLED THEME ───────────────────────────────────────────────────
//  Pure black base + hot-pink/magenta accent (portfolio-inspired)
const Color C_BG         = {  0,  0,  0, 255 };
const Color C_SURFACE    = {  8,  8,  8, 255 };   // glass card base
const Color C_SURFACE2   = { 14, 14, 14, 255 };
const Color C_GLASS      = { 255, 255, 255,  8 };
const Color C_GLASS_HOV  = { 255, 255, 255, 18 };
const Color C_GLASS_SEL  = { 220,  30, 100, 50 };
const Color C_BORDER     = { 255, 255, 255, 20 };
const Color C_BORDER_ACC = { 220,  30, 100, 180 };
const Color C_ACCENT     = { 220,  30, 100, 255 };  // hot pink
const Color C_ACCENT2    = { 255,  60, 130, 255 };  // lighter pink
const Color C_ACCENT_DIM = { 130,  15,  55, 255 };
const Color C_WHITE      = WHITE;
const Color C_TXT1       = WHITE;
const Color C_TXT2       = { 190, 190, 200, 255 };
const Color C_TXT3       = {  90,  90, 100, 255 };

// ─── LAYOUT ──────────────────────────────────────────────────────────────────
const int SW       = 1280;
const int SH       = 800;
const int PAD      = 12;
const int SIDE_W   = 210;
const int RIGHT_W  = 230;
const int PLAYER_H = 100;
const int TOP_H    = SH - PLAYER_H - PAD * 3;

const Rectangle rSide   = { (float)PAD,                           (float)PAD,          (float)SIDE_W,  (float)TOP_H };
const Rectangle rMain   = { (float)(PAD + SIDE_W + PAD),          (float)PAD,          (float)(SW - PAD*4 - SIDE_W - RIGHT_W), (float)TOP_H };
const Rectangle rRight  = { (float)(SW - PAD - RIGHT_W),          (float)PAD,          (float)RIGHT_W, (float)TOP_H };
const Rectangle rPlayer = { (float)PAD, (float)(SH - PLAYER_H - PAD), (float)(SW - PAD*2), (float)PLAYER_H };

// ─── ANIMATION STATE ─────────────────────────────────────────────────────────
float glowPulse = 0.0f;  // 0..1, driven by time, used for glow effects

// ─── HELPERS ─────────────────────────────────────────────────────────────────

void UpdateAnimations() {
    glowPulse = (float)(0.5 + 0.5 * sin(GetTime() * 2.2));
}

// Glass card with pink glow border when active
void DrawCard(Rectangle r, bool active = false, bool hovered = false) {
    Color fill = active ? C_GLASS_SEL : (hovered ? C_GLASS_HOV : C_GLASS);
    DrawRectangleRounded(r, 0.12f, 10, fill);
    if (active) {
        // animated glow border
        Color glowCol = { 220, 30, 100, (unsigned char)(120 + (int)(80 * glowPulse)) };
        DrawRectangleRoundedLines(r, 0.12f, 10, 1.5f, glowCol);
    } else {
        DrawRectangleRoundedLines(r, 0.12f, 10, 0.8f, C_BORDER);
    }
}

// Frosted panel (entire section)
void DrawPanel(Rectangle r) {
    DrawRectangleRec(r, C_SURFACE);
    DrawRectangleRoundedLines(r, 0.03f, 8, 1.0f, C_BORDER);
}

// Section header with divider line
void DrawSectionHeader(float x, float y, float w, const char* label) {
    DrawText(label, (int)x, (int)y, 13, C_TXT3);
    DrawLineEx({x, y + 18}, {x + w, y + 18}, 0.7f, C_BORDER);
}

// Icon-style circle button, returns true if clicked
bool DrawCircleBtn(Vector2 c, float r, const char* icon, bool active = false) {
    Vector2 mouse = GetMousePosition();
    bool hov  = CheckCollisionPointCircle(mouse, c, r);
    bool clk  = hov && IsMouseButtonReleased(MOUSE_LEFT_BUTTON);

    Color bgFill = active ? C_ACCENT : (hov ? C_GLASS_HOV : C_GLASS);
    Color border = active ? C_ACCENT2 : (hov ? C_BORDER_ACC : C_BORDER);
    DrawCircleV(c, r, bgFill);
    DrawCircleLinesV(c, r, border);
    int tw = MeasureText(icon, 14);
    DrawText(icon, (int)(c.x - tw/2), (int)(c.y - 7), 14, active ? C_WHITE : C_TXT1);
    return clk;
}

// Flat text button, returns true if clicked
bool DrawFlatBtn(Rectangle r, const char* label, bool active = false) {
    Vector2 mouse = GetMousePosition();
    bool hov = CheckCollisionPointRec(mouse, r);
    bool clk = hov && IsMouseButtonReleased(MOUSE_LEFT_BUTTON);

    Color bg  = active ? C_ACCENT : (hov ? C_GLASS_HOV : C_GLASS);
    Color bdr = active ? C_ACCENT2 : C_BORDER;
    Color txt = active ? C_WHITE : (hov ? C_TXT1 : C_TXT2);

    DrawRectangleRounded(r, 0.25f, 8, bg);
    DrawRectangleRoundedLines(r, 0.25f, 8, 0.8f, bdr);
    int tw = MeasureText(label, 13);
    DrawText(label, (int)(r.x + r.width/2 - tw/2), (int)(r.y + r.height/2 - 7), 13, txt);
    return clk;
}

// Clamp string to pixel width, appending ".." if needed
std::string ClampStr(const std::string& s, int fontSize, int maxPx) {
    if (MeasureText(s.c_str(), fontSize) <= maxPx) return s;
    std::string out = s;
    while (out.size() > 2 && MeasureText(out.c_str(), fontSize) > maxPx)
        out.resize(out.size() - 1);
    return out + "..";
}
