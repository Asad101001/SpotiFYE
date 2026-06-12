#include <iostream>
using namespace std;

struct Node{
    int data;
    Node *next;
};

Node *front = NULL;
Node *rear = NULL;

void enQueue(int val){

    Node *temp = new Node();
    temp->data = val;
    temp->next = NULL;

    if (front == NULL){
        front = rear = temp;
    }
    else{
        rear->next = temp;
        rear = temp;
    }
}

void deQueue(){

    if (front == NULL){
        cout << "Queue is empty" << endl;
        return;
    }
    Node *temp = front;
    front = front->next;

    if (temp == NULL){
        rear = NULL;
    }
    delete temp;
}

void display(){
    if (front == NULL) {
        cout << "Queue is empty" << endl;
        return;
    }
    Node *temp = front;
    while (temp!=NULL)
    {
        cout << temp->data << " ";
        temp = temp->next;
    }

    cout << endl;
}

int main() {

    //Added elements to rear
    enQueue(10);
    enQueue(23);
    enQueue(44);
    display();

    //Removed (dequeued) element from front
    deQueue();
    display();

    //Emptyin' the queue
    while (front != NULL) {
        deQueue();
    }
    display();

    return 0;
}