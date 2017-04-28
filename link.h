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
        int getFlow() {return flow; }
        void setFlow(int val) { flow = val; }
        void setCapacity(int val) { capacity = val; }
        bool getAlive() { return alive; } 
        bool getBreak() {return breakable; }
        void setAlive(bool val) { alive = val; } 
    private:
        int source;
        int target;
        int capacity;   //total avaliable flow
        int flow;       //the current amount of the capacity used
        bool alive;
        bool breakable; //if this is included in K, then it cannot be broken every
        
};

#endif
