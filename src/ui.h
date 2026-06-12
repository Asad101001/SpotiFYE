#pragma once
#include <raylib.h>
#include <string>
#include <cstring>

// ─── THEME ────────────────────────────────────────────────────────────────────
const Color BG_TOP       = {  8, 10, 22, 255 };
const Color BG_BOT       = { 14, 30, 52, 255 };
const Color GLASS        = { 255, 255, 255,  14 };
const Color GLASS_HOV    = { 255, 255, 255,  30 };
const Color GLASS_SEL    = { 160,  60, 255,  45 };
const Color BORDER       = { 255, 255, 255,  28 };
const Color ACCENT       = { 175,  60, 255, 255 };
const Color ACCENT_DIM   = { 130,  40, 200, 255 };
const Color TXT_PRI      = WHITE;
const Color TXT_SEC      = { 180, 180, 205, 255 };
const Color TXT_DIM      = { 110, 110, 140, 255 };

// ─── LAYOUT ───────────────────────────────────────────────────────────────────
const int SW   = 1280;
const int SH   = 800;
const int PAD  = 14;
const int PLAYER_H = 110;

// Panels (computed once)
const Rectangle rSide    = { PAD,                     PAD,  220, SH - PLAYER_H - PAD*3 };
const Rectangle rMain    = { PAD + 220 + PAD,          PAD,  600, SH - PLAYER_H - PAD*3 };
const Rectangle rRight   = { PAD + 220 + PAD + 600 + PAD, PAD, SW - (PAD + 220 + PAD + 600 + PAD) - PAD, SH - PLAYER_H - PAD*3 };
const Rectangle rPlayer  = { PAD, SH - PLAYER_H - PAD, SW - PAD*2, PLAYER_H };

// ─── HELPERS ─────────────────────────────────────────────────────────────────

// Draw a frosted glass panel with optional title
void DrawPanel(Rectangle r, const char* title = nullptr) {
    DrawRectangleRounded(r, 0.04f, 12, GLASS);
    DrawRectangleRoundedLines(r, 0.04f, 12, 1.2f, BORDER);
    if (title) {
        DrawText(title, (int)r.x + 16, (int)r.y + 14, 18, TXT_PRI);
        DrawLineEx({r.x + 14, r.y + 42}, {r.x + r.width - 14, r.y + 42}, 0.8f, BORDER);
    }
}

// Draw a clickable pill button — returns true if clicked this frame
bool DrawButton(Rectangle r, const char* label, bool active = false) {
    Vector2 mouse = GetMousePosition();
    bool hovered = CheckCollisionPointRec(mouse, r);
    bool clicked = hovered && IsMouseButtonReleased(MOUSE_LEFT_BUTTON);

    Color bg = active ? GLASS_SEL : (hovered ? GLASS_HOV : GLASS);
    Color border = active ? ACCENT : BORDER;
    Color textCol = active ? TXT_PRI : (hovered ? TXT_PRI : TXT_SEC);

    DrawRectangleRounded(r, 0.5f, 12, bg);
    DrawRectangleRoundedLines(r, 0.5f, 12, 1.0f, border);
    int tw = MeasureText(label, 15);
    DrawText(label, (int)(r.x + r.width/2 - tw/2), (int)(r.y + r.height/2 - 8), 15, textCol);
    return clicked;
}

// Draw a single icon button (circle)
bool DrawIconBtn(Vector2 centre, float radius, const char* icon, bool active = false) {
    Vector2 mouse = GetMousePosition();
    bool hovered = CheckCollisionPointCircle(mouse, centre, radius);
    bool clicked = hovered && IsMouseButtonReleased(MOUSE_LEFT_BUTTON);

    Color bg = hovered ? GLASS_HOV : GLASS;
    Color border = active ? ACCENT : BORDER;
    DrawCircleV(centre, radius, bg);
    DrawCircleLinesV(centre, radius, border);
    int tw = MeasureText(icon, 16);
    DrawText(icon, (int)(centre.x - tw/2), (int)(centre.y - 8), 16, active ? ACCENT : TXT_PRI);
    return clicked;
}

// Clamp text so it doesn't overflow a pixel budget
const char* ClampText(const std::string& s, int fontSize, int maxWidth) {
    // Raylib has TextSubtext for this — we use the built-in
    static char buf[256];
    snprintf(buf, sizeof(buf), "%s", s.c_str());
    while (MeasureText(buf, fontSize) > maxWidth && strlen(buf) > 0) {
        buf[strlen(buf) - 1] = '\0';
    }
    if (strlen(buf) < s.size()) {
        // append "..."
        if (strlen(buf) >= 3) { buf[strlen(buf)-1]='.'; buf[strlen(buf)-1]='.'; }
    }
    return buf;
}
