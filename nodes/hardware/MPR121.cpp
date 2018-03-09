/*	MPR121 Touch Sensor Hardware Node	*/

#include <stdio.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <vector>
#include <iostream>

#include "ros/ros.h"
#include "hbs2/i2c_bus.h"

// Perform soft reset and start with MPR121 in "Stop mode" to prevent random reads.
bool write_init(ros::ServiceClient &client, hbs2::i2c_bus &srv) {
    srv.request.request.resize(4);
    srv.request.request = {0x02, 0x5A, 0x80, 0x63};
    srv.request.size = 4;

    if (client.call(srv)) {
		srv.request.request.resize(4);
		srv.request.request = {0x02, 0x5A, 0x5E, 0x00};
		srv.request.size = 4;

		if (client.call(srv)) {
			return true;
		}
    }

    ROS_ERROR("Failed to call i2c_srv");
    return false;
}

// Set up touch and release threshold registers.
bool touch_init(ros::ServiceClient &client, hbs2::i2c_bus &srv) {
	// Touch and release threshold registers 
	uint8_t thres_reg = 0x41;
	uint8_t rel_reg = 0x42;

	// Touch and release thresholds
	uint8_t touch_thres = 12;
	uint8_t rel_thres = 6;

	srv.request.request.resize(4);
	srv.request.size(4);

    // Set up 13 touch channels with touch threshold = 12
	srv.request.request = {0x02, 0x5A, thres_reg, touch_thres};

	for (int i = 0; i < 12; i++) {
		if (client.call(srv)) {
			thres_reg += i*2;
			srv.request.request = {0x02, 0x5A, thres_reg, touch_thres};
		} else { ROS_ERROR("Failed to call i2c_srv for touch"); return false; }
	}

    // Set up 13 touch channels with release threshold = 6
	srv.request.request = {0x02, 0x5A, rel_reg, rel_thres};

	for (int i = 0; i < 12; i++) {
		if (client.call(srv)) {
			rel_reg += i*2;
			srv.request.request = {0x02, 0x5A, rel_reg, rel_thres};
		} else { ROS_ERROR("Failed to call i2c_srv for touch"); return false; }
	}

    return true;
}

// Set up registers (MHD, NHD, NCL, FDL, Debounce touch & release, Baseline tracking)
bool reg_setup(ros::ServiceClient &client, hbs2::i2c_bus &srv) {
	srv.request.request.resize(4);
	srv.request.size = 4;
	srv.request.request = {0x02, 0x5A, 0x00, 0x00};

	// MHD rising reg 0x2B, NHD rising reg 0x2C, NCL rising reg 0x2D, FDL rising reg 0x2E
	srv.request.request.data.at(2) = 0x2B; srv.request.request.data.at(3) = 0x01; 
	srv.request.request.data.at(2) = 0x2C; srv.request.request.data.at(3) = 0x01; 
	srv.request.request.data.at(2) = 0x2D; srv.request.request.data.at(3) = 0x01; 
	srv.request.request.data.at(2) = 0x2E; srv.request.request.data.at(3) = 0x01; 

    buff[0] = 0x2B, buff[1] = 0x01; write(file, buff, 2);
    buff[0] = 0x2C, buff[1] = 0x01; write(file, buff, 2);
    buff[0] = 0x2D, buff[1] = 0x0E; write(file, buff, 2);
    buff[0] = 0x2E, buff[1] = 0x00; write(file, buff, 2);

    // MHD falling reg, 0x2F, NHD falling reg 0x30, NCL falling reg 0x31, FDL falling reg 0x32
    buff[0] = 0x2F, buff[1] = 0x01; write(file, buff, 2);
    buff[0] = 0x30, buff[1] = 0x05; write(file, buff, 2);
    buff[0] = 0x31, buff[1] = 0x01; write(file, buff, 2);
    buff[0] = 0x32, buff[1] = 0x00; write(file, buff, 2);

    // NHD touched reg 0x33, NCL touched 0x34, FDL touched 0x35
    buff[0] = 0x33, buff[1] = 0x00; write(file, buff, 2);
    buff[0] = 0x34, buff[1] = 0x00; write(file, buff, 2);
    buff[0] = 0x35, buff[1] = 0x00; write(file, buff, 2);

    // Debounce touch & release reg 0x5B, config 1 reg 0x5c, config 2 ref 0x5D
    buff[0] = 0x5B, buff[1] = 0x00; write(file, buff, 2);
    buff[0] = 0x5C, buff[1] = 0x10; write(file, buff, 2); // default, 16uA charge current
	buff[0] = 0x5D, buff[1] = 0x20; write(file, buff, 2); // 0.5us encoding, 1ms period



} 

int main(int argc, char **argv) {
    // Initialize touch sensor node
    ros::init(argc, argv, "mpr121");
    ros::ServiceClient client = n.serviceClient<hbs2::i2c_bus>("i2c_bus");
    hbs2::i2c_bus srv;

    // Call functions.

    return 0;
}
