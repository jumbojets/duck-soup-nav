#include <cmath>

#include "ducksoup/misc.h"

#define EARTH_RADIUS 6371.0

/*
 * converts degrees to radians
 *
 * @param deg - angle as degrees
 * @param rad - angle as radians
 */
inline float deg2rad(float deg) {
    return (deg * M_PI / 180);
}

/*
 * converts radians to degrees
 *
 * @param rad - angle as radians
 * @param deg - angle as degrees
 */
inline float rad2deg(float rad) {
    return (rad * 180 / M_PI);
}

namespace ducksoup {

float compute_distance(float lng1d, float lat1d, float lng2d, float lat2d) {
    float lng1r = deg2rad(lng1d);
    float lat1r = deg2rad(lat1d);
    float lng2r = deg2rad(lng2d);
    float lat2r = deg2rad(lat2d);

    float u = sin((lat2r - lat1r) / 2);
    float v = sin((lng2r - lng1r) / 2);

    return 2.0 * EARTH_RADIUS * asin(sqrt(u * u + cos(lat1r) * cos(lat2r) * v * v));
}

}

