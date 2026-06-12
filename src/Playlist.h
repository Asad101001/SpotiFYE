#pragma once
#include "Song.h"

// Adapted directly from DSA_Labs/Week 3/Lab Task/doublyLinkedList.cpp
// STRICTLY NON-OOP (Procedural)
struct Node {
    Song* song;
    Node* next;
    Node* prev;
};

// Global pointers just like the lab tasks
Node* playlistHead = nullptr;
Node* currentSong = nullptr;

void insertIntoPlaylist(Song* newSong) {
    Node* temp = new Node();
    temp->song = newSong;
    temp->next = nullptr;
    temp->prev = nullptr;

    if (playlistHead == nullptr) {
        playlistHead = temp;
        currentSong = playlistHead; // Default current to first song added
        return;
    }

    Node* cur = playlistHead;
    while (cur->next != nullptr) {
        cur = cur->next;
    }

    cur->next = temp;
    temp->prev = cur;
}

void nextSong() {
    if (currentSong != nullptr && currentSong->next != nullptr) {
        currentSong = currentSong->next;
    }
}

void prevSong() {
    if (currentSong != nullptr && currentSong->prev != nullptr) {
        currentSong = currentSong->prev;
    }
}
