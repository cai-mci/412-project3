#include <iostream>
#include <chrono>
#include <random>
#include <fstream>
#include <queue>

#include "load_balancer.h"
#include "request.h"


int main() {
    int num_servers;
    int running_time;


    std::ofstream log("simulation.log");
    std::cout << "Enter number of servers: ";
    std::cin >> num_servers;

    std::cout << "Enter running time (hours): ";
    std::cin >> running_time;

    log << "Number of servers: " << num_servers << "\n";
    log << "Running time: " << running_time << " hours\n";
    // std::ostream &log = std::cout;


    load_balancer lb(10); //resize time = 5
    std::queue<request> request_queue;

    // request generators
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 eng{seed};

    std::uniform_int_distribution<int> ips(1, 1000000);
    std::uniform_int_distribution<int> times(1, 10);
    std::uniform_int_distribution<int> job(1, 2);

    for (int counter = 0; counter < (num_servers*100); ++counter){
        request req = generate_request(eng, ips, times, job);
        lb.add_request(req, 0, log);
    }
    log << "Initial requests generated \n";
    

    // create webservers
    for (int counter = 0; counter < num_servers; counter++){
       lb.add_servers(0, log, counter);
    }

    log << "All servers created.\n";
    for (int current_time = 1; current_time <= running_time; ++current_time) {

        lb.remove_request(current_time, log);
        lb.tick_servers(current_time, log);
        if (current_time % lb.resize_interval == 0)
            lb.check_servers(current_time, log);
    }


    log << "All requests processed.\n";
    std::cout << "Done\n";
    log.close();
    return 0;
}

