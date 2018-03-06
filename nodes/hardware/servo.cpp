
// using namespace std;  (remove this line - it could cause issues in ROS.)

// System
#include<stdlib.h>
#include<string>
#include<stdio.h>
#include<iostream>


/*********************************
	notes:
	make /dev/ttyACM0 a global const string
	use enums or byte literals instead of strings for commands (increases speed)
	the input is not ever from console. your input will be from req.request.command. please change accordingly
	when the command is completed set res.response.success = true
	the entire main function needs to be renamed for something like "handle_req" and it will do the verificaiton of input from the servce call
	main will be used only for doing the ROS setup. if you want, review a service client that has already been written and copy the format

**********************************/

/* remove help() this, we won't see it anyways

int help()
{
	cout << "\nServo Command 1.0\n";
	cout << "chan -> select channel\n";
	cout << "speed -> select channel\n";
	cout << "move -> move servo pos\n";
	cout << "end -> back out\n";
	cout << "help -> see menu\n\n";
	return 1;
}
*/

void change_speed()
{
	string spd; // speed of servo
	string syscomspd; // full string command
	// cout << "speed values -3200 and 3200: ";
	/** you need to check for valid input, don't assume it is valid */
	// cin >> spd;
	syscomspd = "bash speed.sh /dev/ttyACM0 " + spd;
	system((syscomspd).c_str());
}


void move(string chan)
{
	string pos; // pos of servo
	string syscompos;// comand sent at end
	pos = " ";
	while (pos != "end")
	{

		// cout << "pos values # to #: ";
		/** you need to check for valid input, don't assume it is valid */
		// cin >> pos;
		syscompos = "bash run.sh /dev/ttyACM0 "+ chan +" " + pos;
		system((syscompos).c_str());
	}
}


int main(int argc, char **argv)
{
	string comand;
	string pos; // position of servo
	string chan; // channel
	string cmd; // base comand 
	string speed; // speed of servos
	cmd = "";
	while (cmd != "end")
		{
		cout << "controll menu\n";
		cout << "enter command:" ;
		cin >> cmd;
		if (cmd == "chan")
		{
			cout << "channel # 0-5: ";
			cin >> chan;
		}
		if (cmd == "speed")
			change_speed();
		if (cmd == "move")
			move(chan);
	return 0;
}