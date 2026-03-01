#include "load_balancer.h"
#include <queue>
#include "webserver.h"
#include "request.h"

void load_balancer::add_request(const request &req, int current_time, std::ofstream &log){
    request_queue.push(req);
    log << "Time " << current_time << ": Request received from IP " << req.IP_in << " to " << req.IP_out << "\n";
}

void load_balancer::remove_request(int current_time, std::ofstream &log) {
    for (auto &server : servers) {
        if (!server.busy && !request_queue.empty()) {
            request req = request_queue.front();
            request_queue.pop();
            server.take_request(req, current_time, log);
        }
    }
    
}

void load_balancer::tick_servers(int current_time, std::ofstream &log) {
    for (auto &server : servers)
        server.tick(current_time, log);
}


void load_balancer::check_servers(int current_time, std::ofstream &log) {
    // If the queue is longer than 80*num_servers, another server is created. 
    int q_len = request_queue.size();
    int current_num_servers = servers.size();
    if (q_len > 80*current_num_servers) {
        add_servers(current_time, log, current_num_servers); 
    }
    
    // If it is shorter than 50* num_servers, the next webserver that isn't busy, is removed instead of recieving a new task.
    if (q_len < 50*current_num_servers) {
        reduce_servers(current_time, log, current_num_servers);
    }
}

void load_balancer::reduce_servers(int current_time, std::ofstream &log, int num_servers) {
    for (auto it = servers.rbegin(); it != servers.rend(); ++it) {
        if (!it->busy) {
            int removed_id = it->id;
            servers.erase(std::next(it).base());
            log << "Time " << current_time << ": Removed Server " << removed_id << "\n";
            break;
        }
    }
}

void load_balancer::add_servers(int current_time, std::ofstream &log, int num_servers) {
    // find biggest id
    int new_id = num_servers ? servers.back().id + 1 : 1;
    servers.push_back({new_id});
    log << "Time " << current_time << ": Added Server " << new_id << "\n";
}

