#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <set>
#include <unordered_set>
#include <cmath>
#include <algorithm>

#include "ducksoup/roadnet.h"

RoadNet::RoadNet(std::string dbnodes, std::string dbedges) {
	std::string line, item;	

	std::cout << "loading nodes from " << dbnodes << "..." << std::endl;;

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

	graph.shrink_to_fit();  /* dont need to add anymore */

	std::cout << "loading edges from " << dbedges << "..." << std::endl;

	std::ifstream edgefile (dbedges);

	if (edgefile.is_open()) {

		while (std::getline(edgefile, line)) {
			std::stringstream ss (line);

			std::getline(ss, item, ' ');  /* skip the edge id--we dont need it */
			std::getline(ss, item, ' ');
			unsigned int id1 = std::stoi(item);

			std::getline(ss, item, ' ');
			unsigned int id2 = std::stoi(item);

			float lng1 = nodes_meta[id1]->longitude;
			float lat1 = nodes_meta[id1]->latitude;
			float lng2 = nodes_meta[id2]->longitude;
			float lat2 = nodes_meta[id2]->latitude;

			float distance = compute_distance(lng1, lat1, lng2, lat2);

			add_edge(id1, id2, distance);
			add_edge(id2, id1, distance);
		}
	} else {
		std::cout << std::endl << "Error: could not open edges file: " << dbedges << std::endl;
		exit(1);
	}

	nodes_meta.shrink_to_fit();  /* don't need to add anymore */

	std::cout << "proprogating the rtree... " << std::endl;

	for (auto const & node : nodes_meta)
		tree.insert(node);

}

std::vector<std::shared_ptr<GraphNode> > RoadNet::route(float lng_start, float lat_start, float lng_dest, float lat_dest) {
	std::cout << "running routing algorithm... " << std::endl;

	unsigned int start_node_id = tree.get_closest_node_id(lng_start, lat_start);
	unsigned int dest_node_id = tree.get_closest_node_id(lng_dest, lat_dest);

	std::set<std::shared_ptr<visitedlocation>> openlist;  /* this will emulate a priority queue */
	std::unordered_map<unsigned int, std::shared_ptr<visitedlocation>> openlist_id_map;  /* so we can find the item in the openset based on id */
	std::unordered_map<unsigned int, std::shared_ptr<visitedlocation>> closedlist;

	std::shared_ptr<visitedlocation> curr = std::make_shared<visitedlocation>();
	curr->id = start_node_id;
	curr->g = 0;
	curr->f = 0;

	openlist.insert(curr);
	openlist_id_map.insert(std::make_pair(start_node_id, curr));

	while (true) {
		curr = *openlist.begin();
		openlist.erase(curr);
		openlist_id_map.erase(curr->id);

		closedlist.insert(std::make_pair(curr->id, curr));

		if (curr->id == dest_node_id)
			break;

		for (auto const & [ next_id, distance ] : *graph[curr->id]) {
			std::shared_ptr<visitedlocation> successor = std::make_shared<visitedlocation>();
			successor->id = next_id;
			successor->parent = curr.get();
			successor->g = successor->parent->g + distance;

			std::shared_ptr<GraphNode> n = nodes_meta[curr->id];

			float lng = n->longitude;
			float lat = n->latitude;
			float h = compute_distance(lng, lat, lng_dest, lat_dest) * sqrt(2);  /* a* heuristic */

			successor->f = successor->g + h;

			/* skip if this node is already in the open set and has smaller f */

			auto got = openlist_id_map.find(successor->id);

			if (got != openlist_id_map.end() && got->second->f < successor->f) {
				continue;
			}

			/* skip if this node is in the closed set and has smaller f */

			got = closedlist.find(successor->id);

			if (got != closedlist.end() && got->second->f < successor->f) {
				continue;
			}

			/* add node to open list */
			openlist.insert(successor);
			openlist_id_map.insert(std::make_pair(successor->id, successor));
		}
	}

	/* final node is stored in curr. trace back and build the path */
	std::vector<std::shared_ptr<GraphNode> > path = build_path(curr.get());

	return path;
}

void RoadNet::add_edge(unsigned int id1, unsigned int id2, float distance) {
	graph[id1]->insert(std::make_pair(id2, distance));
}

void RoadNet::add_node(std::shared_ptr<GraphNode> node) {
	nodes_meta.push_back(std::move(node));

	/* 
	 * This will be prefered when we have self balancing in the rtree.
	 * Currently, insertion into the btree occurs at the end of the
	 * RoadNet constructor by iterating throught he hashmap, ensuring some
	 * sort of 'randomness' while inserting, making it generally balanced
	 */
	// tree.insert(node);

	std::shared_ptr<Edges> edge = std::make_shared<Edges>();
	graph.push_back(std::move(edge));
}

std::vector<std::shared_ptr<GraphNode> > RoadNet::build_path(visitedlocation *node) {
	std::vector<std::shared_ptr<GraphNode> > path;

	while (node != nullptr) {
		/* get the corresponding graph node with all important information */
		std::shared_ptr<GraphNode> out = nodes_meta[node->id];
		path.push_back(out);
		node = node->parent;
	}

	/* reverse because we intially started at the end */
	std::reverse(path.begin(), path.end());

	return path;
}