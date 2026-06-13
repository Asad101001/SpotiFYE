#pragma once
#include "Song.h"
#include <cstdlib>

// ═══════════════════════════════════════════════════════════════════════════
//  HISTORY STACK  —  Playback history (most recent first)
//  Adapted from: Week 4 Lab Task / dynamicStack.cpp
//  push() uses new Node(), matching the lab's dynamic allocation style.
//  Capped at HISTORY_LIMIT entries to prevent unbounded growth.
// ═══════════════════════════════════════════════════════════════════════════

struct StackNode {
    Song*      song;
    StackNode* next;
};

StackNode* historyTop  = nullptr;
int        historySize = 0;
const int  HISTORY_LIMIT = 20;

// ── Push most-recently-played song onto stack ─────────────────────────────
void historyPush(Song* s) {
    if (!s) return;

    StackNode* temp = new StackNode();
    temp->song = s;
    temp->next = historyTop;
    historyTop = temp;
    historySize++;

    // Trim oldest entry if over the limit
    if (historySize > HISTORY_LIMIT) {
        StackNode* cur = historyTop;
        for (int i = 1; i < HISTORY_LIMIT - 1; i++) cur = cur->next;
        StackNode* tail = cur->next;
        cur->next = nullptr;
        while (tail) {
            StackNode* del = tail;
            tail = tail->next;
            delete del;
        }
        historySize = HISTORY_LIMIT;
    }
}

// ── Peek at top without popping ──────────────────────────────────────────
Song* historyPeek() {
    return historyTop ? historyTop->song : nullptr;
}

// ── Free all stack nodes ─────────────────────────────────────────────────
void cleanupHistory() {
    while (historyTop) {
        StackNode* del = historyTop;
        historyTop = historyTop->next;
        delete del;
    }
    historySize = 0;
}
