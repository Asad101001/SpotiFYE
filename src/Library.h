#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include "Song.h"

class Library {
public:
    std::vector<Song*> allSongs;

    // A simple text parser that splits by the '|' character
    void loadFromCSV(const std::string& filename) {
        std::ifstream file(filename);
        std::string line;
        
        while (std::getline(file, line)) {
            if (line.empty() || line[0] == '#') continue; // Skip empty lines and comments
            
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
            
            allSongs.push_back(s);
        }
    }
};
