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

//this is to check the flow of Static Algorithms
int StaticFlow(Node& S, Node& D, Node *nodesPtr, int deadNode, int deadLink, int prevFlow);

//This is used for StaticFlow to traverse down the graph
int StaticFlowResidual(int target, Node& D, Node * nodesPtr, int lowestFlow, int deadNode, int deadLink, int lostFlow, bool found);

void ClearFlow(Node& S, Node& D, Node * nodesPtr, const int nodeAmount);

int thisNodeisDead;
int thisLInkisDead;

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
    
	cout << "Loading in Graph" << endl;
    loadFile(nodesPtr, nodeAmount, totalLink);
	cout << "Graph done loading." << endl;

    //Set all nodes with no edges to random links
    //SetNodes(nodesPtr, nodeAmount, totalLink);
    
    SetDist(S, D, nodesPtr, nodeAmount);
    //setDestinationNodes(S, D, nodesPtr, nodeAmount);

    addKLinks(S, D, nodesPtr, nodeAmount, 50);
    
	cout << "Starting Max Flow" << endl;
    initialMaxFlow = MaxFlow(S, D, nodesPtr, nodeAmount, totalLink);
    cout << "Initial Max Flow is " << initialMaxFlow << "." << endl;

    cout << "Total Number of Nodes: " << nodeAmount << endl;
    cout << "Total Number of Edges: " << totalLink << endl;


	cin >> done;

    //start of Our Attack Algorithms
    //---------------------------------------Static Routing ----------------------//
    
	int prevFlow = 0;
	//Static Attack//
	//ofstream SS;
	//SS.open("static.dat");
	//SS << "Static\n";

    int i = 0;
    int flow = initialMaxFlow;
