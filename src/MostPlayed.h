#pragma once
#include "Song.h"

// ═══════════════════════════════════════════════════════════════════════════
//  MAX-HEAP for MOST PLAYED songs
//  Adapted from Week 12 Lab Task / heap.cpp
//  Stores Song* instead of int; compares by playCount (max-heap)
// ═══════════════════════════════════════════════════════════════════════════

const int HEAP_MAX = 100;
Song* heapArr[HEAP_MAX];
int   heapSize = 0;

void heapSwap(Song*& a, Song*& b) {
    Song* temp = a; a = b; b = temp;
}

void upHeapify(int i) {
    if (i == 0) return;
    int parent = (i - 1) / 2;
    if (heapArr[parent]->playCount < heapArr[i]->playCount) {
        heapSwap(heapArr[parent], heapArr[i]);
        upHeapify(parent);
    }
}

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

void heapInsert(Song* s) {
    if (!s || heapSize >= HEAP_MAX) return;
    heapArr[heapSize] = s;
    upHeapify(heapSize);
    heapSize++;
}

// Returns the top-N most played songs into outArr (out param)
// Does NOT destroy the heap
void heapTopN(Song** outArr, int n, int& outCount) {
    // Copy heap, extract max N times from the copy
    Song* tmp[HEAP_MAX];
    int   tmpSz = heapSize;
    for (int i = 0; i < heapSize; i++) tmp[i] = heapArr[i];

    outCount = 0;
    while (outCount < n && tmpSz > 0) {
        outArr[outCount++] = tmp[0];
        tmp[0] = tmp[tmpSz - 1];
        tmpSz--;
        // downHeapify on tmp copy
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
