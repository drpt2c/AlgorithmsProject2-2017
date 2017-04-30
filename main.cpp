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

//this loads the file into our data
void loadFile(Node *nodesPtr, int& nodeAmount, int& totalLink);

//This is Breadth First Search and calculates the depth from the Source S
void bfs(Node& S, Node *nodesPtr, const int nodeAmount);

//this calculates the max flow of the algorithm
int MaxFlow(Node& S, Node& D, Node *nodesPtr, const int nodeAmount, const int totalLink);

//This is a resusive function to get the max flow
int Residual(int target, Node&D, Node *nodesPtr, int lgFlow);

//checks the flow at D, which should accurately tell what the current flow is
int FlowOfEnd(Node& D, Node *nodesPtr);

//this clears all flow and visited nodes
void ResetAll(Node *nodesPtr, const int nodeAmount, const int totalLink);

//this will assign D after S is randomly placed
void SetDist(Node& S, Node& D, Node *nodesPtr, const int nodeAmount);

void addKLinks(Node& S, Node& D, Node *nodesPtr, const int nodeAmount, const int numberK);

//this will set all nodes that are not connected
void SetNodes(Node *nodesPtr, const int nodeAmount, const int totalLink);

//START OF OUR ATTACKS

//static attack method
void Static_Attack(Node& S, Node& D, Node *nodesPtr, const int nodeAmount, const int totalLink);

void Reactive_Attack(Node& S, Node& D, Node *nodesPtr, const int nodeAmount, const int totalLink);

void Random_Attack(Node& S, Node& D, Node *nodesPtr, const int nodeAmount, const int totalLink);


int main()
{
    int done;
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

    //Set all nodes with no edges to random links
    SetNodes(nodesPtr, nodeAmount, totalLink);

    
    
    SetDist(S, D, nodesPtr, nodeAmount);
    //setDestinationNodes(S, D, nodesPtr, nodeAmount);

    addKLinks(S, D, nodesPtr, nodeAmount, 50);
    

    initialMaxFlow = MaxFlow(S, D, nodesPtr, nodeAmount, totalLink);
    cout << "Initial Max Flow is " << initialMaxFlow << "." << endl;

    cout << "Total Number of Nodes: " << nodeAmount << endl;
    cout << "Total Number of Edges: " << totalLink << endl;

    int total = 0;
    for (int i = 0; i < nodeAmount; i++)
    {
        if ((nodesPtr + i)->getLinksAmount() == 0)
        {
            total++;
        }
    }
    cout << "Total number of Nodes with no links is: " << total << endl;

    cin >> done;

    //start of Our Attack Algorithms
    //static attack
    /*
    int i = 0;
    int flow = initialMaxFlow;
    while (flow != 0) {
        Static_Attack(S, D, nodesPtr, nodeAmount, totalLink);
    }
    */

    ResetAll(nodesPtr, nodeAmount, totalLink);

    //Reactive Attack
    int i = 0;
    int flow = initialMaxFlow;
    while (flow != 0)
    {
        Reactive_Attack(S, D, nodesPtr, nodeAmount, totalLink);
        flow = MaxFlow(S, D, nodesPtr, nodeAmount, totalLink);
        i++;
    }

    /*
    ResetAll(nodesPtr, nodeAmount, totalLink);
    //Random Attack
    int i = 0;
    int flow = initialMaxFlow;
    while (flow != 0)
    {
        Random_Attack(S, D, nodesPtr, nodeAmount, totalLink);

    }
    */


    
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
    
    //fin.clear();
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
            int capacity = 0;
            while (capacity == 0) {
                capacity = (rand() % 20) + 1;
            }

            Link newLink(holderSource, holderTarget, capacity /*rand() % 20 + 1*/, true);
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

void SetNodes(Node *nodesPtr, const int nodeAmount, const int totalLink)
{
    Link link;
    int randomNumb;
    for (int i = 0; i < nodeAmount; i++)
    {
        if ((nodesPtr + i)->getLinksAmount() == 0)
        {
            link.setSource( (nodesPtr + i)->getVertex() );
            link.setBreak(false);

            //connect node to x many links
            for (int k = 0; k < 20; k++)
            {
                randomNumb = rand() % nodeAmount;

                //make sure the link is not connected to D
                if ((nodesPtr + randomNumb)->getVertex() != (nodesPtr + i)->getVertex() )
                {
                    link.setTarget((nodesPtr + randomNumb)->getVertex());
                    (nodesPtr + i)->addLink(link);
                    cout << "ADDED LINK: " << (nodesPtr+i)->getVertex() << " WITH TARGET: " << (nodesPtr + randomNumb)->getVertex() << endl;
                }
                else
                    k--;
            }
        }
    }
}


void bfs(Node& S, Node *nodesPtr, const int nodeAmount) {

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

    list<int> queue;

    // Mark the current node as visited and enqueue it
    S.setVisited(true);
    queue.push_back(S.getVertex());

    // 'i' will be used to get all adjacent vertices of a vertex


    while (!queue.empty() && !found)
    {
        // Dequeue a vertex from queue and print it
        s = queue.front();
        linksPtr = (nodesPtr + s)->getLinks();
        linkAmount = (nodesPtr + s)->getLinksAmount();

        //cout << s << endl;
        queue.pop_front();

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
                //cout << "Incremented Distance: " << (nodesPtr + targetVertex)->getDistance() << endl;
                queue.push_back(targetVertex);
            }
           /* if ((nodesPtr + targetVertex)->getVertex() == D.getVertex())
            {
                found = true;
                D.setDistance((nodesPtr + targetVertex)->getDistance());
            }*/
        }
    }
  /*  if (queue.empty())
        cout << "QUEUE EMPTY" << endl;
    if (found)
        cout << "FOUND - Node D Distance: " << D.getDistance() << endl;*/
    return;
}


