#include <iostream>
#include <stdlib.h>
using namespace std;

struct Node {
    int data;
    Node* next;
};

Node* top = NULL;

void push(int value) {
    Node* temp = (Node*)malloc(sizeof(Node));
    temp->data = value;
    temp->next = top;
    top = temp;
}

void pop() {
    if (top == NULL) {
        cout << "Stack underflow";
        return;
    }
    Node* temp = top;
    cout << top->data;
    cout << " has been popped";
    top = top->next;
    free(temp);
}

int main() {
    int choice, value;

    while (choice != 3){
        cout << "\n1. Push";
        cout << "\n2. Pop";
        cout << "\n3. Exit";
        cout << "\nEnter your choice: ";
        cin >> choice;
    
        if (choice==1){
            cout << "Enter value"<<endl;
            cin >> value;
            push(value);   
        }
        if (choice==2){
            pop();
        }
        if (choice==3){
            break;
        }

    }

    return 0;
}