
//using namespace std;
# include<stdlib.h>
# include<string>
#include<stdio.h>
#include<iostream>
std::string USB = " /dev/ttyACM0 ";
void help()
{
	std::cout << "\nServo Command 1.0\n";
	std::cout << "chan -> select channel\n";
	std::cout << "speed -> select channel\n";
	std::cout << "move -> move servo pos\n";
	std::cout << "end -> back out\n";
	std::cout << "help -> see menu\n\n";
}

void change_speed(std::string chan)
{
	std::string spd; // speed of servo
	std::string syscomspd; // full string command
	std::cin >> spd;
	syscomspd = "bash speed2.sh" + USB + chan +" "+ spd;
	system((syscomspd).c_str());

}

void move(std::string chan)
{
	std::string pos; // pos of servo
	std::string syscompos;// comand sent at end
	pos = " ";
	while (pos != "end")
	{

		std::cout << "pos values # to #: ";
		std::cin >> pos;
		syscompos = "bash run.sh"+ USB + chan +" " + pos;
		system((syscompos).c_str());
	}
	
}

int main()
{
	std::string comand;
	std::string pos; // position of servo
	std::string chan; // channel
	std::string cmd; // base comand 
	std::string speed; // speed of servos
	cmd = "n/a";
	help();
	chan = "0";
	while (cmd != "end")
		{
		std::cout << "controll menu\n";
		std::cout << "enter command:" ;
		std::cin >> cmd;
		if (cmd == "chan")
		{
			std::cout << "channel # 0-5: ";
			std::cin >> chan;
		}
		if (cmd == "speed")
			change_speed(chan);
		if (cmd == "move")
			move(chan);
		if(cmd == "help")
			help();
		}
	std::cout << "bye\n";
	return 0;
}

//trash
/*
cout << "Enter command: ";
cin >> cmd;

while (pos != "end")
	{
	cout << "chan:";
	cin >> chan;
	cout << "pos:";
	cin >> pos;
	comand = "bash run.sh /dev/ttyACM0 "+ chan +" " + pos;
	//cout<< comand + "\n";
	system((comand).c_str());
	}
return 0;
}

*/
// sudo // ./control.sh /dev/ttyACM0 0 6000 
///opt/.git/capstone-hhri/nodes/hardware/maestro/include
//int n;
//char bigboy[300];
//std::string pos;
//pos = "3000";
//n=sprintf(bigboy, "/opt/.git/capstone-hhri/nodes/hardware/maestro/include/maestro_set_target.sh /dev/ttyACM0 0 %s", pos);
//system("/opt/.git/capstone-hhri/nodes/hardware/maestro/include/maestro_set_target.sh /dev/ttyACM0 0 %s" ,pos);
//system(bigboy);

