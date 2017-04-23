#include "node.h"

using namespace std;

Node::Node()
{
    vertex = -1;
    visited = false;
    linksPtr = links;
    linksAmount = 0;
}
