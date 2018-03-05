/*	SRF02 sonar client node that calls upon i2c_mgr.cpp service	*/


#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <vector>
#include <iostream>

#include "ros/ros.h"
#include "hbs2/i2c_bus.h"

// Configure sensor and set range to centimeters
bool write_init(ros::ServiceClient &client, hbs2::i2c_bus &srv) {
    srv.request.request.resize(4);
    srv.request.request = {0x02, 0x70, 0x00, 0x51};
    srv.request.size = 4;

    if (client.call(srv)) {
	return true;    
    }

    ROS_ERROR("Failed to call service i2c_srv");
    return false;
}

// Read range from register (result is 4 bytes)
unsigned int read_temp(ros::ServiceClient &client, hbs2::i2c_bus &srv) {
    srv.request.request.resize(6);
    srv.request.request = {0x01, 0x70, 0x00, 0x00, 0x00, 0x00};
    srv.request.size = 6;
	
    if (client.call(srv)) {
	std::vector<signed char> data(srv.response.data);

	unsigned char high = srv.response.data.at(2);
	unsigned char low = srv.response.data.at(3);
	// Set higher byte (high) to be upper 8 bits
	unsigned int range = high;
	range <<= 8;
	// Set lower byte (low) to be lower 8 bits
	range += low;

	return range;
    }
    ROS_ERROR("Failed to call service i2c_srv");
    return -1;
}


int main(int argc, char **argv) {

    // Initialize sonar sensor node
    ros::init(argc, argv, "srf02");
    ros::NodeHandle n;
    ros::ServiceClient client = n.serviceClient<hbs2::i2c_bus>("i2c_srv");
    hbs2::i2c_bus srv;
    if (write_init(client, srv)) {
	unsigned int range = read_range(client, srv);
	ROS_INFO("Range in centimeters: %u\n", range);
}
    return 0;	
}
