#pragma once
#include <string>
#include <raylib.h>

// ═══════════════════════════════════════════════════════════════════════════
//  SONG  —  Plain data record for one track
//  playCount is session-only (reset to 0 on load, never written back to disk)
// ═══════════════════════════════════════════════════════════════════════════
struct Song {
    std::string title;
    std::string artist;
    std::string genre;
    int         duration;       // seconds
    int         playCount;      // session plays only (starts at 0)
    float       rating;
    std::string path;
    std::string coverPath;
    Texture2D   coverTexture;   // GPU texture, freed in cleanupLibrary()
};
