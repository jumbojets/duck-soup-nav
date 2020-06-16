#include <iostream>

#include "RoadNet.h"

int main() {
	/* these will become command line arguments */
	std::string dbnodes = "cal_node.txt";
	std::string dbedges = "cal_edge.txt";

	RoadNet* x = new RoadNet(dbnodes, dbedges);

	free(x);

	return 0;
}
