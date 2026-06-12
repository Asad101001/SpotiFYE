#pragma once
#include "Song.h"
#include <cstdlib>

// ═══════════════════════════════════════════════════════════════════════════
//  HISTORY STACK  (adapted from Week 4 Lab Task / dynamicStack.cpp)
//  Stores pointer to Song* so we can display "last played" history
// ═══════════════════════════════════════════════════════════════════════════

struct StackNode {
    Song*      song;
    StackNode* next;
};

StackNode* historyTop = nullptr;
int        historySize = 0;
const int  HISTORY_LIMIT = 20; // cap so it doesn't grow forever

void historyPush(Song* s) {
    if (!s) return;
    StackNode* temp = new StackNode();
    temp->song = s;
    temp->next = historyTop;
    historyTop = temp;
    historySize++;

    // Trim oldest entries beyond limit
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

// Peek without popping
Song* historyPeek() {
    return historyTop ? historyTop->song : nullptr;
}
