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
Node* playlistHead = NULL;
Node* currentSong = NULL;

void insertIntoPlaylist(Song* newSong) {
    Node* temp = new Node();
    temp->song = newSong;
    temp->next = NULL;
    temp->prev = NULL;

    if (playlistHead == NULL) {
        playlistHead = temp;
        currentSong = playlistHead; // Default current to first song added
        return;
    }

    Node* cur = playlistHead;
    while (cur->next != NULL) {
        cur = cur->next;
    }

    cur->next = temp;
    temp->prev = cur;
}

void nextSong() {
    if (currentSong != NULL && currentSong->next != NULL) {
        currentSong = currentSong->next;
    }
}

void prevSong() {
    if (currentSong != NULL && currentSong->prev != NULL) {
        currentSong = currentSong->prev;
    }
}
