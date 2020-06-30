#ifndef RTREE_H
#define RTREE_H

#include "misc.h"

namespace ducksoup {

	/*
	 * The class RTree is used for lookup to determine the nearest node to a specific
	 * latitude and longitude pair. It performs self balancing after insertion.
	 */

	class RTree {
		public:

			/*
			 * insert adds a new TreeNode to the RTree by traversing to where there is
			 * an free spot to be placed. It uses the longitude and latitude of the
			 * underlying wraped graph node to index it in the best spot. Note that currently
			 * it does not implement self-balancing.
			 *
			 * @param graph_node - a shared pointer of the graph node to be wrapped and stored
			 */

			void insert(std::shared_ptr<ducksoup::GraphNode> graph_node);

			/*
			 * searches for the id of the nearest node to a certain longitude, latitude pair
			 * 
			 * @param longitude - longitude of the point under search
			 * @param latitude - latitude of the point under search
			 */

			unsigned int get_closest_node_id(float longitude, float latitude);

		private:

			std::shared_ptr<TreeNode> root;  /* the root node for the b-tree */
	};

}

#endif