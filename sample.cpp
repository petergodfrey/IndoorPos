#include "sample.h"

#include <cmath>
#include <sstream>
#include <iostream>

#define PI 3.14159

Sample::Sample(std::string line) {
    std::vector<std::string> items;
    split(line, ',', items);
    time                = items[0];
    timestamp           = items[1];
    recordTime          = items[2];
    double ax           = std::stod(items[3]);
    double ay           = std::stod(items[4]);
    double az           = std::stod(items[5]);
    double bx           = std::stod(items[6]);
    double by           = std::stod(items[7]);
    double bz           = std::stod(items[8]);
    double aIntensity   = (ax * ax) + (ay * ay) + (az * az);
    double bIntensity   = (bx * bx) + (by * by) + (bz * bz);
    double theta        = (PI / 2) - acos(aIntensity /
                          ( sqrt(aIntensity) * sqrt(bIntensity) ) );
    horizontalIntensity = bIntensity * cos(theta);
    verticalIntensity   = bIntensity * sin(theta);
}

std::vector<std::string> &Sample::split(const std::string &s, char delim, std::vector<std::string> &items) {
    std::stringstream ss(s);
    std::string item;
    while ( std::getline(ss, item, delim) ) {
        items.push_back(item);
    }
    return items;
}
