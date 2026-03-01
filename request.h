#ifndef REQUEST_H
#define REQUEST_H
#include <random>

class request {
public:
    int IP_in;
    int IP_out;
    int time;
    char job_type;
};

request generate_request(std::mt19937 &eng,
                         std::uniform_int_distribution<int> &ips,
                         std::uniform_int_distribution<int> &times,
                         std::uniform_int_distribution<int> &job);
#endif