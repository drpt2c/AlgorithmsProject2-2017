#include "link.h"

using namespace std;

Link::Link()
{
    source = -1;
    target = -1;
    alive = false;
    capacity = -1;
}

Link::Link(int s, int t, int c, bool a)
{
    source = s;
    target = t;
    capacity = c;
    alive = a;
}