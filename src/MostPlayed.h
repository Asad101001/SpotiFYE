#pragma once
#include "Song.h"

// ═══════════════════════════════════════════════════════════════════════════
//  MAX-HEAP  (Week 12 / heap.cpp adapted)
//  Stores Song* and orders by playCount (max at root)
// ═══════════════════════════════════════════════════════════════════════════

const int HEAP_MAX = 200;
Song* heapArr[HEAP_MAX];
int   heapSize = 0;

void heapSwap(Song*& a, Song*& b) { Song* t=a; a=b; b=t; }

void downHeapify(int i) {
    int l = 2*i+1, r = 2*i+2, lg = i;
    if (l < heapSize && heapArr[l]->playCount > heapArr[lg]->playCount) lg=l;
    if (r < heapSize && heapArr[r]->playCount > heapArr[lg]->playCount) lg=r;
    if (lg != i) { heapSwap(heapArr[i], heapArr[lg]); downHeapify(lg); }
}

void upHeapify(int i) {
    if (i==0) return;
    int p=(i-1)/2;
    if (heapArr[p]->playCount < heapArr[i]->playCount) {
        heapSwap(heapArr[p], heapArr[i]);
        upHeapify(p);
    }
}

void heapInsert(Song* s) {
    if (!s || heapSize >= HEAP_MAX) return;
    heapArr[heapSize] = s;
    upHeapify(heapSize);
    heapSize++;
}

// Call when a song's playCount changes — rebuilds in O(n)
void heapRebuild() {
    for (int i = heapSize/2 - 1; i >= 0; i--)
        downHeapify(i);
}

// Non-destructive top-N extraction into outArr
void heapTopN(Song** outArr, int n, int& outCount) {
    Song* tmp[HEAP_MAX];
    int   tmpSz = heapSize;
    for (int i = 0; i < heapSize; i++) tmp[i] = heapArr[i];

    outCount = 0;
    while (outCount < n && tmpSz > 0) {
        outArr[outCount++] = tmp[0];
        tmp[0] = tmp[--tmpSz];
        int i=0;
        while (true) {
            int l=2*i+1, r=2*i+2, lg=i;
            if (l<tmpSz && tmp[l]->playCount>tmp[lg]->playCount) lg=l;
            if (r<tmpSz && tmp[r]->playCount>tmp[lg]->playCount) lg=r;
            if (lg==i) break;
            Song* t=tmp[i]; tmp[i]=tmp[lg]; tmp[lg]=t;
            i=lg;
        }
    }
}
