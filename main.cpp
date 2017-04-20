#include <fstream>
#include <iostream>
#include <string>
#include <cstring>
#include <stdlib.h>
#include "node.h"
#include "link.h"

using namespace std;

void loadFile(Node *nodesPtr, Link *linksPtr, int& nodeAmount, int& linkAmount);

int main()
{
    //arrays of nodes/links and their pointers
    Node allNodes[1000];
    Node *nodesPtr;
    Link allLinks[1000];
    Link *linksPtr;
    
    int nodeAmount = 0; //used to initialize Nodes and Links from file
    int linkAmount = 0;
    
    nodesPtr = allNodes;
    linksPtr = allLinks;
    
    loadFile(nodesPtr, linksPtr, nodeAmount, linkAmount);
    
    return 0;
}

void loadFile(Node *nodesPtr, Link *linksPtr, int& nodeAmount, int& linkAmount)
{
    ifstream fin;
    string line; //used for getting lines from file
    int holderID; //used to hold id values while getting file input
    char stringHolder[100]; //used to convert line to cstring, then used to be converted to an integer
    size_t found;
    
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
            holderID = atoi(stringHolder); //change cstring to int
            (nodesPtr + nodeAmount)->setID(holderID);
            cout << (nodesPtr + nodeAmount)->getID() << endl;
            nodeAmount++;
        }
    }
    //get all links/edges from file
    while(getline(fin,line))
    {
        found = line.find("source ");
        if(found != string::npos)
        {
            line = line.substr(11,3); //get the exact position of the id number for the nodes
            strcpy(stringHolder, line.c_str()); //change string to cstring
            holderID = atoi(stringHolder); //change cstring to int
            (linksPtr + linkAmount)->setSource(holderID); //set source
            (linksPtr + linkAmount)->setAlive(true); //set status
            cout << "source: " << (linksPtr + linkAmount)->getSource() << endl;
        }
        found = line.find("target ");
        if(found != string::npos)
        {
            line = line.substr(11,3); //get the exact position of the id number for the nodes
            strcpy(stringHolder, line.c_str()); //change string to cstring
            holderID = atoi(stringHolder); //change cstring to int
            (linksPtr + linkAmount)->setTarget(holderID); //set target
            cout << "target: " << (linksPtr + linkAmount)->getTarget() << endl;
            linkAmount++;
        }
    }
    fin.close();
    return;
}

