#include <iostream>
#include <stdlib.h>
using namespace std;

struct Node {
    int data;
    Node* next;
    Node* prev;
};

Node* list = NULL;

void insert(int value) {
    Node* temp = new Node();
    temp->data = value;
    temp->next = NULL;
    temp->prev = NULL;

    if (list == NULL) {
        list = temp;
        return;
    }

    Node* cur = list;
    while (cur->next != NULL)
        cur = cur->next;

    cur->next = temp;
    temp->prev = cur;
}

void display() {
    Node* cur = list;
    while (cur != NULL) {
        cout << cur->data << " -> ";
        cur = cur->next;
    }
    cout << "NULL" << endl;
}

int main() {
    insert(10);
    insert(20);
    insert(30);
    display();
    return 0;
}
