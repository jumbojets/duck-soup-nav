#ifndef MISC_H
#define MISC_H

#include <memory>

/*
 * The struct GraphNode represents a node on the RoadNet. There is an unique 
 * id associated with each node given by the data file as well as 
 * a positioning longitude and latitude.
 */

typedef struct GraphNode {
	unsigned int id;
	float longitude;
	float latitude;
} GraphNode;

/*
 * The struct TreeNode is a node for the tree RTree. It wraps a GraphNode which
 * has the unique information about the node on the graph (id, latitude, longitude).
 * This simply manages position in the r-tree
 */

typedef struct TreeNode {
	std::shared_ptr<GraphNode> contents;
	std::shared_ptr<TreeNode> lng_lt_lat_lt;
	std::shared_ptr<TreeNode> lng_lt_lat_gt;
	std::shared_ptr<TreeNode> lng_gt_lat_lt;
	std::shared_ptr<TreeNode> lng_gt_lat_gt;
} TreeNode;

/*
 * computes the distance between two pairs of coordinates
 *
 * @param lng1d - longitude of first point in decimal format
 * @param lat1d - latitude of first point in decimal format
 * @param lng2d - longitude of second point in decimal format
 * @param lat2d - latitude of second point in decimal format
 */
float compute_distance(float lng1d, float lat1d, float lng2d, float lat2d);

#endif