#include <iostream>

#include "roadnet.h"

int main() {
	/* these will become command line arguments */
	std::string dbnodes = "cal_node.txt";
	std::string dbedges = "cal_edge.txt";

	float lng_start = -122.424660;
	float lat_start = 41.985020;

	float lng_dest = -123.508522;
	float lat_dest = 38.857872;

	RoadNet net = RoadNet(dbnodes, dbedges);

	std::vector<std::shared_ptr<GraphNode> > path = net.route(lng_start, lat_start, lng_dest, lat_dest);

	return 0;
}
