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
    private:
        int vertex;
        Link links[20];
        Link *linksPtr;
        int linksAmount;
        bool visited;
    
};

#endif
