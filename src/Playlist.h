#pragma once
#include "Song.h"

// ═══════════════════════════════════════════════════════════════════════════
//  DOUBLY LINKED LIST PLAYLIST
//  Adapted from Week 3 Lab Task / doublyLinkedList.cpp
// ═══════════════════════════════════════════════════════════════════════════

struct Node {
    Song* song;
    Node* next;
    Node* prev;
};

Node* playlistHead = nullptr;
Node* playlistTail = nullptr;  // track tail for O(1) insert
Node* currentSong  = nullptr;

bool  repeatAll    = false;    // circular repeat mode (Circular LL behaviour)

void insertIntoPlaylist(Song* newSong) {
    Node* temp = new Node();
    temp->song = newSong;
    temp->next = nullptr;
    temp->prev = nullptr;

    if (playlistHead == nullptr) {
        playlistHead = temp;
        playlistTail = temp;
        currentSong  = temp;
        return;
    }

    // Append to tail (O(1) with tail pointer)
    playlistTail->next = temp;
    temp->prev         = playlistTail;
    playlistTail       = temp;

    // If repeatAll: keep tail pointing back to head (circular)
    if (repeatAll) playlistTail->next = playlistHead;
}

// Advance forward — wraps around if repeatAll
void nextSong() {
    if (!currentSong) return;
    if (currentSong->next && currentSong->next != playlistHead) {
        currentSong = currentSong->next;
    } else if (repeatAll) {
        currentSong = playlistHead;   // circular wrap
    }
}

// Step backward
void prevSong() {
    if (!currentSong) return;
    if (currentSong->prev) {
        currentSong = currentSong->prev;
    } else if (repeatAll) {
        currentSong = playlistTail;   // circular wrap backward
    }
}

void toggleRepeat() {
    repeatAll = !repeatAll;
    // Update circular link
    if (repeatAll && playlistTail) {
        playlistTail->next = playlistHead;
    } else if (playlistTail) {
        playlistTail->next = nullptr;
    }
}

// Jump directly to a specific node
void jumpToNode(Node* target) {
    if (target) currentSong = target;
}
