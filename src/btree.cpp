#include <iostream>

#include "btree.h"
#include "misc.h"

void BTree::insert(std::shared_ptr<GraphNode> graph_node) {
	std::shared_ptr<TreeNode> new_node = std::make_shared<TreeNode>();
	new_node->contents = graph_node;

	float new_lng = new_node->contents->longitude;
	float new_lat = new_node->contents->latitude;

	if (root == nullptr) {
		root = std::move(new_node);
		return;
	}

	std::shared_ptr<TreeNode> curr = root;

	while (true) {
		float curr_lng = curr->contents->longitude;
		float curr_lat = curr->contents->latitude;

		if (new_lng <= curr_lng && new_lat <= curr_lat) {
			if (curr->lng_lt_lat_lt == nullptr) {
				curr->lng_lt_lat_lt = std::move(new_node);
				return;
			}
			curr = curr->lng_lt_lat_lt;
		} else if (new_lng > curr_lng && new_lat <= curr_lat) {
			if (curr->lng_gt_lat_lt == nullptr) {
				curr->lng_gt_lat_lt = std::move(new_node);
				return;
			}
			curr = curr->lng_gt_lat_lt;
		} else if (new_lng <= curr_lng && new_lat > curr_lat) {
			if (curr->lng_lt_lat_gt == nullptr) {
				curr->lng_lt_lat_gt = std::move(new_node);
				return;
			}
			curr = curr->lng_lt_lat_gt;
		} else {
			if (curr->lng_gt_lat_gt == nullptr) {
				curr->lng_gt_lat_gt = std::move(new_node);
				return;
			}
			curr = curr->lng_gt_lat_gt;
		}
	}
}

unsigned int BTree::get_closest_node_id(float lng, float lat) {
	std::shared_ptr<TreeNode> curr = root;

	unsigned int closest_id = root->contents->id;
	float closest_distance = compute_distance(lng, lat, root->contents->longitude, root->contents->latitude);

	while (curr != nullptr) {
		float curr_lng = curr->contents->longitude;
		float curr_lat = curr->contents->latitude;

		float distance = compute_distance(lng, lat, curr_lng, curr_lat);

		if (distance < closest_distance) {
			closest_id = curr->contents->id;
			closest_distance = distance;
		}

		if (lng <= curr_lng && lat <= curr_lat) {
			curr = curr->lng_lt_lat_lt;
		} else if (lng > curr_lng && lat <= curr_lat) {
			curr = curr->lng_gt_lat_lt;
		} else if (lng <= curr_lng && lat > curr_lat) {
			curr = curr->lng_lt_lat_gt;
		} else {
			curr = curr->lng_gt_lat_gt;
		}
	}

	return closest_id;
}