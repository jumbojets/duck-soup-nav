#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>

#include "RoadNet.h"

RoadNet::RoadNet(std::string dbnodes, std::string dbedges) {
	std::string line, item;	

	std::cout << "loading nodes from " << dbnodes << "... ";

	std::ifstream nodefile (dbnodes);

	if (nodefile.is_open()) {

		while (std::getline(nodefile, line)) {
			std::stringstream ss (line);

			Node* node = new Node();  /* will be converted to unique_ptr upon insertion */

			std::getline(ss, item, ' ');
			node->id = std::stoi(item);

			std::getline(ss, item, ' ');
			node->longitude = std::stof(item);

			std::getline(ss, item, ' ');
			node->latitude = std::stof(item);

			nodes_meta.insert(std::make_pair(node->id, node));
		}
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

			AddEdge(id1, id2, distance);
			AddEdge(id2, id1, distance);
		}
	}

	std::cout << "done" << std::endl;
}

void RoadNet::AddEdge(unsigned int id1, unsigned int id2, float distance) {
	std::unordered_map<unsigned int, std::unique_ptr<edges> >::const_iterator got;

	got = graph.find(id1);

	if (got == graph.end()) {
		/* need to make a new edge entry */
		edges* edge = new edges();  /* will be converted to unique_ptr upon insertion */
		edge->insert(std::make_pair(id2, distance));
		graph.insert(std::make_pair(id1, edge));
	} else {
		got->second->insert(std::make_pair(id2, distance));
	}
}