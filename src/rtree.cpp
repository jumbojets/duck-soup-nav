#include <iostream>

#include "ducksoup/rtree.h"
#include "ducksoup/misc.h"

/*
 * represents the type of child a certain node pertains two with
 * respect to the current parent. LNG / LAT stands for longitude /
 * latitude and LT / GT stands for less than / greater than.
 */
enum ChildCategory {
    LNG_LT_LAT_LT,
    LNG_GT_LAT_LT,
    LNG_LT_LAT_GT,
    LNG_GT_LAT_GT
};

/*
 * resolves a comparison of two pairs of coordinates as the enum ChildCategory
 *
 * @param lng1 - longitude of first point in decimal format
 * @param lat1 - latitude of first point in decimal format
 * @param lng2 - longitude of second point in decimal format
 * @param lat2 - latitude of second point in decimal format
 * @return Key - resolved key as an enum
 */
ChildCategory resolve_child_category(float lng1, float lat1, float lng2, float lat2) {
    if (lng1 <= lng2 && lat1 <= lat2) {
        return LNG_LT_LAT_LT;
    } else if (lng1 > lng2 && lat1 <= lat2) {
        return LNG_GT_LAT_LT;
    } else if (lng1 <= lng2 && lat1 > lat2) {
        return LNG_LT_LAT_GT;
    } else {
        return LNG_GT_LAT_GT;
    }
}

namespace ducksoup {

void RTree::insert(std::shared_ptr<GraphNode> graph_node) {
    std::shared_ptr<TreeNode> new_node = std::make_shared<TreeNode>();
    new_node->contents = graph_node;

    float new_lng = new_node->contents->longitude;
    float new_lat = new_node->contents->latitude;

    if (root == nullptr) {
        root = std::move(new_node);
        return;
    }

    std::shared_ptr<TreeNode> curr = root;

    for (;;) {
        float curr_lng = curr->contents->longitude;
        float curr_lat = curr->contents->latitude;

        ChildCategory cc = resolve_child_category(new_lng, new_lat, curr_lng, curr_lat);

        switch (cc) {
            case LNG_LT_LAT_LT:
                if (curr->lng_lt_lat_lt == nullptr) {
                    curr->lng_lt_lat_lt = std::move(new_node);
                    return;
                }
                curr = curr->lng_lt_lat_lt;
                break;

            case LNG_GT_LAT_LT:
                if (curr->lng_gt_lat_lt == nullptr) {
                    curr->lng_gt_lat_lt = std::move(new_node);
                    return;
                }
                curr = curr->lng_gt_lat_lt;
                break;

            case LNG_LT_LAT_GT:
                if (curr->lng_lt_lat_gt == nullptr) {
                    curr->lng_lt_lat_gt = std::move(new_node);
                    return;
                }
                curr = curr->lng_lt_lat_gt;
                break;

            case LNG_GT_LAT_GT:
                if (curr->lng_gt_lat_gt == nullptr) {
                    curr->lng_gt_lat_gt = std::move(new_node);
                    return;
                }
                curr = curr->lng_gt_lat_gt;
                break;
        }
    }
}

unsigned int RTree::get_closest_node_id(float lng, float lat) {
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

        ChildCategory cc = resolve_child_category(lng, lat, curr_lng, curr_lat);

        switch (cc) {
            case LNG_LT_LAT_LT:
                curr = curr->lng_lt_lat_lt;
                break;

            case LNG_GT_LAT_LT:
                curr = curr->lng_gt_lat_lt;
                break;

            case LNG_LT_LAT_GT:
                curr = curr->lng_lt_lat_gt;
                break;

            case LNG_GT_LAT_GT:
                curr = curr->lng_gt_lat_gt;
        }
    }

    return closest_id;
}
    
}