//
// Created by Paul Cirstean on 05.12.2023.
//
#include <cmath>
#include <algorithm>
#define PI 3.14159265358979323846
#define TERRA_RADIUS 6356.752
#define RADIANS PI / 180
#include "Location.h"

double Location::GetDistance(Location from, Location to) {
    auto coordinates_from = from.GetCoordinates();
    auto coordinates_to = to.GetCoordinates();

    coordinates_from.lat  = coordinates_from.lat  * RADIANS;
    coordinates_from.log = coordinates_from.log * RADIANS;
    coordinates_to.lat  = coordinates_to.lat  * RADIANS;
    coordinates_to.log = coordinates_to.log * RADIANS;

    double haversine = (pow(sin((1.0 / 2) * (coordinates_to.lat - coordinates_from.lat)), 2)) +
            ((cos(coordinates_from.lat)) * (cos(coordinates_to.lat)) * (pow(sin((1.0 / 2) * (coordinates_to.log - coordinates_from.log)), 2)));
    double temp = 2 * asin(std::min(1.0, sqrt(haversine)));

    return TERRA_RADIUS * temp;
}

Coordinates Location::GetCoordinates() {
    // consider N and E positive and S and V negative.

    if(location == "en") {
        return {52.3555, -1.1743};
    }

    if(location == "fr") {
        return {46.2276, 2.2137};
    }

    if(location == "usa") {
        return {37.0902, -95.7129};
    }

    if(location == "rus") {
        return {61.5240, 105.3188};
    }

    if(location == "china") {
        return {35.8617, 104.1954};
    }

    return {0, 0};
}

