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


    // LOG COLORS:
    #define request_c "\033[95m"                    // Adding Requests
    #define processing_c    "\033[38;2;255;105;180m"      // Request processing
    #define servers_c  "\033[38;2;255;182;193m"      // Adding / Deleting Servers
    #define gen_info_c  "\033[38;2;255;20;147m"       // General Info
    #define reg_c       "\033[0m"

    std::ofstream log("simulation.log");
    std::cout << "Enter number of servers: ";
    std::cin >> num_servers;

    std::cout << "Enter running time: ";
    std::cin >> running_time;
    
    log << reg_c << "Starting Simulation.\n";
    log << gen_info_c << "general information:\n";

    log << gen_info_c << "Number of servers: " << num_servers << "\n";
    log << gen_info_c << "Running time: " << running_time << " clock ticks\n";
    // std::ostream &log = std::cout;
    int num_new_requests;
    int resize_interval;
    int addmore_interval;
    int process_max_time;

    std::ifstream file("config.txt");

    if (!file) {
        std::cerr << "Could not open config file\n";
        return 1;
    }

    file >> num_new_requests
         >> resize_interval
         >> addmore_interval
         >> process_max_time;

    log << gen_info_c << "Max time to process a request: " << process_max_time << "\n";
    log << gen_info_c << "Every " << addmore_interval << " clock ticks, "<<num_new_requests << " new requests will be added.\n";
    log << gen_info_c << "The number of servers will be resized every: " << resize_interval << " clock ticks.\n";
    log << gen_info_c << "Queue starting size: " << num_servers * 100 << "\n";

    load_balancer lb(resize_interval, addmore_interval); 
    std::queue<request> request_queue;

    // request generators
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 eng{seed};

    std::uniform_int_distribution<int> ips(1, 1000000);
    std::uniform_int_distribution<int> times(1, process_max_time);
    std::uniform_int_distribution<int> job(1, 2);

    for (int counter = 0; counter < (num_servers*100); ++counter){
        request req = generate_request(eng, ips, times, job);
        lb.add_request(req, 0, log);
    }
    log << gen_info_c << "Initial requests generated \n";
    

    // create webservers
    for (int counter = 0; counter < num_servers; counter++){
       lb.add_servers(0, log, counter);
    }

    log << gen_info_c <<"Initial servers created.\n";
    for (int current_time = 1; current_time <= running_time; ++current_time) {

        lb.remove_request(current_time, log);
        lb.tick_servers(current_time, log);
        if (current_time % lb.resize_interval == 0) {
            log <<servers_c<< "Time " << current_time << ": Checking if server needs to be resized.\n";
            lb.check_servers(current_time, log);

        }
        if (current_time % lb.addmore_interval == 0) {
            log << request_c << "Time " << current_time << ": Adding "<< num_new_requests <<" new requests.\n";

            for (int counter = 0; counter < (20); ++counter){
                request req = generate_request(eng, ips, times, job);
                lb.add_request(req, 0, log);
            }
        }
            
    }


    log << gen_info_c << "Running time over.\n";
    log << gen_info_c << "Ending queue size:" << lb.request_queue.size() <<"\n";
    
    std::cout << "Done\n";
    log.close();
    return 0;
}

