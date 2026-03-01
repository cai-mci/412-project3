#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <fstream>
#include "request.h"

class webserver {
public:
    bool busy;
    int remaining;
    int id;

    webserver(int id_);

    void take_request(const request &req, int current_time, std::ofstream &log);
    void tick(int current_time, std::ofstream &log);
};

#endif