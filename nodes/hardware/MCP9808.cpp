/* 	
	MCP9808 client node that calls upon i2c_mgr.cpp service
	req format: [type, device addr, reg1, dat1, <dat1>,...,regN, datN, <datN>]
	if type = read req, dat1:N = 0
*/


#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>

#include "ros/ros.h"
#include "hbs2/i2c_bus.h"

//send write req to i2c_mgr.cpp service node to configure, set resolution, & for read request

int main(int argc, char **argv) {
	// Initialize temp sensor node
	ros::init(argc, argv, "mcp9808");
	ros::NodeHandle n;
	ros::ServiceClient client = n.serviceClient<hbs2::i2c_bus>("i2c_srv");
	hbs2::i2c_bus srv;

	// Write 2 bytes (0x00, 0x00) to register 0x01 to configure sensor
	unsigned char wreq[5] = {0x02, 0x18, 0x01, 0x00, 0x00};
	srv.request.request = wreq;
	srv.request.size = 5; // number of values you're writing (including reg)

	if(!client.call(srv)) {
		ROS_ERROR("Failed to call service i2c_srv");
		return 1;
	}
	
	// Write 1 byte (0x03) to register 0x08 to set temperature resolution
	wreq[0] = {0x02}; wreq[1] = {0x18}; wreq[2] = {0x08}; wreq[3] = {0x03};
	srv.request.request = wreq;
	srv.request.size = 4; // number of values you're writing (including reg)

	if (!client.call(srv)) {
		ROS_ERROR("Failed to call service i2c_srv");
		return 1;
	}

	// Write 0 bytes to register 0x05 in order to read temperature
	wreq[0] = {0x02}; wreq[1] = {0x18}; wreq[2] = {0x05};
	srv.request.request = wreq;
	srv.request.size = 3; // number of values you're writing (including reg)

	if (!client.call(srv)) {
		ROS_ERROR("Failed to call service i2c_srv");
		return 1;
	}
	
	// Read 2 bytes (msb and lsb) from register 0x05
	unsigned char rreq[] = {0x01, 0x18, 0x00, 0x00};
	unsigned char data[2];
	srv.request.request = rreq;
	srv.request.size = 4; // number of values you're expecting to read

	if(!client.call(srv)) {
		ROS_ERROR("Failed to call service i2c_srv");
		return 1;
	} else if (srv.response.success) {
		data = srv.response.data;
		int temp = ((data[0] & 0x1F) * 256 + data[1]);
		if (temp > 4095)
			temp -= 8192;

		float fTemp = (temp * 0.0625) * 1.8 + 32;
		ROS_INFO("Temperature in Fahrenheit: %.2f \n", fTemp);
	}

	return 0;
}
