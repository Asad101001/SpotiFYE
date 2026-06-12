#include <iostream>
using namespace std;

int stack[10];
int top = -1;

void push(int val){
    if(top==9){
        cout << "Stack overflow\n";
        return;
    }
    top++;
    stack[top] = val;
    
}

void pop(){
    if(top==-1){
        cout << "Stack underflow\n";
        return;
    }
    cout << "Element popped: "<<stack[top]<<endl;
    top--;
}

void display(){

    if (top==-1){
        cout << "Stack underflow"<<endl;
        return;
    }
    for (int i = top; i >= 0;i--){
        cout << stack[i]<<" ";
    }
    cout << endl;
}

int main(){

    cout << "11,22,33 pushed to stack"<<endl;
    push(11);
    push(22);
    push(33);
    display();

    cout << "Last pushed element popped (LIFO)"<<endl;
    pop();
    display();

    cout<<"44 pushed to top"<<endl;
    push(44);
    display();

    return 0;
}