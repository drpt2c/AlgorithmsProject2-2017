#include "link.h"

using namespace std;

Link::Link()
{
    source = -1;
    target = -1;
    alive = true;
    capacity = 1;
    flow = 0;
    breakable = true;
}

Link::Link(int s, int t, int c, bool a)
{
    source = s;
    target = t;
    capacity = c;
    alive = a;
    flow = 0;
    breakable = true;
}
