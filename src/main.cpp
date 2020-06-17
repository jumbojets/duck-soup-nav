#include <iostream>

#include "roadnet.h"

int main() {
	/* these will become command line arguments */
	std::string dbnodes = "cal_node.txt";
	std::string dbedges = "cal_edge.txt";

	RoadNet x = RoadNet(dbnodes, dbedges);

	return 0;
}
