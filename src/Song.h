#pragma once
#include <string>

struct Song {
    std::string title;
    std::string artist;
    std::string genre;
    int duration; // in seconds
    int playCount;
    float rating;
    std::string path;
    std::string coverPath;
};
