#include <fstream>
#include <iostream>
#include <string>
#include <cstring>
#include <stdlib.h>
#include <stdio.h>
#include <list>
#include <time.h>
#include "node.h"
#include "link.h"

using namespace std;

void loadFile(Node *nodesPtr, int& nodeAmount, int& totalLink);

void setDestinationNodes(Node& S, Node& D, Node *nodesPtr, const int nodeAmount);

int measureShortestDistance(Node& S, Node& D, Node *nodesPtr, const int nodeAmount);

bool bfs(Node& S, Node& D, Node *nodesPtr, const int nodeAmount);

int MaxFlow(Node& S, Node& D, Node *nodesPtr, const int nodeAmount, const int totalLink);

int Residual(int target, Node&D, Node *nodesPtr, int lgFlow);

int FlowOfEnd(Node& D, Node *nodesPtr);

void ResetAll(Node& S, Node& D, Node *nodesPtr, const int nodeAmount, const int totalLink);

void addKLinks(Node& S, Node& D, Node *nodesPtr, const int nodeAmount, const int numberK);

void ResetKLinks(Node& S, Node& D);

int main()
{
    int initialMaxFlow = 0;//this is the starting maxflow
    srand(time(NULL));
    //arrays of nodes/links and their pointers
    Node *allNodes = new Node[1000];
    Node *nodesPtr;

    Node S; //starting node
    Node D; //ending node
    
    int nodeAmount = 0; //used to initialize Nodes and Links from file
    int totalLink = 0; //used to clear flow of the links
    
    nodesPtr = allNodes;
    
    loadFile(nodesPtr, nodeAmount, totalLink);
    
    setDestinationNodes(S, D, nodesPtr, nodeAmount);

    addKLinks(S, D, nodesPtr, nodeAmount, 10);

    initialMaxFlow = MaxFlow(S, D, nodesPtr, nodeAmount, totalLink);


    
    delete[] allNodes;
    return 0;
}

void loadFile(Node *nodesPtr, int& nodeAmount, int& totalLink)
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
            totalLink++; //total number of links
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


bool bfs(Node& S, Node& D, Node *nodesPtr, const int nodeAmount) {

    Link *linksPtr;
    int linkAmount;
    int targetVertex;
    int s;
    bool found = false; //used to find ending node

                        //set all nodes to default
    for (int i = 0; i < nodeAmount; i++)
    {
        (nodesPtr + i)->setVisited(false);
        (nodesPtr + i)->setDistance(0);
    }

    //creation of queue and starting queue
    list<int> q;
    q.push_back(S.getVertex());

    //set S to visited
    S.setVisited(true);
    
    while (!q.empty())
    {
        s = q.front();
        linksPtr = (nodesPtr + s)->getLinks();
        linkAmount = (nodesPtr + s)->getLinksAmount();
        // Get all adjacent vertices of the dequeued vertex s
        // If a adjacent has not been visited, then mark it visited
        // and enqueue it
        for (int i = 0; i < linkAmount; i++)
        {
            targetVertex = (linksPtr + i)->getTarget();
            if ((nodesPtr + targetVertex)->getVisited() == false)
            {
                (nodesPtr + targetVertex)->setVisited(true);
                (nodesPtr + targetVertex)->setParent(s);
                (nodesPtr + targetVertex)->setDistance((nodesPtr + s)->getDistance());
                (nodesPtr + targetVertex)->incrementDistance();
                cout << "Incremented Distance: " << (nodesPtr + targetVertex)->getDistance() << endl;
                q.push_back(targetVertex);
            }
            if ((nodesPtr + targetVertex)->getVertex() == D.getVertex())
            {
                found = true;
                D.setDistance((nodesPtr + targetVertex)->getDistance());
            }
        }
    }
    if (found == true)
    {
        cout << "Still a path to the end node D." << endl;
    }
    else {
        cout << "All paths are cut off." << endl;
    }
    return found;
}


int MaxFlow(Node& S, Node& D, Node *nodesPtr, const int nodeAmount, const int totalLink) { //if the flow is 0, the network is dead
    Link *linksPtr;     //useful variables
    int linkAmount;
    int targetVertex;
    int totalFlow = 0;
    int flow = 0;
    int capacity;//for links capcity
    int avFlow;//for the lgFlow
    bool alive = false;

    //clear the flow to recalculate it & the nodes that have been visited
    for (int i = 0; i < totalLink; i++)
    {
        (linksPtr + i)->setFlow(0);
    }
    for (int i = 0; i < nodeAmount; i++)
    {
        (nodesPtr + i)->setVisited(false);
    }

    //base case stuff from S
    linksPtr = (nodesPtr + S.getVertex())->getLinks();
    linkAmount = (nodesPtr + S.getVertex())->getLinksAmount();
    //need a way to sort the links by avaiable flow !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    //main part, maxflow is the basecase, this will call residual recursively.
    int i = 0;
    while (i < linkAmount/*there are links to choose*/)
    {
        targetVertex = (linksPtr + i)->getTarget();
        capacity = (linksPtr + i)->getCapacity();
        avFlow = capacity - (linksPtr + i)->getFlow();
        alive = (linksPtr + i)->getAlive();

        if (alive == true/*the links are alive*/)
        {
            flow = Residual(targetVertex, D, nodesPtr, avFlow); //I DON'T THINK I PASSED THE NEXT NODE RIGHT
            totalFlow = totalFlow + flow;
        }
        i++;
    }



    return totalFlow;
}

