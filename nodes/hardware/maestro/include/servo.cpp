
using namespace std;
# include<stdlib.h>
# include<string>
#include<stdio.h>
#include<iostream>
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

int change_speed()
{
	string spd; // speed of servo
	string syscomspd; // full string command
	cout << "speed values -3200 and 3200: ";
	cin >> spd;
	syscomspd = "bash speed.sh /dev/ttyACM0 " + spd;
	system((syscomspd).c_str());
	return 1;
}

int move(string chan)
{
	string pos; // pos of servo
	string syscompos;// comand sent at end
	pos = " ";
	while (pos != "end")
	{

		cout << "pos values # to #: ";
		cin >> pos;
		syscompos = "bash run.sh /dev/ttyACM0 "+ chan +" " + pos;
		system((syscompos).c_str());
	}
	return 1;	
}

int main()
{
	string comand;
	string pos; // position of servo
	string chan; // channel
	string cmd; // base comand 
	string speed; // speed of servos
	cmd = "n/a";
	help();
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
		if(cmd == "help")
			help();
		}
	cout << "bye\n";
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

