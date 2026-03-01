#include "webserver.h"

webserver::webserver(int id_) : id(id_), busy(false), remaining(0) {}

void webserver::take_request(const request &req, int current_time, std::ofstream &log) {
    busy = true;
    remaining = req.time;
    log << "Time " << current_time
        << ": Server " << id
        << " assigned request from IP "
        << req.IP_in << " to "
        << req.IP_out << "\n";
}

void webserver::tick(int current_time, std::ofstream &log) {
    if (busy) {
        remaining--;
        if (remaining <= 0) {
            busy = false;
            log << "Time " << current_time
                << ": Server " << id
                << " completed request\n";
        }
    }
}