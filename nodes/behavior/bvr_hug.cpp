// System
#include <stdio.h>
#include <unistd.h>
#include <vector>
#include <iostream>

// ROS
#include "ros/ros.h"
#include "std_msgs/Int16MultiArray.h"
#include "hbs2/tts.h"

ros::NodeHandlePtr n = NULL;

void adc_callback(const std_msgs::Int16MultiArray::ConstPtr& msg) {
    // Print float of voltage detected at each pin
    int i = 0;
    for(std::vector<int16_t>::const_iterator it = msg->data.begin(); it != msg->data.end(); ++it) {
        ROS_INFO("ADC pin %i: %.2f V", i, (*it * 0.0001875));
        i++;
    }

    // Call tts service with text ("I love you") if hug occurs
    ros::ServiceClient tts_client = n->serviceClient<hbs2::tts>("tts_srv");
    hbs2::tts srv_tts;

    if (/* Correct voltage threshold reached at certain pins? */) {
        srv_tts.request.text = "I love you";
        tts_client.call(srv_tts);
    }

}


int main(int argc, char **argv) {
    ros::init(argc, argv, "hug");
    n = ros::NodeHandlePtr(new ros::NodeHandle);
    ros::Subscriber sub = n->subscribe("tpc_adc", 5, adc_callback);

    ros::spin();

    return 0;
}