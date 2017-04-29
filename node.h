#ifndef NODE_H
#define NODE_H
#include "link.h"
using namespace std;

class Node
{
    public:
        Node();
        int getVertex() { return vertex; }
        void setVertex(int val) { vertex = val; }
        Link *getLinks() 
        { 
            linksPtr = links;
            return linksPtr; 
        }
        void addLink(Link l)
        {
            links[linksAmount] = l;
            linksAmount++;
        }
        int getLinksAmount() { return linksAmount; }
        bool getVisited() { return visited; }
        void setVisited(bool val) { visited = val; }
        int getDistance() { return distance; }
        void setDistance(int val) { distance = val; }
        void incrementDistance() { distance++; }
        int getParent() { return parent; }
        void setParent(int val) { parent = val; }
    private:
        int vertex;
        Link links[100];
        Link *linksPtr;
        int linksAmount;
        bool visited;
        int distance;
        int parent;
    
};

#endif
