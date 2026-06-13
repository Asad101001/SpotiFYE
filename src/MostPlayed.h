#pragma once
#include "Song.h"

// ═══════════════════════════════════════════════════════════════════════════
//  MAX-HEAP  —  Most-Played tracker (orders songs by session playCount)
//  Adapted from: Week 12 Lab Task / heap.cpp
//  Uses a flat Song* array identical to the lab's int arr[] approach.
//  upHeapify / downHeapify match the exact lab naming and structure.
// ═══════════════════════════════════════════════════════════════════════════

const int HEAP_MAX  = 1000;
Song*     heapArr[HEAP_MAX];
int       heapSize  = 0;

// ── Swap two Song pointers ────────────────────────────────────────────────
void heapSwap(Song*& a, Song*& b) {
    Song* temp = a;
    a = b;
    b = temp;
}

// ── Bubble up: restore heap property after insert ────────────────────────
void upHeapify(int i) {
    if (i == 0) return;
    int parent = (i - 1) / 2;
    if (heapArr[parent]->playCount < heapArr[i]->playCount) {
        heapSwap(heapArr[parent], heapArr[i]);
        upHeapify(parent);
    }
}

// ── Bubble down: restore heap property after removal/rebuild ─────────────
void downHeapify(int i) {
    int left    = 2 * i + 1;
    int right   = 2 * i + 2;
    int largest = i;

    if (left  < heapSize && heapArr[left]->playCount  > heapArr[largest]->playCount) largest = left;
    if (right < heapSize && heapArr[right]->playCount > heapArr[largest]->playCount) largest = right;

    if (largest != i) {
        heapSwap(heapArr[i], heapArr[largest]);
        downHeapify(largest);
    }
}

// ── Insert a song into the heap ──────────────────────────────────────────
void heapInsert(Song* s) {
    if (!s || heapSize >= HEAP_MAX) return;
    heapArr[heapSize] = s;
    upHeapify(heapSize);
    heapSize++;
}

// ── Rebuild heap after any playCount change (O(n)) ───────────────────────
// Called every time a song is played so Most Played view stays accurate
void heapRebuild() {
    for (int i = heapSize / 2 - 1; i >= 0; i--)
        downHeapify(i);
}

// ── Non-destructive top-N extraction ─────────────────────────────────────
// Copies heapArr into a temp buffer and extracts without modifying the real heap
void heapTopN(Song** outArr, int n, int& outCount) {
    Song* tmp[HEAP_MAX];
    int   tmpSz = heapSize;
    for (int i = 0; i < heapSize; i++) tmp[i] = heapArr[i];

    outCount = 0;
    while (outCount < n && tmpSz > 0) {
        outArr[outCount++] = tmp[0];
        tmp[0] = tmp[--tmpSz];  // replace root with last element

        // Sink down in the temporary copy
        int i = 0;
        while (true) {
            int l = 2*i+1, r = 2*i+2, lg = i;
            if (l < tmpSz && tmp[l]->playCount > tmp[lg]->playCount) lg = l;
            if (r < tmpSz && tmp[r]->playCount > tmp[lg]->playCount) lg = r;
            if (lg == i) break;
            Song* t = tmp[i]; tmp[i] = tmp[lg]; tmp[lg] = t;
            i = lg;
        }
    }
}
