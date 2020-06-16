#ifndef ROAD_NET_H
#define ROAD_NET_H

#include <unordered_map>
#include <memory>

/*
 * The struct Node represents a node on the RoadNet. There is an unique 
 * id associated with each node given by the data file as well as 
 * a positioning longitude and latitude.
 */

typedef struct Node {
	unsigned int id;
	float longitude;
	float latitude;
} Node;

/*
 * the type edges is an adjacency map for the edges of each node
 * the type graph_t maps each node to an adjacency map
 * the type node_m allows for node meta data lookup by node id
*/

typedef std::unordered_map<unsigned int, float> edges;
typedef std::unordered_map<unsigned int, std::unique_ptr<edges> > graph_t;
typedef std::unordered_map<unsigned int, std::unique_ptr<Node> > nodes_m;

/*
 * RoadNet stores a road network for a given region as a graph in memory
 * using adjacency maps. It also stores meta data (longitude / latitude)
 * for each node in the graph.
 */

class RoadNet {
	private:

		graph_t graph;       /* in memory graph data structure */		
		nodes_m nodes_meta;  /* accessor for node struct by id */

		/*
		 * AddEdge creates an edge between two nodes, adding id2 to the
		 * adjacency map of id1 and storing the associated distance
		 *
		 * @param id1 - id of the first node, the one that will own the edge
		 * @param id2 - id of the second node, the one that will be in the edge
		 * @param distance - distance between the two nodes
		 */

		void AddEdge(unsigned int id1, unsigned int id2, float distance);

		/*
		 * AddNode creates a node in the graph and creates its adjacency map
		 *
		 * @param node - pointer to node struct to insert
		 */

		void AddNode(Node* node);

	public:

		/*
		 * Constructor for RoadNet
		 *
		 * @param dbnodes - path to text file of the road network nodes
		 * @param dbedges - path to text file of the road nexwork edges
		 */

		RoadNet(std::string dbnodes, std::string dbedges);
};

#endif
