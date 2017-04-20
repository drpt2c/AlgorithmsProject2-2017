#ifndef LINK_H
#define LINK_H
#include <iostream>

class Link
{
    public:
        Link();
        Link(int s, int t, bool a);
        int getSource() { return source; }
        void setSource(int val) { source = val; } 
        int getTarget() { return target; }
        void setTarget(int val) { target = val; }
        bool getAlive() { return alive; } 
        void setAlive(bool val) { alive = val; } 
    private:
        int source;
        int target;
        bool alive;
};

#endif
