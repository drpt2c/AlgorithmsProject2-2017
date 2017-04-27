#include <fstream>
#include <iostream>
#include <string>
#include <cstring>
#include <stdlib.h>
#include <list>
#include "node.h"
#include "link.h"

using namespace std;

void loadFile(Node *nodesPtr, int& nodeAmount);

void setDestinationNodes(Node& S, Node& D, Node *nodesPtr, const int nodeAmount);

int measureShortestDistance(Node& S, Node& D, Node *nodesPtr, const int nodeAmount);

int main()
{
    srand(time(NULL));
    //arrays of nodes/links and their pointers
    Node allNodes[1000];
    Node *nodesPtr;

    Node S; //starting node
    Node D; //ending node
    
    int nodeAmount = 0; //used to initialize Nodes and Links from file
    
    nodesPtr = allNodes;
    
    loadFile(nodesPtr, nodeAmount);
    
    setDestinationNodes(S, D, nodesPtr, nodeAmount);
    
    return 0;
}

void loadFile(Node *nodesPtr, int& nodeAmount)
{
    ifstream fin;
    string line; //used for getting lines from file
    int holderSource; //used to hold id values while getting file input
    int holderTarget;
    char stringHolder[100]; //used to convert line to cstring, then used to be converted to an integer
    size_t found;
    
    nodeAmount = 0;
    
    fin.clear();
    fin.open("Kdl.gml.txt");

    //get all nodes from file
    while(getline(fin,line) && nodeAmount < 754)
    {
        found = line.find(" id ");
        if(found != string::npos)
        {
            line = line.substr(7,3); //get the exact position of the id number for the nodes
            strcpy(stringHolder, line.c_str()); //change string to cstring
            holderSource = atoi(stringHolder); //change cstring to int
            (nodesPtr + nodeAmount)->setVertex(holderSource);
            nodeAmount++;
        }
    }
    holderSource = -1;
    //get all links/edges from file
    while(getline(fin,line))
    {
        found = line.find("source ");
        if(found != string::npos)
        {
            line = line.substr(11,3); //get the exact position of the id number for the nodes
            strcpy(stringHolder, line.c_str()); //change string to cstring
            holderSource = atoi(stringHolder); //change cstring to int
        }
        found = line.find("target ");
        //set target and capacity
        if(found != string::npos)
        {
            line = line.substr(11,3); //get the exact position of the id number for the nodes
            strcpy(stringHolder, line.c_str()); //change string to cstring
            holderTarget = atoi(stringHolder); //change cstring to int
            
            //(linksPtr + linkAmount)->setTarget(holderSource); //set target
            //(linksPtr + linkAmount)->setCapacity(rand() % 20 + 1); //set capacity

            Link newLink(holderSource, holderTarget, rand() % 20 + 1, true);
            for(int i = 0; i < nodeAmount; i++)
            {
                if((nodesPtr + i)->getVertex() == holderSource)
                {
                    (nodesPtr + i)->addLink(newLink);
                } 
            }
        }
    }
    fin.close();
    
    Link *linkPtr;//for printing purposes
    //print
    for(int i = 0; i < nodeAmount; i++)
    {
        cout << "vertex" << (nodesPtr + i)->getVertex() << endl;
        linkPtr = (nodesPtr + i)->getLinks();
        //print all links from node i
        for(int j = 0; j < (nodesPtr + i)->getLinksAmount(); j++)
        {
            cout << "link " << j << ":" << endl; 
            cout << "\t target: " << (linkPtr + j)->getTarget() << endl;
            cout << "\t capacity: " << (linkPtr + j)->getCapacity() << endl;
        }
    }
    
    return;
}

void setDestinationNodes(Node& S, Node& D, Node *nodesPtr, const int nodeAmount)
{
    bool farApart = false;
    int randomNumb;
    int d;
    
    while(!farApart)
    {
        //set random S and D nodes
        randomNumb = rand() % nodeAmount;
        S.setVertex( (nodesPtr + randomNumb)->getVertex() );
        randomNumb = rand() % nodeAmount;
        D.setVertex( (nodesPtr + randomNumb)->getVertex() );
        
        cout << "Starting Node Attempt: " << S.getVertex() << endl;
        cout << "Ending Node Attempt: " << D.getVertex() << endl;

        //measure distance to make sure they are far enough apart
        d = measureShortestDistance(S, D, nodesPtr, nodeAmount);
        if(d >= 8)
        {
            farApart = true;
            cout << "Successful S and D assignment: " << endl;
        }
        else
        {
            cout << "random S and D, failed, trying again." << endl;
        }
            
    }
}

int measureShortestDistance(Node& S, Node& D, Node *nodesPtr, const int nodeAmount)
{
    Link *linksPtr;
    int linkAmount;
    int targetVertex;
    int s;
    bool found = false; //used to find ending node
    
    //set all nodes to default
    for(int i = 0; i < nodeAmount; i++)
    {
        (nodesPtr + i)->setVisited(false);
        (nodesPtr + i)->setDistance(0);
    }
    
    list<int> queue;
 
    // Mark the current node as visited and enqueue it
    S.setVisited(true);
    queue.push_back(S.getVertex());
 
    // 'i' will be used to get all adjacent vertices of a vertex
    
 
    while(!queue.empty() && !found)
    {
        // Dequeue a vertex from queue and print it
        s = queue.front();
        linksPtr = (nodesPtr + s)->getLinks();
        linkAmount = (nodesPtr + s)->getLinksAmount();
        
        cout << s << endl;
        queue.pop_front();
 
        // Get all adjacent vertices of the dequeued vertex s
        // If a adjacent has not been visited, then mark it visited
        // and enqueue it
        for(int i = 0; i < linkAmount; i++)
        {
            targetVertex = (linksPtr + i)->getTarget();
            if((nodesPtr + targetVertex)->getVisited() == false)
            {
                (nodesPtr + targetVertex)->setVisited(true);
                (nodesPtr + targetVertex)->setParent(s);
                (nodesPtr + targetVertex)->setDistance((nodesPtr + s)->getDistance());
                (nodesPtr + targetVertex)->incrementDistance();
                cout << "Incremented Distance: " << (nodesPtr + targetVertex)->getDistance() << endl;
                queue.push_back(targetVertex);
            }
            if((nodesPtr + targetVertex)->getVertex() == D.getVertex())
            {
                found = true;
                D.setDistance((nodesPtr + targetVertex)->getDistance());
            }
        }
    }
    if(queue.empty())
        cout << "QUEUE EMPTY" << endl;
    if(found)
        cout << "FOUND - Node D Distance: " << D.getDistance() << endl;
    return D.getDistance();
}



