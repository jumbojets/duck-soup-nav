#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include <cstdlib>

#include "roadnet.h"

RoadNet::RoadNet(std::string dbnodes, std::string dbedges) {
	std::string line, item;	

	std::cout << "loading nodes from " << dbnodes << "... ";

	std::ifstream nodefile (dbnodes);

	if (nodefile.is_open()) {

		while (std::getline(nodefile, line)) {
			std::stringstream ss (line);

			std::shared_ptr<GraphNode> node = std::make_shared<GraphNode>();

			std::getline(ss, item, ' ');
			node->id = std::stoi(item);

			std::getline(ss, item, ' ');
			node->longitude = std::stof(item);

			std::getline(ss, item, ' ');
			node->latitude = std::stof(item);

			add_node(node);
		}
	} else {
		std::cout << std::endl << "Error: could not open nodes file: " << dbnodes << std::endl;
		exit(1);
	}

	std::cout << "done" << std::endl;

	std::cout << "loading edges from " << dbedges << "... ";

	std::ifstream edgefile (dbedges);

	if (edgefile.is_open()) {

		while (std::getline(nodefile, line)) {
			std::stringstream ss (line);

			std::getline(ss, item, ' ');  /* skip the edge id--we dont need it */
			std::getline(ss, item, ' ');
			unsigned int id1 = std::stoi(item);

			std::getline(ss, item, ' ');
			unsigned int id2 = std::stoi(item);

			std::getline(ss, item, ' ');
			float distance = std::stof(item);

			add_edge(id1, id2, distance);
			add_edge(id2, id1, distance);
		}
	} else {
		std::cout << std::endl << "Error: could not open edges file: " << dbedges << std::endl;
		exit(1);
	}

	std::cout << "done" << std::endl;

	std::cout << "proprogating the btree... ";

	for (const auto & [ _, node ] : nodes_meta) {
		tree.insert(node);
	}

	std::cout << "done" << std::endl;
}

void RoadNet::add_edge(unsigned int id1, unsigned int id2, float distance) {
	std::unordered_map<unsigned int, std::unique_ptr<Edges> >::const_iterator got;

	got = graph.find(id1);

	if (got == graph.end()) {
		std::cout << "Error: Bad edges file! Cannot add edge when node id does not already exist: " << id1 << std::endl;
		exit(1);
	} else {
		/* found the edge in the graph. the adjacency map is stored in parameter second */
		got->second->insert(std::make_pair(id2, distance));
	}
}

void RoadNet::add_node(std::shared_ptr<GraphNode> node) {
	nodes_meta.insert(std::make_pair(node->id, node));


	/* 
	 * This will be prefered when we have self balancing in the btree.
	 * Currently, insertion into the btree occurs at the end of the
	 * RoadNet constructor by iterating throught he hashmap, ensuring some
	 * sort of 'randomness' while inserting, making it generally balanced
	 */
	// tree.insert(node);

	Edges *edge = new Edges();
	graph.insert(std::make_pair(node->id, edge));
}
