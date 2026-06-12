#include <iostream>
#include <stdlib.h>
using namespace std;

struct Edge;

struct Vertex{
    char data;
    Vertex *vertex_next;
    Edge *edge_list;
    bool visited;
};

struct Edge{
    Edge *edge_next;
    Vertex *vertex;
};

struct QNode{
    Vertex* data;
    QNode *next;
};

QNode *front = NULL;
QNode *rear = NULL;

void enQueue(Vertex *val){

    QNode *temp = new QNode();
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

Vertex* deQueue(){

    if (front == NULL){
        cout << "Queue is empty" << endl;
        return;
    }
    QNode *temp = front;
    front = front->next;

    if (temp == NULL){
        rear = NULL;
    }
    delete temp;
}

Vertex *graph = NULL;

void addVertex(char val){
    Vertex *temp = (Vertex*)malloc(sizeof(Vertex));
    temp->data = val;
    temp->vertex_next = NULL;
    temp->edge_list = NULL;

    if(graph==NULL){
        graph = temp;
        return;
    }

    Vertex *curr = graph;
    while(curr->vertex_next!=NULL){
        curr = curr->vertex_next;
    }
    curr->vertex_next=temp;
}

Vertex* getAddress(char a){
    Vertex *curr = graph;
    while(curr!=NULL){
        if(a==curr->data){
            return curr;
        }
        curr = curr->vertex_next;
    }
    cout << "Vertex does not exist in the graph" << endl;
    return 0;
}

void addEdge(char vertex1, char vertex2){
    Vertex *v1 = getAddress(vertex1);
    Vertex *v2 = getAddress(vertex2);

    if(v1==NULL || v2==NULL){
        cout << "Vertex/Vertices don't exist" << endl;
        return;
    }

    //for edge from v1 to v2
    Edge *temp1 = (Edge *)malloc(sizeof(Edge));
    temp1->vertex = v2;
    temp1->edge_next = NULL;
    if(v1->edge_list==NULL){
        v1->edge_list = temp1;
    }else{
        Edge *curr1 = v1->edge_list;
        while (curr1->edge_next!=NULL){
            curr1 = curr1->edge_next;
        }
        curr1->edge_next = temp1;
    }

    //for edge from v2 to v1
    Edge *temp2=(Edge*)malloc(sizeof(Edge));
    temp2->vertex = v1;
    temp2->edge_next = NULL;
    if(v2->edge_list==NULL){
        v2->edge_list = temp2;
    }else{
        Edge *curr2 = v2->edge_list;
        while(curr2->edge_next!=NULL){
            curr2 = curr2->edge_next;
        }
        curr2->edge_next = temp2;
    }
}

void display(){
    Vertex *curr = graph;
    Edge *curr_edge;

    if(graph!=NULL){

        while(curr!=NULL){
            cout << curr->data << " | ";
            curr_edge = curr->edge_list;
            while(curr_edge!=NULL){
                cout << curr_edge->vertex->data << " ";
                curr_edge = curr_edge->edge_next;
            }
            curr = curr->vertex_next;
            cout << endl;
        }

    }
    else{
        cout << "Graph is empty" << endl;
    }
}

void MatrixPrint(){
    int n = 5;
    int adj_matrix[n][n];

    for (int i = 0; i < n;i++){
        for (int j = 0; j < n;j++){
            adj_matrix[j][i] = 0;
        }
    }

    Vertex *v = graph;
    int i = 0;
    while (v!=NULL)
    {
        Edge*curr=v->edge_list;
        while(curr!=NULL){
            Vertex *temp = graph;
            int j = 0;
            while(temp!=NULL){
                if(temp->data==curr->vertex->data){
                    adj_matrix[i][j] = 1;
                }
                temp = temp->vertex_next;
                j++;
            }
            curr = curr->edge_next;
        }
        v = v->vertex_next;
        i++;
    }

    cout << "Adjacency matrix representation of graph" << endl;
    for (int x = 0;x<n;x++){
        for (int y = 0; y < n;y++){
            cout << adj_matrix[x][y] << " ";
        }
        cout << endl;
    }
}

void vertexDegree(int val){
    int count = 0;
    Vertex *curr = graph;
    if(val==curr->data){
        Edge *curr_edge = curr->edge_list;
        while(curr_edge!=NULL){
            count++;
            curr_edge = curr_edge->edge_next;
        }
    }
    cout << "Degree of " << val << " is:" << count;
}

void BFS(Vertex* graph){
    Vertex *curr = graph;
    enQueue(curr);



    Edge *curr_edge = curr->edge_list;
    while(front!=NULL){
        

    }

}

int main ()
{
    int choice; 
    char val,num1,num2;
        
        while (true)
        {
            cout << "\n--- MENU ---\n";
            cout << "1. Add Vertex \n2. Display \n3. Add Edge \n4. Adjacency Matrix \n5. Degree";
            cout << "Enter choice: ";
            cin >> choice; 
            
            switch(choice) 
            {
                case 1:
                    cout << "Enter value to insert: ";
                    cin >> val;
                    addVertex(val);
                    break;

                case 2:
                    display();
                    break;

                case 3:
                    cout << "Enter vertex I:";
                    cin >> num1;
                    cout << "Enter vertex II to attach:";
                    cin >> num2;
                    addEdge(num1,num2);
                    break;

                case 4:
                    MatrixPrint();
                    break;

                case 5:
                    cout << "Enter the vertices to find degree of:";
                    int d1;
                    cin d
                    vertexDegree(d1);
                    break;

                default : cout  << "Invalid choice. Please try again."    << endl;
            }
        }
    return 0;
}
