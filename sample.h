#ifndef SAMPLE_H
#define SAMPLE_H

#include <string>
#include <vector>

class Sample {

private:

    std::string timestamp;
    std::string recordTime;
    int         horizontalIntensity;
    int         verticalIntensity;

    std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &items);

public:
    Sample(std::string line);
    std::string time;
};

#endif // SAMPLE_H
