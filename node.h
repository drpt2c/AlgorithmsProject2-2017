#ifndef NODE_H
#define NODE_H

using namespace std;

class Node
{
    public:
        Node();
        Node(int val);
        int getID() { return ID; }
        void setID(int val) { ID = val; }
    private:
        int ID;
    
};

#endif