/*	prevFlow = flow;
	cout << "Starting the Static Attack on the Static Routing" << endl;
	cout << "Inital Max Flow is " << flow << endl;
    while (flow != 0) {
        Static_Attack(S, D, nodesPtr, nodeAmount, totalLink);
		flow = StaticFlow(S, D, nodesPtr, thisNodeisDead, thisLInkisDead, prevFlow);
		SS << i << " " << flow << "\n";
		i++;
		cout << "Attack finshed, new flow is " << flow << endl;
		prevFlow = flow;
		cout << "SAS Round " << i << "with a flow of " << flow << endl;
    }
    

    ResetAll(nodesPtr, nodeAmount, totalLink);

    //Reactive Attack//
	SS << "Reactive\n";

    i = 0;
    flow = initialMaxFlow;
	prevFlow = flow;
	cout << "Starting the Reactive Attack on the Static Routing" << endl;
    while (flow != 0)
    {
        Reactive_Attack(S, D, nodesPtr, nodeAmount, totalLink);
		flow = StaticFlow(S, D, nodesPtr, thisNodeisDead, thisLInkisDead, prevFlow);
		SS << i << " " << flow << "\n";
        i++;
		prevFlow = flow;
		cout << "RAS Round " << i << " with a flow of " << flow << endl;
    }
	SS << "\n";
    
    ResetAll(nodesPtr, nodeAmount, totalLink);

    //Random Attack//

	SS << "Random\n";

    i = 0;
    flow = initialMaxFlow;
	prevFlow = flow;
	cout << "Randomly Attacking the Static Routing" << endl;
    while (flow != 0)
    {
        Random_Attack(S, D, nodesPtr, nodeAmount, totalLink);
		flow = StaticFlow(S, D, nodesPtr, thisNodeisDead, thisLInkisDead, prevFlow);
		SS << i << " " << flow << "\n";
		i++;
		prevFlow = flow;
		cout << "RaS Round " << i << "with a flow of " << flow << endl;
    }
	SS.close();
	
	ResetAll(nodesPtr, nodeAmount, totalLink);

	*/
	//-----------------------------------------Reactive Routing -----------------------//
	ofstream SR;
	SR.open("reactive.dat");
	SR << "Static\n";

	i = 0;
	flow = initialMaxFlow; //will run first iteration
	cout << "Starting the Static Attack on Reactive Routing" << endl;
	while (flow != 0) {
		flow = MaxFlow(S, D, nodesPtr, nodeAmount, totalLink);			//new flow
		//cout << "The flow is " << flow << endl;
		Static_Attack(S, D, nodesPtr, nodeAmount, totalLink);			//attacks a link
		SR << i << " " << flow << "\n";									//writes to file
		i++;															//inc time stamp
	}


	SR << "\n";

	ResetAll(nodesPtr, nodeAmount, totalLink);

	//Reactive Attack//

	SR << "Reactive\n";
	i = 0;
	flow = initialMaxFlow;
	cout << "Started the Reactive Attack on Reactive Routing" << endl;
	while (flow != 0)
	{
		flow = MaxFlow(S, D, nodesPtr, nodeAmount, totalLink);		//Gets the new maxflow
		bfs(S, nodesPtr, nodeAmount);								//sets the distance from S
		Reactive_Attack(S, D, nodesPtr, nodeAmount, totalLink);		//Attacks a node closest to S

		SR << i << " " << flow << "\n";								//writes to a file
		//ClearFlow(S, D, nodesPtr, nodeAmount);						//clears the flow
		i++;														//increments the time stamp
	}
	SR << "\n";


	ResetAll(nodesPtr, nodeAmount, totalLink);
	//Random Attack//
	SR << "Random\n";

	i = 0;
	flow = initialMaxFlow;
	cout << "Randomly Attacking the Reactive Routing" << endl;
	while (flow != 0)
	{
		flow = MaxFlow(S, D, nodesPtr, nodeAmount, totalLink);		//New flow
		Random_Attack(S, D, nodesPtr, nodeAmount, totalLink);		//Kills a link
		SR << i << " " << flow << "\n";								//writes to file
		//ClearFlow(S, D, nodesPtr, nodeAmount);						//resets flow
		i++;														//new timestamp
	}

	SR.close();
	/*
	//------------------------------------K Static Graphs------------------------------------//
	
	ofstream SSK;
	SSK.open("k_static.dat");
	SSK << "Static\n";

	i = 0;
	flow = initialMaxFlow;
	prevFlow = flow;
	cout << "Starting the Static Attack on the Static Routing" << endl;
	while (flow != 0) {
		Static_Attack(S, D, nodesPtr, nodeAmount, totalLink);
		flow = StaticFlow(S, D, nodesPtr, thisNodeisDead, thisLInkisDead, prevFlow);
		SSK << i << " " << flow << "\n";
		i++;
		prevFlow = flow;
	}
	SSK << "\n";


	ResetAll(nodesPtr, nodeAmount, totalLink);

	//Reactive Attack//

	SSK << "Reactive\n";

	i = 0;
	flow = initialMaxFlow;
	prevFlow = flow;
	cout << "Starting the Reactive Attack on the Static Routing" << endl;
	while (flow != 0)
	{
		Reactive_Attack(S, D, nodesPtr, nodeAmount, totalLink);
		flow = StaticFlow(S, D, nodesPtr, thisNodeisDead, thisLInkisDead, prevFlow);
		SSK << i << " " << flow << "\n";
		i++;
		prevFlow = flow;
	}
	SSK << "\n";

	ResetAll(nodesPtr, nodeAmount, totalLink);

	//Random Attack//
	SSK << "Reactive\n";

	i = 0;
	flow = initialMaxFlow;
	prevFlow = flow;
	cout << "Randomly Attacking the Static Routing" << endl;
	while (flow != 0)
	{
		Random_Attack(S, D, nodesPtr, nodeAmount, totalLink);
		flow = StaticFlow(S, D, nodesPtr, thisNodeisDead, thisLInkisDead, prevFlow);
		SSK << i << " " << flow << "\n";
		i++;
		prevFlow = flow;
	}
	SSK.close();
	
	ResetAll(nodesPtr, nodeAmount, totalLink);

	//------------------------------------K Reactive Graphs----------------------------------//
	ofstream SKR;
	SKR.open("k_reactive.dat");
	SKR << "Static\n";

	i = 0;
	flow = initialMaxFlow;
	cout << "Starting the Static Attack on Reactive Routing" << endl;
	while (flow != 0) {
		Static_Attack(S, D, nodesPtr, nodeAmount, totalLink);
		flow = MaxFlow(S, D, nodesPtr, nodeAmount, totalLink);
		SKR << i << " " << flow << "\n";
		ClearFlow(S, D, nodesPtr, nodeAmount);
		i++;
	}

	SKR << "\n";

	ResetAll(nodesPtr, nodeAmount, totalLink);

	//Reactive Attack//

	SKR << "Reactive\n";

	i = 0;
	flow = initialMaxFlow;
	cout << "Started the Reactive Attack on Reactive Routing" << endl;
	while (flow != 0)
	{
		Reactive_Attack(S, D, nodesPtr, nodeAmount, totalLink);
		flow = MaxFlow(S, D, nodesPtr, nodeAmount, totalLink);
		SKR << i << " " << flow << "\n";
		ClearFlow(S, D, nodesPtr, nodeAmount);
		i++;
	}


	ResetAll(nodesPtr, nodeAmount, totalLink);
	//Random Attack//
	SKR << "Random\n";

	i = 0;
	flow = initialMaxFlow;
	cout << "Randomly Attacking the Reactive Routing" << endl;
	while (flow != 0)
	{
		Random_Attack(S, D, nodesPtr, nodeAmount, totalLink);
		flow = MaxFlow(S, D, nodesPtr, nodeAmount, totalLink);
		SKR << i << " " << flow << "\n";
		ClearFlow(S, D, nodesPtr, nodeAmount);
		i++;
	}

	SKR.close();

	*/


	//--------------------------------------Completed------------------------------//

    
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
            //int capacity = 0;
            //while (capacity == 0) {
            //    capacity = (rand() % 20) + 1;
            //}

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