int MaxFlow(Node& S, Node& D, Node *nodesPtr, const int nodeAmount, const int totalLink) { //if the flow is 0, the network is dead
    Link *linksPtr;     //useful variables
    int linkAmount;
    int targetVertex;
    int totalFlow = 0;
    int flow = 0;
    int i = 0;
    int capacity;//for links capcity
    int avFlow;//for the lgFlow
    bool alive = false;

    linksPtr = (nodesPtr + S.getVertex())->getLinks();

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
    //sort the links by most avaiable flow 
    /*
    int *hold = new int[linkAmount];
    int *sorted = new int[linkAmount];

    int maxLinkNum = linkAmount;
    int maxFlow = 0;
    for (int i = 0; i < maxLinkNum; i++) //this should partially sort the list
    {
        targetVertex = (linksPtr + i)->getTarget();
        capacity = (linksPtr + i)->getCapacity();
        avFlow = capacity - (linksPtr + i)->getFlow();//avaiable flow
        hold[i] = avFlow; //this is to place each flow in a array
        sorted[i] = i;
    }
    int temphold;//temp for hold
    int temp; //temp for sorted
    for (int i = 1; i < linkAmount; i++) //this is insertion sort to sort link saturation
    {
        for (int j = i; j >= 1; j--)
        {
            if (hold[j] > hold[j - 1])// if emement further in the array is greater
            {
                temphold = hold[j];//hold
                temp = sorted[j];//sorted
                hold[j] = hold[j - 1];//hold
                sorted[j] = sorted[j - 1];//sorted
                hold[j - 1] = temphold;//hold
                sorted[j - 1] = temp;//sorted
            }
        }
    }
    delete[] hold;

    int s;*/
    //main part, maxflow is the basecase, this will call residual recursively.
    while (i < linkAmount)/*there are links to choose*/
    {
        //s = sorted[i];
        targetVertex = (linksPtr + i)->getTarget(); //this is the information for Residual
        capacity = (linksPtr + i)->getCapacity();
        avFlow = capacity - (linksPtr + i)->getFlow();
        alive = (linksPtr + i)->getAlive(); //end of info for residual
        for (int j = 0; j < nodeAmount; j++)
        {
            (nodesPtr + j)->setVisited(false);
        }

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
    //curVertex = (nodesPtr)->getVertex();

    //sort the links by most avaiable flow 

    /*
    int *hold = new int[linkAmount];
    int *sorted = new int[linkAmount];
  
    int maxLinkNum = linkAmount;
    int maxFlow = 0;
    for (int i = 0; i < maxLinkNum; i++) //this should partially sort the list
    {
        targetVertex = (linksPtr + i)->getTarget();
        capacity = (linksPtr + i)->getCapacity();
        avFlow = capacity - (linksPtr + i)->getFlow();//avaiable flow
        hold[i] = avFlow; //this is to place each flow in a array
        sorted[i] = i;
    }
    int temphold;//temp for hold
    int temp; //temp for sorted
    for (int i = 1; i < linkAmount; i++) //this is insertion sort to sort link saturation
    {
        for (int j = i; j >= 1; j--)
        {
            if (hold[j] > hold[j - 1])// if emement further in the array is greater
            {
                temphold = hold[j];//hold
                temp = sorted[j];//sorted
                hold[j] = hold[j - 1];//hold
                sorted[j] = sorted[j - 1];//sorted
                hold[j - 1] = temphold;//hold
                sorted[j - 1] = temp;//sorted
            }
        }
    }
    delete[] hold;


    int s;*/
    //goes through each node it will visit
    while (i < linkAmount) //while there are still links from G to visit
    {
        //s = sorted[0];
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
                //delete[] sorted;
                return flow;
            }
        }
        i++;

    }
    //delete[] sorted;
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
void ResetAll(Node *nodesPtr, const int nodeAmount, const int totalLink)
{
    Link *linkPtr;
    int linkAmount;
;
    for (int i = 0; i < nodeAmount; i++) //this resets the depth and the visted for all nodes
    {
        (nodesPtr + i)->setVisited(false);
        (nodesPtr + i)->setDistance(0);
        linkPtr = (nodesPtr + i)->getLinks();
        linkAmount = (nodesPtr + i)->getLinksAmount();
        for (int k = 0; k < totalLink; k++) //this resets all the link values that have been altered
        {
            (linkPtr + k)->setFlow(0);
            (linkPtr + k)->setAlive(true);
            (linkPtr + k)->setBreak(true);
        }
    }
}

