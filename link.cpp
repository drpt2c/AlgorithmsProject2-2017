#include "link.h"

using namespace std;

Link::Link()
{
    source = -1;
    target = -1;
    alive = false;
}

Link::Link(int s, int t, bool a)
{
    source = s;
    target = t;
    alive = a;
}