void SetNodes(Node *nodesPtr, const int nodeAmount, const int totalLink)
{
	cout << "Setting Isolated Nodes" << endl;
	//________________________________________________
    Link link;
    int randomNumb;
	int randCap;
    for (int i = 0; i < nodeAmount; i++) //for each node
    {
        if ((nodesPtr + i)->getLinksAmount() == 0) //if it is not attached to anything
        {
			
            link.setSource( (nodesPtr + i)->getVertex() ); //set the source and breakability of each link
            link.setBreak(true);
			link.setAlive(true);

            //connect node to x many links
            for (int k = 0; k < 20; k++) //connect it to 20 nodes
            {
                randomNumb = rand() % nodeAmount; //random node

                //make sure the link is not connected to D
                if ((nodesPtr + randomNumb)->getVertex() != (nodesPtr + i)->getVertex() )
                {
                    link.setTarget((nodesPtr + randomNumb)->getVertex());

					randCap = rand() % 20 + 1;
					link.setCapacity(randCap);
                    (nodesPtr + i)->addLink(link);
                    //cout << "ADDED LINK: " << (nodesPtr+i)->getVertex() << " WITH TARGET: " << (nodesPtr + randomNumb)->getVertex() << endl;
                }
				else
				{
					k--;
				}
            }
        }
    }
	cout << "Finished with adding nodes" << endl;
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
	Link *linkPtr;
	for (int i = 0; i < nodeAmount; i++)//reset the flow of everything
	{
		linkPtr = (nodesPtr + i)->getLinks(); //gets the amount of links for each node
		for (int k = 0; k < (nodesPtr + i)->getLinksAmount(); k++)
		{
			(linkPtr + k)->setFlow(0);
		}
	}//resets all the flow, because MaxFlow is used for reactive only

	int currentRun = 1; //total of current run
	int totalFlow = 0;  //total of all runs
	int flow = 0;		//total of one run
	int target;			//the next node to go to
	int linkAmount;		//the total links in S
	int lowFlow;		//lowestflow of S
	int capacity;		//cacpity of the link
	
	linkAmount = S.getLinksAmount();
	linkPtr = S.getLinks();

	while (currentRun > 0) //until there is no more paths to go to
	{
		currentRun = 0; //nothing has gone this round yet
		for (int i = 0; i < linkAmount; i++) //start of the round
		{
			target = (linkPtr + i)->getTarget();			//grabs the next node
			capacity = (linkPtr + i)->getCapacity();		//grabs capacity
			lowFlow = capacity - (linkPtr + i)->getFlow();	//grabs the flow of the link
			flow = Residual(target, D, nodesPtr, lowFlow);  //gets the lowest flow for each path
			currentRun = currentRun + flow;					//the total of a run

		} //round is over
		totalFlow = totalFlow + currentRun; //total of all runs
		
	}
	return totalFlow;
}

