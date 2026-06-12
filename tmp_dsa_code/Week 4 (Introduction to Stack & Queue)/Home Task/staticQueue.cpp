#include <iostream>
using namespace std;

const int SIZE = 5;
int queue[SIZE];
int front = 0;
int rear = -1;
int count = 0;

void enQueue(int value){
    if (count == SIZE){
        cout << "Queue is full!\n" << endl;
        return;
    }
    rear = (rear + 1) % SIZE;
    queue[rear] = value;
    count++;
}

void deQueue(){
    if (count == 0){
        cout << "Queue is empty" << endl;
        return;
    }
    int value = queue[front];
    front = (front + 1) % SIZE;
    count--;

    cout << "Dequeued: " << value << endl;  
}

void display(){
    if (count ==0){
        cout << "Queue is empty" << endl;
        return;
    }

    int index = front;
    for (int i = 0; i < count;i++){
        cout << queue[index] << " ";

        index = (index + 1) % SIZE;
    }
    cout << endl;
}

int main() {

    enQueue(10);
    enQueue(20);
    enQueue(30);
    enQueue(40);
    enQueue(50);
    display();

    
    enQueue(60);
    
    deQueue();
    deQueue();
    display();
    

    enQueue(70);
    enQueue(80);
    display();
    
    while (count > 0) {
        deQueue();
    }
    display();
    
    return 0;
}