void SetDist(Node& S, Node& D, Node *nodesPtr, const int nodeAmount) {
    int randomNumb; //random number
    int curr = 0;   //help with getting max distance
    int maxDis = 0;

    while (maxDis < 9) {
        //set random S and D nodes
        randomNumb = rand() % nodeAmount;
        S.setVertex((nodesPtr + randomNumb)->getVertex()); //sets the starting node S to a random spot

        bfs(S, nodesPtr, nodeAmount);   //calculates all the distances from S
        for (int i = 0; i < nodeAmount; i++)
        {
            curr = (nodesPtr + i)->getDistance(); //gets the distance for each node
            if (curr > maxDis) //if a node is larger
            {
                maxDis = curr;//sets the values to the new furthest node
                D.setVertex( (nodesPtr + i)->getVertex() );
                D.setDistance( (nodesPtr + i)->getDistance() );
            }
        }

        cout << "D has been set." << endl;
        cout << "D's distance from S is " << D.getDistance() << "." << endl;
    }
    
}

void addKLinks(Node& S, Node& D, Node *nodesPtr, const int nodeAmount, const int numberK)
{
    int randomNumb;
    Link Klink;

    Klink.setSource(S.getVertex());
    Klink.setBreak(false);

    //make klinks for S
    for (int i = 0; i < numberK; i++)
    {
        randomNumb = rand() % nodeAmount;

        //make sure the link is not connected to D
        if ((nodesPtr + randomNumb)->getVertex() != D.getVertex())
        {
            Klink.setTarget((nodesPtr + randomNumb)->getVertex());
            S.addLink(Klink);
            cout << "ADDED LINK FROM S WITH TARGET: " << (nodesPtr + randomNumb)->getVertex() << endl;
        }
        else
            i--;
    }

    Klink.setSource(D.getVertex());
    Klink.setBreak(false);

    //make klinks for D
    for (int i = 0; i < numberK; i++)
    {
        randomNumb = rand() % nodeAmount;

        //make sure the link is not connected to S
        if ((nodesPtr + randomNumb)->getVertex() != S.getVertex())
        {
            Klink.setTarget((nodesPtr + randomNumb)->getVertex());
            D.addLink(Klink);
            cout << "ADDED LINK FROM D WITH TARGET: " << (nodesPtr + randomNumb)->getVertex() << endl;

        }
        else
            i--;
    }

}




/*    __________________OUR_ALGORITHMS___________________________*/


void Static_Attack(Node& S, Node& D, Node *nodesPtr, const int nodeAmount, const int totalLink)
{




}


void Reactive_Attack(Node& S, Node& D, Node *nodesPtr, const int nodeAmount, const int totalLink)
{




}


void Random_Attack(Node& S, Node& D, Node *nodesPtr, const int nodeAmount, const int totalLink)
{






}