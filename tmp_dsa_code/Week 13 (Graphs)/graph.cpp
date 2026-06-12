#include<iostream>
#include<stdio.h>

using namespace std;

struct edge;

struct vertex
{
    char data;
    vertex*vnext;
    edge*edgelist;
    bool visited;
    
};

struct edge
{
    int weight;
    edge*enext;
    
    vertex*v;

};
vertex*graph=NULL;




struct queue
{
    vertex*data;
    queue*next;
}; 

queue*front=NULL;
queue*rear= NULL;


void Enqueue(vertex* value)
{
    queue*temp;
    temp=(queue*)malloc(sizeof(queue));
    temp->data=value;
    temp->next=nullptr;

    if (front ==NULL & rear == NULL) // if both front and rear is null the assign the temp to front and rear which is the first cond if queueing the first elemnt
    {
        front=temp;
        rear=temp;
        // temp->next=front;

        cout<<"\nThe value successfully queued\n";
        
    }
    
    else
    {
        rear->next=temp; // used the rear because it is always pointing to the end
        rear=temp;  // then we point the rear to the new last elemnt

        // temp->next=front;
        
        cout<<"\nThe value successfully queued\n";


    }


}





vertex* Dequeue()
{
    queue*curr=front;
   
    
    if(front==NULL) // cond 1 queue is empty
    {
        cout<<"List is empty, deletion not possible"<<endl;
        return;
    }

    if (front==rear) // cond 2 only one element present
    {
        vertex* tmp = curr->data;
        free (curr);
        cout<<"\nValue Dequeued\n";
        rear=NULL;
        front=NULL;
        return tmp;
    }


    else // cond 3 more than one element
    {
        // rear->next=front->next;
        front=front->next;

        free(curr);
        cout<<"\nValue Dequeued\n";
        return 0;
    }
    

}




// int arrmat[5][5];


void AddVertex (char value)
{
    vertex*temp;  // temp for the courses
    temp=(vertex*)malloc(sizeof(vertex));
    temp->data=value; 
    temp->vnext=NULL;  // 
    temp->visited=false;
    
    temp->edgelist=NULL; // 

    if (graph==NULL)
    {
        graph=temp;
    }
    else
    {
        vertex*curr=graph;
        while(curr->vnext!=NULL)
        {
            curr=curr->vnext;
        }
        curr->vnext=temp;
    }


}

void removeAllOcc(char value)
{
    vertex*curr=graph;

        while (curr!=NULL)
        {
            edge*ecurr=curr->edgelist;
            edge*prev1;
            while(ecurr!=NULL)
            {
                if (value==curr->edgelist->v->data)
                {
                    curr->edgelist=curr->edgelist->enext;
                    free(ecurr);
                    break;
                }
                prev1=curr->edgelist;
                ecurr=ecurr->enext;
                while(ecurr!=NULL)
                {
                    if (value==ecurr->v->data)
                    {
                        prev1->enext=ecurr->enext;
                        free(ecurr);
                        break;

    
                    }
                    prev1=ecurr;
                    ecurr=ecurr->enext;
                }
                
                ecurr=ecurr->enext;
            }
            curr=curr->vnext;
        }
    return;

}


void RemoveVertex(char value)
{
    vertex*curr=graph;
    vertex* prev;

    while (curr!=NULL)
    {
        if (value==curr->data)
        {
            graph=graph->vnext;
            edge*ecurr=curr->edgelist;

            while(ecurr!=NULL)
            {
                curr->edgelist=curr->edgelist->enext;
                free(ecurr);
                ecurr=curr->edgelist;
                
            }
            free(curr);
            return;
        }

        prev=graph;
        curr=curr->vnext;

        while (curr !=NULL)
        {
            if (value==curr->data)
            {
                prev->vnext=curr->vnext;
                edge* ecurr = curr->edgelist;

                while(ecurr!=NULL) 
                {
                    curr->edgelist=curr->edgelist->enext;  

                    free(ecurr);  

                    ecurr= curr->edgelist;  
                    
                }
                free(curr); 
                
            }
            prev=curr;
            curr=curr->vnext;
        }

        removeAllOcc(value);

    }
    
}

vertex* GiveAddress(char a)
{
    vertex*curr=graph;
    vertex*address;

    while (curr!=NULL)
    {
        if (a==curr->data)
        {
            return curr;
        }
      curr=curr->vnext;
    }

    cout<<"The vertex doesnot exists"<<endl;
    return 0;
    
}


