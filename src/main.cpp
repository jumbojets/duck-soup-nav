#include <iostream>

#include "roadnet.h"

int main() {
	/* these will become command line arguments */
	std::string dbnodes = "cal_node.txt";
	std::string dbedges = "cal_edge.txt";

	float lng_start = -122.442436;
	float lat_start = 41.977470;

	float lng_dest = -122.965858;
	float lat_dest = 42.045238;

	RoadNet net = RoadNet(dbnodes, dbedges);

	std::vector<std::shared_ptr<GraphNode> > path = net.route(lng_start, lat_start, lng_dest, lat_dest);

	std::cout << "route: ";

	for (const auto & n : path) {
		std::cout << n->id << " ";
	}

	std::cout << std::endl;

	return 0;
}
