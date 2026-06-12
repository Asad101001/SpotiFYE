#include <iostream>
#include <stdlib.h>
using namespace std;

struct Node {
    int data;
    Node* next;
};

Node* list = NULL;

void insert(int value) {
    Node* temp = new Node();
    temp->data = value;

    if (list == NULL) {
        list = temp;
        temp->next = list;
        return;
    }

    Node* cur = list;
    while (cur->next != list)
        cur = cur->next;

    cur->next = temp;
    temp->next = list;
}

void display() {
    if (list == NULL) {
        cout << "List is empty" << endl;
        return;
    }

    Node* cur = list;
    do {
        cout << cur->data << "  ";
        cur = cur->next;
    } while (cur != list);

    cout << "(head)" << endl;
}

void search(int value) {
    if (list == NULL) {
        cout << "List is empty" << endl;
        return;
    }

    Node* cur = list;
    int pos = 1;

    do {
        if (cur->data == value) {
            cout << "Found at position " << pos << endl;
            return;
        }
        cur = cur->next;
        pos++;
    } while (cur != list);

    cout << "Not found" << endl;
}

void deleteElement(int value) {
    if (list == NULL) {
        cout << "List is empty" << endl;
        return;
    }

    Node* curr = list;
    Node* prev = NULL;

    if (list->data == value && list->next == list) {
        delete list;
        list = NULL;
        return;
    }

    if (list->data == value) {
        while (curr->next != list)
            curr = curr->next;

        Node* temp = list;
        curr->next = list->next;
        list = list->next;
        delete temp;
        return;
    }

    prev = list;
    curr = list->next;

    while (curr != list) {
        if (curr->data == value) {
            prev->next = curr->next;
            delete curr;
            return;
        }
        prev = curr;
        curr = curr->next;
    }

    cout << "Value not found" << endl;
}

int main() {
    int choice, val;

    while (true) {
        cout << "\n1- Insert\n2- Display\n3- Delete\n4- Search\n5- Exit\n";
        cin >> choice;

        if (choice == 1) {
            cout << "Enter the number to insert in the list" << endl;
            cin >> val;
            insert(val);
        }
        else if (choice == 2) {
            display();
        }
        else if (choice == 3) {
            cout << "Enter the number to delete from the list" << endl;
            cin >> val;
            deleteElement(val);
        }
        else if (choice == 4) {
            cout << "Enter the number to search for in the list" << endl;
            cin >> val;
            search(val);
        }
        else if (choice == 5) {
            break;
        }
        else {
            cout << "Invalid choice! Try again." << endl;
        }
    }
}
