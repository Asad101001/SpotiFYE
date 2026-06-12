#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include "Song.h"

// STRICTLY NON-OOP (Procedural Array instead of vectors/classes)
const int MAX_SONGS = 100;
Song* librarySongs[MAX_SONGS];
int totalSongs = 0;

void loadLibrary(const std::string& filename) {
    std::ifstream file(filename);
    std::string line;
    totalSongs = 0;
    
    while (std::getline(file, line) && totalSongs < MAX_SONGS) {
        if (line.empty() || line[0] == '#') continue; 
        
        std::stringstream ss(line);
        std::string item;
        Song* s = new Song();
        
        std::getline(ss, s->title, '|');
        std::getline(ss, s->artist, '|');
        std::getline(ss, s->genre, '|');
        
        std::getline(ss, item, '|');
        s->duration = std::stoi(item);
        
        std::getline(ss, item, '|');
        s->playCount = std::stoi(item);
        
        std::getline(ss, item, '|');
        s->rating = std::stof(item);
        
        std::getline(ss, s->path, '|');
        std::getline(ss, s->coverPath, '|');
        
        // Strip carriage return if present (Windows text files)
        if (!s->coverPath.empty() && s->coverPath.back() == '\r') {
            s->coverPath.pop_back();
        }
        
        librarySongs[totalSongs] = s;
        totalSongs++;
    }
}
