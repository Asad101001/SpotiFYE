#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <map>
#include "Song.h"

// ═══════════════════════════════════════════════════════════════════════════
//  LIBRARY  —  Procedural flat array of Song pointers
//  Pattern: global array + size counter (matches lab Week 1 / arrays pattern)
// ═══════════════════════════════════════════════════════════════════════════

const int  MAX_SONGS = 1000;
Song*      librarySongs[MAX_SONGS];
int        totalSongs = 0;

// Cover texture cache: path -> Texture2D  (avoids loading same image twice)
std::map<std::string, Texture2D> loadedCovers;

// ── Parse library.txt ────────────────────────────────────────────────────
// Format: title|artist|genre|duration_sec|playCount|rating|path|coverPath
// NOTE: playCount from file is IGNORED — starts at 0 for every session
void loadLibrary(const std::string& filename) {
    std::ifstream file(filename);
    std::string   line;
    totalSongs = 0;

    while (std::getline(file, line) && totalSongs < MAX_SONGS) {
        if (line.empty() || line[0] == '#') continue;

        std::stringstream ss(line);
        std::string       item;
        Song* s = new Song();

        std::getline(ss, s->title,  '|');
        std::getline(ss, s->artist, '|');
        std::getline(ss, s->genre,  '|');

        std::getline(ss, item, '|');
        s->duration = std::stoi(item);

        // Read but DISCARD the stored playCount — always start fresh
        std::getline(ss, item, '|');
        s->playCount = 0;  // session-only counter

        std::getline(ss, item, '|');
        s->rating = std::stof(item);

        std::getline(ss, s->path,      '|');
        std::getline(ss, s->coverPath, '|');

        // Strip Windows carriage return (\r)
        if (!s->path.empty()      && s->path.back()      == '\r') s->path.pop_back();
        if (!s->coverPath.empty() && s->coverPath.back() == '\r') s->coverPath.pop_back();

        // Load cover texture — cache by path so shared album art loads once
        if (!s->coverPath.empty()) {
            if (loadedCovers.find(s->coverPath) == loadedCovers.end()) {
                loadedCovers[s->coverPath] = LoadTexture(s->coverPath.c_str());
            }
            s->coverTexture = loadedCovers[s->coverPath];
        } else {
            s->coverTexture = {0};
        }

        librarySongs[totalSongs] = s;
        totalSongs++;
    }
}

// ── Free GPU memory for all loaded textures ──────────────────────────────
void cleanupLibrary() {
    for (auto const& [key, val] : loadedCovers) {
        if (val.id > 0) UnloadTexture(val);
    }
    loadedCovers.clear();

    // Free heap-allocated Song objects
    for (int i = 0; i < totalSongs; i++) {
        delete librarySongs[i];
        librarySongs[i] = nullptr;
    }
    totalSongs = 0;
}
