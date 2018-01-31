// System
#include <sys/types.h>
#include <string>
#include <vector>
#include <queue>
#include <unistd.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>

// ROS
#include "ros/ros.h"
#include "hbs2/i2c_bus.h"

// Custom
#include "Request.hpp"


int fd;
char* iic_dev = "/dev/i2c-1";
std::queue<Request> work_queue, completed_queue;


bool write_req(Request* req) {
    if (fd = open(iic_dev, O_RDWR) < 0) { return false; }
    else {
	char[job.get_size()] to_write = {0};
	char[1] addr = {job.get_id()};
	for (int i = 0; i < job.regs.size(); i++) {
	    to_write[i] = jobs.data.at(i);
	}

	ioctl(fd, I2C_SLAVE, addr);

	if (write(fd, to_write, job.get_size()) != job.get_size()) { return false; }
    }

    job.set_status(true);
    return true;
}

bool read_req(Request* req) {
    if (fd = open(iic_dev, O_RDWR) < 0 { return false; }
    else {
	char[job.get_expected()] is_read = {0};
        char[1] addr = {job.get_id()};
	char[jobs.get_size()] to_read = {0};
        for (int i = 0; i < job.regs.size(); i++) {
            to_read[i] = jobs.data.at(i);
        }

        ioctl(fd, I2C_SLAVE, addr);

	if (read(fd, is_read, job.get_expected) != jobs.get_expected) { return false; }

    }

    job.set_status(true);	
    return true;
}

bool handle_req(hbs2::i2c_bus::Request &req, hbs2::i2c_bus::Response &res) {
    Request request = Request(req.request, req.size);

    if (request.get_type() == "status") {
	res.success = false;
	for (int i = 0; i < completed_queue.size(); i++) {
	    Request job = completed_queue.pop();
	    if (job.get_id() == req.request[1]) { res.success = true; break; }
	    else { completed_queue.push(job); }    
	}
	return true;
    }
    else { work_queue.push(Request); }
    
    if (!work_queue.empty()) {

	// TODO sort on priority

	Request job = work_queue.pop();
	if (job.get_type() == "read") { 
	    if (read_req(&job)) { completed_queue.push(job); }
	    else { work_queue.push(job); }
	else if (job.get_type() == "write") { 
	    if (write_req(&job)) { completed_queue.push(job); }
	    else { work_queue.push(job); }

    }
    return true;
}

int main(int argc, char **argv) {
    ros::init(argc, argv, "i2c_bus_mgr_srv");
    ros::NodeHandle n;

    ros::ServiceServer srv = n.advertiseService("i2c_srv", handle_req);
    ros::spin();        
    return 0;
}
