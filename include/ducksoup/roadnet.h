#ifndef ROADNET_H
#define ROADNET_H

#include <unordered_map>
#include <memory>

#include "btree.h"
#include "misc.h"

/*
 * the type Edges is an adjacency map for the edges of each node
 * the type Graph maps each node to an adjacency map
 * the type NodesMeta allows for node meta data lookup by node id
*/

typedef std::unordered_map<unsigned int, float> Edges;
typedef std::unordered_map<unsigned int, std::unique_ptr<Edges> > Graph;
typedef std::unordered_map<unsigned int, std::shared_ptr<GraphNode> > NodesMeta;

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
};

#endif
