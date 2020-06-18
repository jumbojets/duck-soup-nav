#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <set>
#include <unordered_set>

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

std::vector<std::shared_ptr<GraphNode> > RoadNet::route(float lng_start, float lat_start, float lng_dest, float lat_dest) {
	unsigned int start_node_id = tree.get_closest_node_id(lng_start, lat_start);
	unsigned int dest_node_id = tree.get_closest_node_id(lng_dest, lat_dest);

	std::set<visitedlocation*> openlist;  /* this will emulate a priority queue */
	std::unordered_map<unsigned int, visitedlocation*> openlist_id_map;  /* so we can find the item in the openset based on id */
	std::unordered_map<unsigned int, visitedlocation*> closedlist;

	visitedlocation *curr = new visitedlocation{start_node_id, 0, 0};

	openlist.insert(curr);
	openlist_id_map.insert(std::make_pair(start_node_id, curr));

	while (true) {
		curr = *openlist.begin();
		openlist.erase(curr);
		openlist_id_map.erase(curr->id);

		closedlist.insert(std::make_pair(curr->id, curr));

		if (curr->id == dest_node_id)
			break;

		/* loop over all edges */

		std::cout << graph.size() << std::endl;

		for (const auto & [ next_id, distance ] : *graph[curr->id]) {
			std::cout << next_id << std::endl;

			visitedlocation *successor = new visitedlocation();
			successor->id = next_id;
			successor->parent = curr;
			successor->g = successor->parent->g + distance;

			std::shared_ptr<GraphNode> n = nodes_meta[curr->id];

			float lng = n->longitude;
			float lat = n->latitude;
			float h = compute_distance(lng, lat, lng_dest, lat_dest) * sqrt(2);  /* a* heuristic */

			successor->f = successor->g + h;

			/* skip if this node is already in the open set and has smaller */

			auto got = openlist_id_map.find(successor->id);

			if (got != openlist_id_map.end() && got->second->f < successor->f) {
				free(successor);
				continue;
			}

			/* skip if this node is in the closed set and has smaller f */

			got = closedlist.find(successor->id);

			if (got != closedlist.end() && got->second->f < successor->f) {
				free(successor);
				continue;
			}

			/* add node to open list */
			openlist.insert(successor);
			openlist_id_map.insert(std::make_pair(successor->id, successor));
		}
	}

	/* final node is stored in curr. trace back and build the path */
	std::vector<std::shared_ptr<GraphNode> > path = build_path(curr);

	/* free all variables in openlist and closedlist */

	for (auto f : openlist)
		free(f);

	for (const auto & [ _, f ] : closedlist)
		free(f);

	return path;
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

std::vector<std::shared_ptr<GraphNode> > RoadNet::build_path(visitedlocation *node) {
	std::vector<std::shared_ptr<GraphNode> > path;

	while (node != nullptr) {
		/* get the corresponding graph node with all important information */
		std::shared_ptr<GraphNode> out = nodes_meta[node->id];
		path.push_back(out);
	}

	/* reverse because we intially started at the end */
	std::reverse(path.begin(), path.end());

	return path;
}