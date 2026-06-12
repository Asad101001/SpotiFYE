#include <iostream>
#include <stdlib.h>
using namespace std;

struct Edge;

struct Vertex
{
    char data;
    bool visited;
    Vertex *vnext;
    Edge *Edgelist;
};

struct Edge
{
    Edge *enext;
    Vertex *v;
};

Vertex *graph = NULL;

// ================= QUEUE STRUCTURE =================
struct dqueue
{
    Vertex *data;
    dqueue *next;
};

dqueue *front = nullptr;
dqueue *rear = nullptr;

// ================= QUEUE FUNCTIONS =================
void Enqueue(Vertex *value)
{
    dqueue *temp = (dqueue *)malloc(sizeof(dqueue));
    temp->data = value;
    temp->next = nullptr;

    if (front == nullptr && rear == nullptr)
    {
        front = rear = temp;
    }
    else
    {
        rear->next = temp;
        rear = temp;
    }
}

Vertex *Dequeue()
{
    if (front == nullptr)
        return nullptr;

    dqueue *temp = front;
    Vertex *val = temp->data;
    front = front->next;

    if (front == nullptr)
        rear = nullptr;

    free(temp);
    return val;
}

// ================= GRAPH FUNCTIONS =================
void addVertex(char value)
{
    Vertex *temp = (Vertex *)malloc(sizeof(Vertex));
    temp->data = value;
    temp->visited = false;
    temp->vnext = NULL;
    temp->Edgelist = NULL;

    if (graph == NULL)
    {
        graph = temp;
        return;
    }

    Vertex *curr = graph;
    while (curr->vnext != NULL)
        curr = curr->vnext;

    curr->vnext = temp;
}

Vertex *address(char a)
{
    Vertex *curr = graph;

    while (curr != NULL)
    {
        if (a == curr->data)
            return curr;

        curr = curr->vnext;
    }

    cout << "Vertex does not exist." << endl;
    return NULL;
}

void addEdge(char vertex1, char vertex2)
{
    Vertex *v1 = address(vertex1);
    Vertex *v2 = address(vertex2);

    if (v1 == NULL || v2 == NULL)
        return;

    // v1 -> v2
    Edge *temp1 = (Edge *)malloc(sizeof(Edge));
    temp1->v = v2;
    temp1->enext = NULL;

    if (v1->Edgelist == NULL)
        v1->Edgelist = temp1;
    else
    {
        Edge *curr = v1->Edgelist;
        while (curr->enext != NULL)
            curr = curr->enext;
        curr->enext = temp1;
    }

    // v2 -> v1 (undirected)
    Edge *temp2 = (Edge *)malloc(sizeof(Edge));
    temp2->v = v1;
    temp2->enext = NULL;

    if (v2->Edgelist == NULL)
        v2->Edgelist = temp2;
    else
    {
        Edge *curr = v2->Edgelist;
        while (curr->enext != NULL)
            curr = curr->enext;
        curr->enext = temp2;
    }
}

void display()
{
    Vertex *curr = graph;

    if (graph == NULL)
    {
        cout << "Graph is empty\n";
        return;
    }

    while (curr != NULL)
    {
        cout << curr->data << " | ";

        Edge *e = curr->Edgelist;
        while (e != NULL)
        {
            cout << e->v->data << " ";
            e = e->enext;
        }

        cout << endl;
        curr = curr->vnext;
    }
}

void Degree()
{
    Vertex *v = graph;

    while (v != NULL)
    {
        int count = 0;
        Edge *e = v->Edgelist;

        while (e != NULL)
        {
            count++;
            e = e->enext;
        }

        cout << "Degree of " << v->data << " : " << count << endl;
        v = v->vnext;
    }
}

// ================= BFS FUNCTION =================
void BFS(char start)
{
    Vertex *s = address(start);
    if (s == NULL)
        return;

    // reset visited
    Vertex *temp = graph;
    while (temp != NULL)
    {
        temp->visited = false;
        temp = temp->vnext;
    }

    cout << "BFS Traversal: ";

    s->visited = true;
    Enqueue(s);

    while (front != nullptr)
    {
        Vertex *v = Dequeue();
        cout << v->data << " ";

        Edge *e = v->Edgelist;

        while (e != NULL)
        {
            if (!e->v->visited)
            {
                e->v->visited = true;
                Enqueue(e->v);
            }
            e = e->enext;
        }
    }

    cout << endl;
}

// ================= MAIN =================
int main()
{
    int choice;
    char val, num1, num2;

    while (true)
    {
        cout << "\n--- MENU ---\n";
        cout << "1. Add Vertex\n";
        cout << "2. Display\n";
        cout << "3. Add Edge\n";
        cout << "4. Degree\n";
        cout << "5. BFS\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            cout << "Enter vertex: ";
            cin >> val;
            addVertex(val);
            break;

        case 2:
            display();
            break;

        case 3:
            cout << "Enter vertex 1: ";
            cin >> num1;
            cout << "Enter vertex 2: ";
            cin >> num2;
            addEdge(num1, num2);
            break;

        case 4:
            Degree();
            break;

        case 5:
            cout << "Enter starting vertex: ";
            cin >> val;
            BFS(val);
            break;

        default:
            cout << "Invalid choice\n";
        }
    }

    return 0;
}