// System
#include <stdlib.h>
#include <unistd.h>
#include <iostream>

// ROS
#include "ros/ros.h"
#include "hbs2/iot_srv.h"



void handle_request(hbs2::iot_srv::Request &req, hbs2::iot_srv::Response &res, ros::NodeHandle &n) {
    
    ros::ServiceClient sonar_client = n.serviceClient<hbs2::sonar_hw>("sonar_srv");
    ros::ServiceClient tts_client = n.serviceClient<hbs2::tts_hw>("tts_srv");
    ros::ServiceClient servo_client = n.serviceClient<hbs2::servo_hw>("servo_srv");

    hbs2::sonar_hw srv_sonar;
    hbs2::tts_hw srv_tts;
    hbs2::servo_hw srv_servo;

    switch (req.command) {
        case 1: {
            ROS_INFO("Serving request to read sonar sensor.");
            sonar_client.call(srv_sonar);
            if (srv_sonar.response.success == true) {
                res.response.success = true;
                res.response.data = srv_sonar.response.data;
            }
            else {
                ROS_ERROR("Request to read sonar sensor failed in iot_srv.");
                res.response.success = false;
            }
            break;
        }
        case 2: {
            ROS_INFO("Serving request for TTS.");
            srv_tts.request.text = req.request.text;
            tts_client.call(srv_tts);
            if (srv_tts.response.success == true) { res.response.success = true; }
            else {
                ROS_ERROR("TTS Request failed in iot_srv.");
                res.response.success = false;
            }
            break;
        }
        case 3: {
            ROS_INFO("Serving request to shake head.");
            servo_client.call(srv_servo);
            if (srv_servo.response.success == true) { res.response.success = true; }
            else {
                ROS_ERROR("Request to shake head has failed in iot_srv.");
                res.response.success = false;
            }
            break;
        }
    }
}


int main(int argc, char **argv) {
    ros::init(argc, argv, "iot_srv");
    ros::NodeHandle n;

    ros::ServiceServer srv = n.advertiseService("iot_srv", handle_req);
    ros::ServiceClient sonar_client = n.serviceClient<hbs2::sonar_hw>("sonar_srv");
    ros::ServiceClient tts_client = n.serviceClient<hbs2::tts_hw>("tts_srv");
    ros::ServiceClient servo_client = n.serviceClient<hbs2::servo_hw>("servo_srv");

    hbs2::sonar_hw srv_sonar;
    hbs2::tts_hw srv_tts;
    hbs2::servo_hw srv_servo;

    ROS_INFO("ROS IOT Service has started.");
    ros::spin();
    return 0;
}
