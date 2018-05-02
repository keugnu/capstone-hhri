// sudo // ./control.sh /dev/ttyACM0 0 6000 
///opt/.git/capstone-hhri/nodes/hardware/maestro/include
//int n;
//char bigboy[300];
//std::string pos;
//pos = "3000";
//n=sprintf(bigboy, "/opt/.git/capstone-hhri/nodes/hardware/maestro/include/maestro_set_target.sh /dev/ttyACM0 0 %s", pos);
//system("/opt/.git/capstone-hhri/nodes/hardware/maestro/include/maestro_set_target.sh /dev/ttyACM0 0 %s" ,pos);
//system(bigboy);
using namespace std;
# include<stdlib.h>
# include<string>
#include<stdio.h>
#include<iostream>
int main()
{
string comand;
string pos; // position of servo
string chan; // channel
chan = "0";
pos = "0";
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