void AddEdge(char vert1,char vert2)
{
    vertex*curr=graph;
    vertex*curr2=graph;
    
    edge*ecurr1;
    edge*ecurr2;

    while(curr!=NULL)
    {
        if (vert2==curr->data)
        {
            vertex*address1=GiveAddress(vert2);

            while (curr2!=NULL)
            {
                if (vert1==curr2->data)
                {
                    
                    edge*temp;
                    temp=(edge*)malloc(sizeof(edge));
                    temp->v=address1;
                    temp->enext=NULL;
                    temp->weight=0;
                    
                   
                    if (curr2->edgelist==NULL)
                    {
                        curr2->edgelist=temp;
                    }
                    else
                    {
                        ecurr1=curr2->edgelist;
                        while(ecurr1->enext!=NULL)
                        {
                            ecurr1=ecurr1->enext;
                        }
                        ecurr1->enext=temp;

                    }
                }
                curr2=curr2->vnext;
                
            }

            vertex*address2=GiveAddress(vert1);

            edge*temp2;
            temp2=(edge*)malloc(sizeof(edge));
            temp2->v=address2;
            temp2->enext=NULL;
            temp2->weight=0;

            if (curr->edgelist==NULL)
            {
                curr->edgelist=temp2;
            }
            else
            {
                ecurr2=curr->edgelist;
                while(ecurr2->enext!=NULL)
                {
                    ecurr2=ecurr2->enext;
                }
                ecurr2->enext=temp2;

            }

        }
        curr=curr->vnext;
    }
}





void display()
{
    vertex*curr=graph;
    edge*ecurr;
    if (graph!=NULL)
    {
        while(curr!=NULL)
        {
            
            cout<<curr->data<<" | ";
            ecurr=curr->edgelist;
            while(ecurr!=NULL)
            {
                cout<<ecurr->v->data<<" ";
                ecurr=ecurr->enext;

            }
            curr=curr->vnext;


            cout<<endl;
        }
    }
    else
    {
        cout<<"Graph is empty"<<endl;
    }
}


void degree( int value)
{
    int count=0;
    vertex* curr=graph;
    if (value==curr->data)
    {
        edge*ecurr=curr->edgelist;
        while(ecurr!=NULL)
        {
            count++;
            ecurr=ecurr->enext;
        }

        cout<<count;

    }
}


void BFS() 
{
    vertex* curr = graph;
    vertex*output;
    Enqueue(curr);

    edge* ecurr=curr->edgelist;
    while(front!=NULL)
    {
        Dequeue();

    }


}    




// void MatrixForm()
// {
//     int n=5;
//     int arraymat[n][n];

//     for (size_t i = 0; i < n; i++)
//     {
//         for (size_t j = 0; j < n; j++)
//         {
//             arraymat[i][j]=0;
//         }
        
//     }
    

//     vertex* curr=graph;


    
//     int i=0;
//     int j=0;
//     while(curr!=NULL)
//     {
//         edge* ecurr=curr->edgelist;
        
//         while (ecurr!=NULL)
//         {
//             if (ecurr->v->data==curr->data)
//             {
//                 arraymat[i][j]=1;
//             }
//             j++;
//             curr=curr->vnext;
//         }
        
//         ecurr=ecurr->enext;

//     }

    


// }





int main ()
{
    int choice; 
    char val,num1,num2;
        
        while (true)
        {
            cout << "\n--- MENU ---\n";
            cout << "1. Add Vertex \n2. Display \n3. Add Edge \n4. Remove Vertex \n5. Matrix form  \n6. degree  \n";
            cout << "Enter choice: ";
            cin >> choice; 
            
            switch(choice) 
            {
                case 1:
                    cout << "Enter value to insert: ";
                    cin >> val;
                    AddVertex(val);
                    break;

                case 2:
                    display();
                    break;

                case 3:
                    cout << "Enter vertex 1:";
                    cin >> num1;
                    cout << "Enter vertex 2 to attach:";
                    cin >> num2;

                    AddEdge(num1,num2);
                    break;

                case 4:
                    cout << "Enter value to delete: ";
                    cin >> val;
                    RemoveVertex(val);
                    break;

                // case 5:
                //     MatrixForm();
                //     break;

                case 5:
                    cout << "Enter value to insert: ";
                    cin >> val;
                    degree(val);
                    break;

                default:
                    cout << "Invalid choice. Please try again." << endl;
            }
        }

    return 0;
}