int Residual(int target, Node&D, Node *nodesPtr, int lgFlow)  //target is the current node vertex
{
	Link *linkPtr;
	int linkAmount;
	int capacity;
	int flow;
	int availFlow;
	int lowestFlow;
	int next;
	int setFlow;
	if ( target == D.getVertex() ) //if we are at D, we are done, return lowest flow
	{

		//cout << "We reached the D and the lowestFlow is " << lgFlow << endl;
		return lgFlow; //lowestflow of all links
	}

	linkAmount = (nodesPtr + target)->getLinksAmount();
	linkPtr = (nodesPtr + target)->getLinks();

	for (int j = 0; j < linkAmount; j++) //for each link in the current node
	{
		if ((linkPtr + j)->getAlive() == true) //if the link is still alive
		{
			capacity = (linkPtr + j)->getCapacity();
			flow = (linkPtr + j)->getFlow();
			availFlow = capacity - flow;			//calculates the current available flow
			//cout << "Available Flow " << availFlow << endl;
			next = (linkPtr + j)->getTarget();		//next node to go to

			if (availFlow > 0 && next != -1) //if there is flow available in the link
			{
				if (availFlow > lgFlow)	//there is more flow avaiable in this link than others
				{
					lowestFlow = Residual(next, D, nodesPtr, lgFlow);
					//cout << "LowestFlow " << lowestFlow << endl;
				}
				else {	//the flow is equal or less than the lowest flow of the path
					lowestFlow = Residual(next, D, nodesPtr, availFlow);
					//cout << "LowestFlow " << lowestFlow << endl;

				}

				if (lowestFlow > 0)	//if there is flow that has made it to D
				{
					setFlow = availFlow + lgFlow;
					(linkPtr + j)->setFlow(setFlow); //sets the link to the lowest out of all in the path
					return lowestFlow;	//we are done
				}
			}
		}

	}
	return 0; //no path was found.

}

int StaticFlow(Node& S, Node& D, Node *nodesPtr, int deadNode, int deadLink, int prevFlow)
{ //this function will the check and readd the flow after an attack on Static Routing
	Link *linkPtr;
	return 0;
}

//target is current node, D is end, nodesPtr, lowestFlow is the lowest Flow of path, deadNode & link is dead spot, lostFlow is the flow of deadNode and Link, found is for changing the path after a link has died
int StaticFlowResidual(int target, Node& D, Node * nodesPtr, int lowestFlow, int deadNode, int deadLink, int lostFlow, bool found)
{ //this is the Recursive function for StaticFlow
	
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
        for (int k = 0; k < linkAmount; k++) //this resets all the link values that have been altered
        {
            (linkPtr + k)->setFlow(0);
            (linkPtr + k)->setAlive(true);
            //(linkPtr + k)->setBreak(true);
        }
    }
}

void SetDist(Node& S, Node& D, Node *nodesPtr, const int nodeAmount) //sets the placement of S and D
{
	int randomNumb;

	while (S.getVertex() == -1 || D.getVertex() == -1)
	{
		randomNumb = rand() % nodeAmount;
		if ((nodesPtr + randomNumb)->getLinksAmount() > 0)
		{
			S.setVertex((nodesPtr + randomNumb)->getVertex());
		}
		randomNumb = rand() % nodeAmount;
		if ((nodesPtr + randomNumb)->getLinksAmount() > 0 && (nodesPtr + randomNumb)->getVertex() != S.getVertex())
		{
			D.setVertex((nodesPtr + randomNumb)->getVertex());
		}
		
	}
	//cout << "S vertex" << S.getVertex() << " D Vertex" << D.getVertex() <<endl;
}

void addKLinks(Node& S, Node& D, Node *nodesPtr, const int nodeAmount, const int numberK)
{
	int randomNumb;
	Link Klink;
	int nodesK[100]; //used to save S's K nodes so that D cannot have a K node that is the same

	Klink.setCapacity(20);
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
			nodesK[i] = (nodesPtr + randomNumb)->getVertex();
			//cout << "ADDED LINK FROM S WITH TARGET: " << (nodesPtr + randomNumb)->getVertex() << endl;
		}
		else
			i--;
	}

	Klink.setTarget(D.getVertex());

	//make klinks for D
	for (int i = 0; i < numberK; i++)
	{
		bool allowed = true;
		randomNumb = rand() % nodeAmount;
		for (int j = 0; j < numberK; j++)
		{
			if ((nodesPtr + randomNumb)->getVertex() == nodesK[j])
				allowed = false;
		}

		//make sure the link is not connected to S or its K nodes
		if ((nodesPtr + randomNumb)->getVertex() != S.getVertex() && allowed)
		{
			Klink.setSource((nodesPtr + randomNumb)->getVertex());
			(nodesPtr + randomNumb)->addLink(Klink);
			//cout << "ADDED LINK TO D WITH SOURCE: " << (nodesPtr + randomNumb)->getVertex() << endl;
		}
		else
		{
			i--;
		}
	
	}

}

