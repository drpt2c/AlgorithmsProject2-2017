#ifndef LINK_H
#define LINK_H
#include <iostream>

class Link
{
    public:
        Link();
        Link(int s, int t, int c, bool a);
        int getSource() { return source; }
        void setSource(int val) { source = val; } 
        int getTarget() { return target; }
        void setTarget(int val) { target = val; }
        int getCapacity() { return capacity; }
        void setCapacity(int val) { capacity = val; }
        bool getAlive() { return alive; } 
        void setAlive(bool val) { alive = val; } 
    private:
        int source;
        int target;
        int capacity;
        bool alive;
};

#endif
