#include <iostream>
using namespace std;

struct Node {
    int data;
    Node* left;
    Node* right;
};

Node* Root = NULL;

Node* insertElement(Node* curr, int val) {
    if (curr == NULL) {
        Node* temp = new Node();
        temp->data = val;
        temp->left = NULL;
        temp->right = NULL;
        return temp;
    }

    if (val > curr->data)
        curr->right = insertElement(curr->right, val);
    else
        curr->left = insertElement(curr->left, val);

    return curr;
}

void PreOrder(Node* curr) {
    if (curr) {
        cout << curr->data << " ";
        PreOrder(curr->left);
        PreOrder(curr->right);
    }
}

void PostOrder(Node* curr) {
    if (curr) {
        PostOrder(curr->left);
        PostOrder(curr->right);
        cout << curr->data << " ";
    }
}

void InOrder(Node* curr) {
    if (curr) {
        InOrder(curr->left);
        cout << curr->data << " ";
        InOrder(curr->right);
    }
}

Node* minNode(Node* curr) {
    Node* temp = curr;
    while (temp->left != NULL) {
        temp = temp->left;
    }
    return temp;
}


Node* deleteNode(Node* curr, int val) {
    if (curr == NULL) {
        cout << "Value not found\n";
        return NULL;
    }

    if (val < curr->data) {
        curr->left = deleteNode(curr->left, val);
    }
    else if (val > curr->data) {
        curr->right = deleteNode(curr->right, val);
    }
    else {
        
        //0 child
        if (curr->left == NULL && curr->right == NULL) {
            delete curr;
            return NULL;
        }

        //1 child on right
        if (curr->left == NULL) {
            Node* temp = curr->right;
            delete curr;
            return temp;
        }

        //1 child on left
        if (curr->right == NULL) {
            Node* temp = curr->left;
            delete curr;
            return temp;
        }

        //2 children
        Node* temp = minNode(curr->right);
        curr->data = temp->data;
        curr->right = deleteNode(curr->right, temp->data);
    }

    return curr;
}


int main() {
    int choice;

    do {
        cout << "\n===== MENU =====\n";
        cout << "1. Insert element\n";
        cout << "2. Display tree\n";
        cout << "3. Delete element\n";
        cout << "4. Exit\n";
        cout << "Enter choice: ";

        cin >> choice;

        switch (choice) {

            case 1: {
                int val;
                cout << "Enter number: ";
                cin >> val;
                Root = insertElement(Root, val);
                break;
            }

            case 2: {
                if (Root == NULL) {
                    cout << "Tree is empty\n";
                    break;
                }

                int choice2;
                cout << "\nDisplay method:\n";
                cout << "1. Preorder\n";
                cout << "2. Postorder\n";
                cout << "3. Inorder\n";
                cout << "Enter choice: ";
                cin >> choice2;

                if (choice2 == 1) PreOrder(Root);
                else if (choice2 == 2) PostOrder(Root);
                else if (choice2 == 3) InOrder(Root);
                else cout << "Invalid option";

                cout << endl;
                break;
            }

            case 3:
                cout << "Enter the element you want to delete";
                int del;
                cin >> del;
                deleteNode(Root, del);

            case 4:
                cout << "Goodbye\n";
                break;

            default:
                cout << "Invalid choice\n";
        }

    } while (choice != 4);
}
