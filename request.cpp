#include "request.h"
#include <random>

request generate_request(std::mt19937 &eng,
                         std::uniform_int_distribution<int> &ips,
                         std::uniform_int_distribution<int> &times,
                         std::uniform_int_distribution<int> &job) {

    request new_request;
    new_request.IP_in = ips(eng);
    new_request.IP_out = ips(eng);
    new_request.time = times(eng);
    new_request.job_type = (job(eng) == 1) ? 'S' : 'T';

    return new_request;
}