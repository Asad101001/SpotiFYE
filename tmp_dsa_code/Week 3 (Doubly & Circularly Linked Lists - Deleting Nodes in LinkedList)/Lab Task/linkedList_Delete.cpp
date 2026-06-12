#include <iostream>
#include <stdlib.h>
using namespace std;

struct Node {
    int data;
    Node* next;
};

Node* list = NULL;

void insert(int value) {
    Node* temp = new Node();  //only writing Node* temp; -> works in C not C++
    // temp = (Node *)malloc(sizeof(Node)); //malloc -> allocates memory & sizeof defines size in bits of memory to be allocated : Node
    temp->data = value;
    temp->next = NULL;

    if (list == NULL) {
        list = temp;
    } else {
        Node* cur = list;
        while (cur->next != NULL) {
            cur = cur->next;
        }
        cur->next = temp;
    }
}

void display() {
    Node* cur = list;
    while (cur != NULL) {
        cout << cur->data << " -> ";
        cur = cur->next;
    }
    cout << "NULL" << endl;
}

void deleteElement(int value){
    if (list==NULL)
    {
        cout << "List is empty/NULL";
    }
    Node* curr = list;
    if (value==list->data)
    {
        list = list->next;
        free(curr);
        return;
    }
    curr = list->next;
    Node* prev = list;

    while (curr!=NULL){
        if (value==curr->data)
        {
            prev->next = curr->next;
            free(curr);
            return;
        }
        curr = curr->next;
        prev = prev->next;
    }
}

int main() {
    int choice, val;

    while (true) {
        cout << "\n1- Insert\n2- Display\n3- Delete\n4- Exit\n";
        cin >> choice;

        if (choice == 1) {
            cout << "Enter value: ";
            cin >> val;
            insert(val);
        }
        else if (choice == 2) {
            display();
        }
        else if (choice == 3) {
            cout << "Enter the value to delete";
            cin >> val;
            deleteElement(val);
        }
        else if (choice == 4){
            break;
        }
        else {
            cout << "Invalid choice" << endl;
        }
    }

    return 0;
}