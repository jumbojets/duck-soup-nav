#include <iostream>

#include "RoadNet.h"

int main() {
	std::string dbnode = "cal_node.txt";
	std::string dbedge = "cal_edge.txt";

	RoadNet* x = new RoadNet(dbnode, dbedge);

	free(x);
}