void ClearFlow(Node& S, Node& D, Node * nodesPtr, const int nodeAmount)
{
	int linkamount;
	Link *linkPtr;
	for (int i = 0; i < nodeAmount; i++)
	{
		linkPtr = (nodesPtr + i)->getLinks();
		linkamount = (nodesPtr + i)->getLinksAmount();
		for (int k = 0; k < linkamount; k++)
		{
			(linkPtr + k)->setFlow(0);
		}
	}
}




/*    __________________OUR_ALGORITHMS___________________________*/


void Static_Attack(Node& S, Node& D, Node *nodesPtr, const int nodeAmount, const int totalLink)
{
	Link *linkPtr;
	int MaxNode = 0;
	int MaxLink = 0; //keep track of most saturated
	int MaxFlow = 0;
	int flow = 0;

	for (int i = 0; i < nodeAmount; i++)  //for each node
	{
		linkPtr = (nodesPtr + i)->getLinks(); //for each node
		for (int k = 0; k < (nodesPtr + i)->getLinksAmount(); k++) //each link in a node
		{
			flow = (linkPtr + k)->getFlow();
			if (flow > MaxFlow) //if a links flow is greater than current stored max
			{
				if ((linkPtr + k)->getBreak() == true) //if breakable
				{
					if ((linkPtr + k)->getAlive() == true) //if alive
					{
						MaxFlow = (linkPtr + k)->getFlow(); //saves the max flow and current position of link
						MaxNode = i;
						MaxLink = k;
					}
				}

			}
		}
	}
	cout << "Attack has finished. Chosen node " << MaxNode << " and link " << MaxLink << endl;

	linkPtr = (nodesPtr + MaxNode)->getLinks(); //kills link
	(linkPtr + MaxLink)->setAlive(false);
	thisNodeisDead = MaxNode;
	thisLInkisDead = MaxLink;

}


void Reactive_Attack(Node& S, Node& D, Node *nodesPtr, const int nodeAmount, const int totalLink)
{
	Link *linkPtr;
	int MaxNode = 0;
	int MaxLink = 0; //keep track of most saturated
	int MaxFlow = 0;
	int LeastDis = 0;
	int tempDis = 0;


	for (int i = 0; i < nodeAmount; i++)
	{
		linkPtr = (nodesPtr + i)->getLinks(); //for each node
		tempDis = (nodesPtr + i)->getDistance();
		for (int k = 0; k < (nodesPtr + i)->getLinksAmount(); k++) //each link in a node
		{
			if ((linkPtr + k)->getFlow() > MaxFlow && tempDis < LeastDis && (linkPtr + k)->getBreak() == true && (linkPtr + k)->getAlive() == true) //if a links flow is greater than current stored max
			{//more saturated, least distance, breakable, and is alive
				MaxFlow = (linkPtr + k)->getFlow(); //saves the max flow and current position of link
				MaxNode = i;
				MaxLink = k;
				LeastDis = tempDis;
			}
		}
	}
	cout << "Attack has finished. Chosen node " << MaxNode << " and link " << MaxLink << endl;

	linkPtr = (nodesPtr + MaxNode)->getLinks(); //kills link
	(linkPtr + MaxLink)->setAlive(false);
	thisLInkisDead = MaxLink;
	thisNodeisDead = MaxNode;
	

}


void Random_Attack(Node& S, Node& D, Node *nodesPtr, const int nodeAmount, const int totalLink)
{
	Link *linkPtr;
	int randNode;
	int randLink;
	int randCho;
	bool done = false;

	while (done != true)
	{
		randNode = rand() % nodeAmount;
		if ((nodesPtr + randNode)->getLinksAmount() != 0)
		{
			randLink = rand() % (nodesPtr + randNode)->getLinksAmount(); //gets random link from randomnode
			linkPtr = (nodesPtr + randNode)->getLinks();
			if ((linkPtr + randLink)->getAlive() == true && (linkPtr + randLink)->getBreak() == true) //alive and breakable
			{
				thisLInkisDead = randLink;
				thisNodeisDead = randNode;
				(linkPtr + randLink)->setAlive(false);//it am dead
			}
		}


		cout << "Attack has finished. Chosen node " << randNode << " and link " << randLink << endl;
	}
}