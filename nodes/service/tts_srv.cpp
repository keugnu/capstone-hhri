// System
#include <stdlib.h>
#include <stdio.h>
#include <string>

// ROS
#include "ros/ros.h"
#include "hbs2/iot.h"
#include "hbs2/tts.h"
#include "hbs2/sonar.h"

ros::NodeHandlePtr n = NULL;


bool handle_req(hbs2::tts::Request &req, hbs2::tts::Response &res)
{
    char buf[150];
    int str_len = sprintf(buf, "echo %s | festival --tts", req.text.c_str());
    printf("%s\n", buf);
    if (system(buf) != 0)
    {
    ROS_ERROR("System call to festival failed.");
    res.success = false;
    }
    else 
    {
    res.success = true;
    ROS_INFO("System call to festival succeeded.");
    }  
}


int main(int argc, char **argv)
{  
    // Initial tts node  
    ros::init(argc, argv, "tts_srv");
    n = ros::NodeHandlePtr(new ros::NodeHandle);
    
    //ros::ServiceClient = n->seviceClient<hbs2::i2c_bus>("i2c_srv");
    //hbs2::i2c_bus srv_i2c;
    
    ros::ServiceServer srv = n->advertiseService("tts_srv", handle_req);
    ROS_INFO("ROS TTS service has started.");
    ros::spin();

    // End program
    return 0;
}
