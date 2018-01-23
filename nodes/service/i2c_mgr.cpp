#include <sys/types.h>
#include <string>
#include <ros/ros.h>
#include "hbs2_1/i2cbus.h"


int fd;
char* iic_dev = "/dev/i2c-1";


void write_req(uint8_t addr, char regs[25], char data[25], int datasize, char* res) {

}

void read_req(uint8_t addr, char regs[25], char data[25], int datasize, char* res) {
    
}

void handle_req(hbs2_1::i2cbus::Request &req, hbs2_1::i2cbus::Response &res) {
    if (argc != 4) { 
        // add error message
        return false;
    }
    
    uint8_t addr = req.address;
    char* req = req.request;
    char* data = req.data;
    int dsize = req.datasize;

    if (req[0] == 'R') { 
        if (read_req(addr, req, data, dsize, res.data)) { res.success = true; }
        else { res.sucess = false; }
    }
    else if (req[0] == 'W') {
        if (write_req(addr, req, data, dsize, res.data)) { res.success = true; } 
        else { res.success = false; }
    }
    else { 
        // add error message
        res.success = false;
    }
}

int main(int argc, char **argv) {
    ros::init(argc, argv, "i2c_bus_mgr_srv")
    ros::NodeHandle n;

    ros::ServiceServer srv = n.advertiseService("i2c_mgr", handle_req)
    ros:spin();        
    return 0;
}
