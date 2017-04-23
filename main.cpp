#include <fstream>
#include <iostream>
#include <string>
#include <cstring>
#include <stdlib.h>
#include "node.h"
#include "link.h"

using namespace std;

void loadFile(Node *nodesPtr, int& nodeAmount);

void setDestinationNodes(Node& S, Node& D, Node *nodesPtr, const int nodeAmount);

int measureShortestDistance(int distance, int sID, int dID);

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
    
    //setDestinationNodes(S, D, nodesPtr, nodeAmount);
    
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
        
        //measure distance to make sure they are far enough apart
        //d = measureShortestDistance(0, S.getVertex(), D.getVertex());
        if(d >= 10)
        {
            farApart = true;
        }
            
    }
}
/*
int measureShortestDistance(int distance, int sID, int dID)
{
    int amountT = 0; //amount of links from S
    int targets[30]; //all targets from source node
    int distances[50]; //all potential
    //search all links for ones with source = S and get their target IDs
    for(int i = 0; i < linkAmount; i++)
    {
        if(sID == (linksPtr + i)->getSource())
        {
            target[amountT] = (linksPtr + i)->getTarget();
            amountT++;
        } 
    }
    if(amountT > 0)
    {
        distance++;
    }
    //check to see if any of the targets are the end node
    for(int i = 0; i < amountT; i++)
    {
        if(targets[i] == dID)
            return distance;
    }
    //find all links, find if one of them has any of target links
    for(int h = 0; h < linkAmount; h++)  
    {
        for(int i = 0; i < amountT; i++)
        {
            if((linksPtr + h)->getTarget() == targets[i])
            {
                measureShortestDistance(distance, (linksPtr + h)->getID(), dID, linksPtr, linkAmount);
                        
            }
        }
    }
    return 0;
}
*/



