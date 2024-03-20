//
// Created by Paul Cirstean on 05.12.2023.
//

#ifndef SHARED_LOCATION_H
#define SHARED_LOCATION_H

#include <iostream>

struct Coordinates {
    double lat;
    double log;
};
class Location {
    std::string location;
private: Coordinates GetCoordinates();
public: Location(std::string location) : location(location) {}
public: static double GetDistance(Location, Location);
};


#endif //CDN_NODE_LOCATION_H
