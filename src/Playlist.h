#pragma once
#include "Song.h"

// ═══════════════════════════════════════════════════════════════════════════
//  DOUBLY CIRCULAR LINKED LIST  —  SpotiFYE Playlist
//  Adapted from: Week 3 Lab Task / doublyLinkedList.cpp
//  Each Node wraps a Song pointer; prev/next enable bidirectional traversal.
//  When repeatAll=true the list behaves as circular (tail->next = head).
// ═══════════════════════════════════════════════════════════════════════════

struct Node {
    Song* song;
    Node* next;
    Node* prev;
};

Node* playlistHead = nullptr;
Node* playlistTail = nullptr;   // tail pointer for O(1) append
Node* currentSong  = nullptr;

bool  repeatAll    = false;     // circular repeat mode

// ── Insert at tail (O(1)) ─────────────────────────────────────────────────
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

    playlistTail->next = temp;
    temp->prev         = playlistTail;
    playlistTail       = temp;

    // Keep circular link consistent
    if (repeatAll) playlistTail->next = playlistHead;
}

// ── Navigate forward (wraps if repeatAll) ────────────────────────────────
void nextSong() {
    if (!currentSong) return;
    if (currentSong->next && currentSong->next != playlistHead) {
        currentSong = currentSong->next;
    } else if (repeatAll) {
        currentSong = playlistHead;  // circular wrap
    }
}

// ── Navigate backward (wraps if repeatAll) ───────────────────────────────
void prevSong() {
    if (!currentSong) return;
    if (currentSong->prev) {
        currentSong = currentSong->prev;
    } else if (repeatAll) {
        currentSong = playlistTail;  // circular wrap backward
    }
}

// ── Toggle circular repeat on/off ────────────────────────────────────────
void toggleRepeat() {
    repeatAll = !repeatAll;
    if (repeatAll && playlistTail) {
        playlistTail->next = playlistHead;   // close the circle
    } else if (playlistTail) {
        playlistTail->next = nullptr;        // open the circle
    }
}

// ── Jump directly to a given node ────────────────────────────────────────
void jumpToNode(Node* target) {
    if (target) currentSong = target;
}

// ── Free all nodes ───────────────────────────────────────────────────────
void cleanupPlaylist() {
    // Open any circular link first to avoid infinite loop
    if (repeatAll && playlistTail) playlistTail->next = nullptr;
    Node* cur = playlistHead;
    while (cur) {
        Node* del = cur;
        cur = cur->next;
        delete del;
    }
    playlistHead = playlistTail = currentSong = nullptr;
}
