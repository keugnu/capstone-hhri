
// System
#include <stdlib.h>
#include <string>
#include <stdio.h>
#include <iostream>

// ROS
#include "ros/ros.h"
#include "hbs2/servo.h"


const int _MAESTRO_CHANNEL_ = 5;
const std::string _MAESTRO_DEV_ = "/dev/ttyACM0"
const std::string _MAESTRO_POSCTRL_ = "/home/nvidia/hbs2_1/src/hbs2/lib/maestro_set_pos.sh"
const std::string _MAESTRO_SPDCTRL_ = "/home/nvidia/hbs2_1/src/hbs2/lib/maestro_set_spd.sh"


int convert_from_deg(int deg)
{
	int pos = 36*deg + 1060;
	return pos;
}


void change_speed(spd)
{
	ROS_INFO("Changing speed to %s RPM.", spd);
	std::string syscomspd = _MAESTRO_SPDCTRL_ + " " + _MAESTRO_DEV_ + " " + std::to_string(spd);
	system((syscomspd).c_str());
}


void move(std::string chan, std::string pos)
{
	ROS_INFO("Changing head position to %s degrees.", pos);
	pos = convert_from_deg(pos);
	std::string syscompos = _MAESTRO_POSCTRL_ + " " + _MAESTRO_DEV_ + " " + _MAESTRO_CHANNEL_ + " " + std::to_string(pos);
	system((syscompos).c_str());
}


bool handle_req(hbs2::servo::Request &req, hbs2::servo::Response &res)
{
	switch (req.command) {
		case 1: {
			ROS_INFO("A request to move the position of the head has been made.");
			if(move(_MAESTRO_CHANNEL_, req.position)) {
				res.success = false;
				return true;
			}
			else {
				ROS_ERROR("The posistion request as failed.");
				return false;
			}
			break;
		}
		case 2: {
			ROS_INFO("A request to change the speed of head movement has been made.");
			if(change_speed(req.speed)) {
				res.success = true;
				return true;
			}
			else {
				ROS_ERROR("The servo request has failed.");
				res.success = false;
				return false;
			}
			break;
		}
		default:
			break;
	}
}


int main(int argc, char** argv) {
	ros::init(agc, argv, "servo");
	ros::NodeHandle n;

	ros::ServiceServer srv = n.advertiseService("servo_srv", handle_req);
	ROS_INFO("ROS servo service has started.");
	
	ros::spin();
	
	return 0;
}