int Residual(int target, Node&D, Node *nodesPtr, int lgFlow)  //target is the current node
{
    //useful info
    Link *linksPtr;
    int linkAmount;
    int targetVertex;
    int curVertex;
    int flow = 0;
    int i = 0; //for incrementing in the while loop
    int capacity;//for links capcity
    int avFlow;//for the lgFlow
    bool alive = false;

    if (target == D.getVertex()) //if reached the ending node
    {
        return lgFlow;
    }

    //get the necessary info from the current node
    linksPtr = (nodesPtr + target)->getLinks();
    linkAmount = (nodesPtr + target)->getLinksAmount();
    curVertex = (nodesPtr)->getVertex();

    //sort the links by most avaiable flow !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


    //goes through each node it will visit
    while (i < linkAmount) //while there are still links from G to visit
    {
        alive = (linksPtr + i)->getAlive();
        if (alive == true && (nodesPtr + target)->getVisited() != true) /*the link is alive and not visited*/
        {
            targetVertex = (linksPtr + i)->getTarget();
            capacity = (linksPtr + i)->getCapacity();
            avFlow = capacity - (linksPtr + i)->getFlow();//avaiable flow 
            if (avFlow > lgFlow) //the avaliabe capacity of the link is greater than the lowest avaliable capacity
            {
                flow = Residual(targetVertex, D, nodesPtr, lgFlow);//recursively calls the data
            }
            else {
                flow = Residual(targetVertex, D, nodesPtr, avFlow);//recursively calls the data
            }
            if (flow == 0) {} //not a valid path, try again
            else {//the new lowest capacity flow of a link
                (linksPtr + i)->setFlow(flow);
                (nodesPtr + target)->setVisited(true);
                return flow;
            }
        }
        i++;

    }
    return 0;
}

int FlowOfEnd(Node& D, Node *nodesPtr) //I don't think this works nor is it necessary.
{
    //gets the amount and links of D(end node)
    int endlinks = (nodesPtr + D.getVertex())->getLinksAmount();
    Link *linksPtr = (nodesPtr + D.getVertex())->getLinks();
    int totalFlow = 0;

    //the current FLOW going into D
    for (int i = 0; i < endlinks; i++)
    {
        totalFlow = (linksPtr + i)->getFlow();
    }

    return totalFlow;
}

//this will make all the settings for Nodes and Links their default settings. aka, master reset.
void ResetAll(Node& S, Node& D, Node *nodesPtr, const int nodeAmount, const int totalLink)
{
    Link *linkPtr;
    int linkAmount
    
    for (int i = 0; i < nodeAmount; i++)
    {
        (nodesPtr + i)->setVisited(false);
        (nodesPtr + i)->setDistance(0);
        linkPtr = (nodesPtr + i)->getLinks();
        linkAmount = (nodesPtr + i)->getLinksAmount();
        
        for (int i = 0; i < linkAmount; i++)
        {
            (linkPtr + i)->setFlow(0);
            (linkPtr + i)->setAlive(true);
            (linkPtr + i)->setBreak(true);
        }

    }
}

void ResetKLinks(Node& S, Node& D)
{
    Link *linkPtr;
    int linkAmount;

    //reset K links
    linkPtr = S.getLinks();
    linkAmount = S.getLinksAmount();
    for (int i = 0; i < linkAmount; i++)
    {
        if((linkPtr + i)->getBreak() == false)
        {
            (linkPtr + i)->setTarget(-1);
            (linkPtr + i)->setBreak(true);
            S.decrementLinks();
        }
    }
    linkPtr = D.getLinks();
    linkAmount = D.getLinksAmount();
    for (int i = 0; i < linkAmount; i++)
    {
        if((linkPtr + i)->getBreak() == false)
        {
            (linkPtr + i)->setTarget(-1);
            (linkPtr + i)->setBreak(true);
            D.decrementLinks();
        }
    }
}

void addKLinks(Node& S, Node& D, Node *nodesPtr, const int nodeAmount, const int numberK)
{
    int randomNumb;
    Link Klink;
    
    Klink.setSource(S.getVertex());
    Klink.setBreak(false);
    
    //make klinks for S
    for(int i = 0; i < numberK; i++)
    {
        randomNumb = rand() % nodeAmount;
        
        //make sure the link is not connected to D
        if((nodesPtr + randomNumb)->getVertex() != D.getVertex())
        {
            Klink.setTarget((nodesPtr + randomNumb)->getVertex());
            S.addLink(Klink);
            cout << "ADDED LINK FROM S WITH TARGET: " << (nodesPtr + randomNumb)->getVertex() << endl;
        }
        else
            i--;
    }
    
    Klink.setSource(D.getVertex());

    //make klinks for D
    for(int i = 0; i < numberK; i++)
    {
        randomNumb = rand() % nodeAmount;
        
        //make sure the link is not connected to S
        if((nodesPtr + randomNumb)->getVertex() != S.getVertex())
        {
            Klink.setTarget((nodesPtr + randomNumb)->getVertex());
            D.addLink(Klink);
            cout << "ADDED LINK FROM D WITH TARGET: " << (nodesPtr + randomNumb)->getVertex() << endl;

        }
        else
            i--;
    }

}





