
#include "Node.h"
#include "SuperBlock.h"
#include "Disc.h"


Node NewNodeInstance()
{
	Node node;
	node.size = UNUSED;
	return node;
}