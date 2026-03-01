#include <iostream>
#include <chrono>
#include <random>
#include <fstream>
#include <queue>
#include "request.h"
#include "webserver.h"


class load_balancer {
public:
    int time = 0;
    std::queue<request> request_queue;
    std::vector<webserver> servers;
    int resize_interval;

    load_balancer(int resize_interval_) : resize_interval(resize_interval_){}

    void add_request(const request &req, int current_time, std::ofstream &log);

    void remove_request(int current_time, std::ofstream &log);
    void tick_servers(int current_time, std::ofstream &log);

    void check_servers(int current_time, std::ofstream &log);

    void reduce_servers(int current_time, std::ofstream &log, int num_servers);

    void add_servers(int current_time, std::ofstream &log, int num_servers);
};