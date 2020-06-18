#ifndef ROADNET_H
#define ROADNET_H

#include <unordered_map>
#include <memory>
#include <vector>

#include "btree.h"
#include "misc.h"

/* 
 * struct used by a* to represent a node. it has the ability to be
 * sorted and used in a set by picking the minimum f value
 */
struct visitedlocation {
	unsigned int id;
	float f;
	float g;
	visitedlocation *parent;
	bool operator<(const visitedlocation& rhs) const {
		return f < rhs.f;
	}
};

/*
 * the type Edges is an adjacency map for the edges of each node
 * the type Graph maps each node to an adjacency map
 * the type NodesMeta allows for node meta data lookup by node id
*/

typedef std::unordered_map<unsigned int, float> Edges;
typedef std::vector<std::shared_ptr<Edges> > Graph;
typedef std::vector<std::shared_ptr<GraphNode> > NodesMeta;

/*
 * RoadNet stores a road network for a given region as a graph in memory
 * using adjacency maps. It also stores meta data (longitude / latitude)
 * for each node in the graph.
 */

class RoadNet {
	public:

		/*
		 * Constructor for RoadNet
		 *
		 * @param dbnodes - path to text file of the road network nodes
		 * @param dbedges - path to text file of the road nexwork edges
		 */

		RoadNet(std::string dbnodes, std::string dbedges);

		/*
		 * Using a* search algorithm to compute and return a path to take to
		 * get from start location to destination.
		 *
		 * @param lng_start - longitude of starting location
		 * @param lat_start - latitude of starting location
		 * @param lng_dest - longtiude of destination
		 * @param lat_dest - latitude of destination
		 * @return path - vector of GraphNodes that represent the path computed
		 */

		std::vector<std::shared_ptr<GraphNode> > route(float lng_start, float lat_start, float lng_dest, float lat_dest);

	private:

		Graph graph;           /* in memory graph data structure */		
		NodesMeta nodes_meta;  /* accessor for node struct by id */
		BTree tree;            /* b-tree for searching nearest node based on coordinates */

		/*
		 * add_edge creates an edge between two nodes, adding id2 to the
		 * adjacency map of id1 and storing the associated distance
		 *
		 * @param id1 - id of the first node, the one that will own the edge
		 * @param id2 - id of the second node, the one that will be in the edge
		 * @param distance - distance between the two nodes
		 */

		void add_edge(unsigned int id1, unsigned int id2, float distance);

		/*
		 * add_node creates a node in the graph and creates its adjacency map
		 *
		 * @param node - pointer to node struct to insert
		 */

		void add_node(std::shared_ptr<GraphNode> node);

		/*
		 * Builds a path of GraphNodes from a final node. It traverses backward from
		 * each visitedlocation's parent node
		 *
		 * @param node - the final node to traverse backwards from
		 * @return path - the generated path
		 */

		std::vector<std::shared_ptr<GraphNode> > build_path(visitedlocation *node);
};

#endif
