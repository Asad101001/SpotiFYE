#pragma once
#include "Song.h"
#include <iostream>

// Adapted directly from DSA_Labs/Week 3/Lab Task/doublyLinkedList.cpp
struct Node {
    Song* song;
    Node* next;
    Node* prev;
};

class Playlist {
public:
    Node* head;
    Node* current; // Pointer to the currently playing song

    Playlist() {
        head = nullptr;
        current = nullptr;
    }

    void insert(Song* newSong) {
        Node* temp = new Node();
        temp->song = newSong;
        temp->next = nullptr;
        temp->prev = nullptr;

        if (head == nullptr) {
            head = temp;
            current = head; // Default current to first song added
            return;
        }

        Node* cur = head;
        while (cur->next != nullptr) {
            cur = cur->next;
        }

        cur->next = temp;
        temp->prev = cur;
    }

    void nextSong() {
        if (current != nullptr && current->next != nullptr) {
            current = current->next;
        }
    }

    void prevSong() {
        if (current != nullptr && current->prev != nullptr) {
            current = current->prev;
        }
    }

    void displayConsole() {
        Node* cur = head;
        while (cur != nullptr) {
            std::cout << cur->song->title << " -> ";
            cur = cur->next;
        }
        std::cout << "NULL" << std::endl;
    }
};
