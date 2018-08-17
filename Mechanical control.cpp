#include <iostream>
#include <stdio.h>
#include <Windows.h>
#include<string>
#include<thread>
#include <sstream>
using namespace std;

class Serial { // 串口类，用于管理串口，进行串口的打开、读、写操作
    HANDLE hCom; // 串口指针
public:
	Serial(char *port_name) {
		hCom = CreateFile(port_name,//串口设备名称
			GENERIC_READ | GENERIC_WRITE, //允许读和写
			0, //独占方式
			NULL,
			OPEN_EXISTING, //打开而不是创建
			0, //同步方式
			NULL);

		//if (hCom == (HANDLE)-1)
		if (hCom == INVALID_HANDLE_VALUE)
		{
			printf("打开COM失败!\n");
		}
		else
		{
			printf("COM打开成功！\n");
		}

		SetupComm(hCom, 1024, 1024); //输入缓冲区和输出缓冲区的大小都是1024
		COMMTIMEOUTS TimeOuts;
		//设定读超时
		TimeOuts.ReadIntervalTimeout = 200;
		TimeOuts.ReadTotalTimeoutMultiplier = 20;
		TimeOuts.ReadTotalTimeoutConstant = 50;
		//设定写超时
		TimeOuts.WriteTotalTimeoutMultiplier = 10;
		TimeOuts.WriteTotalTimeoutConstant = 10;
		SetCommTimeouts(hCom, &TimeOuts); //设置超时
		DCB dcb;
		GetCommState(hCom, &dcb);
		dcb.BaudRate = (unsigned int)9600; //波特率为9600
		dcb.ByteSize = 8; //每个字节有8位
		dcb.Parity = NOPARITY; //无奇偶校验位
		dcb.StopBits = ONESTOPBIT; //1个停止位
		SetCommState(hCom, &dcb);
	}
    string read(int n){
		// 读取数据并返回字符串
        char buf[100] = {0};
        DWORD wrsize = 0;
        if(n>100) n=100;
        ReadFile(hCom,buf,n,&wrsize,NULL);
        string s = buf;
        return s;
    }
    bool write(string s0){
		// 向串口写入字符串
        string s = s0+"\r\n";  // 加上指令结束标志
        DWORD wrsize = 0;
        WriteFile(hCom,s.c_str(),s.length(),&wrsize,NULL);

		if(wrsize==s.length()) return true;
		else return false;
    }
};

int main() {
	Serial Robot("COM6");
	char model;

	while(true){
	    cout<<"model 0:directly input;\nmodel 1:pick and place\nmodel 2:pick and shake"<<endl;
        cin>>model;
        if(model=='0'){    // 手动输入坐标用于调试
            string s = "";
            while (true) {
                DWORD wrsize;
                cout << "input:";
                cin >> s;
                if (s[1] == 'R') {
                    Robot.write(s);
                    Sleep(500);
                    cout << "result:" << Robot.read(50) << endl;
                }
                else {
                    stringstream ss(s.substr(1));
                    s = "#";
                    char c;
                    while (ss >> c) {
                        s = s + c;
                        int value = 0;
                        ss >> value;
                        s = s + (char)value;
                    }
                    cout << "write:" << s << endl;
                    Robot.write(s);
                    cout << "result:" << Robot.read(50) << endl;
                }
            }
        }
        else if(model=='1'){  // 预设动作 1 : 拾取与放置
            Robot.write("#R");
            getchar();
            Robot.read(20);
            string s = "";
            s = s + "#E"+(char)70+ "#C" + (char)40+ "F" + (char)70+ "A" + (char)40 + "B" + (char)140;
            Robot.write(s);
            getchar();
            Robot.read(20);
            s = "";
            s = s + "#B" + (char)115 + "A" + (char)100;
            Robot.write(s);
            getchar();
            Robot.read(20);
            s = "";
            s = s + "#G" + (char)40;
            Robot.write(s);
            getchar();
            Robot.read(20);
            s = "";
            s = s + "#A" + (char)50 + "B" + (char)160;
            Robot.write(s);
            getchar();
            Robot.read(20);
            s = "";
            s = s + "#F" + (char)130;
            Robot.write(s);
            /*getchar();
            Robot.read(20);
            s = "";
            s = s + "#F" + (char)40;
            Robot.write(s);*/
            getchar();
            Robot.read(20);
            s = "";
            s = s + "#B" + (char)75 + "A" + (char)70;
            Robot.write(s);
            getchar();
            Robot.read(20);
            s = "";
            s = s + "#G" + (char)1;
            Robot.write(s);
            //Sleep(1000);
            getchar();
            s = "";
            Robot.read(20);
            s = s + "#A" + (char)30;
            Robot.write(s);
            getchar();
            s = "";
            Robot.read(20);
            Robot.write(s+"#F"+(char)85+"A"+(char)70+"B"+(char)200);
            Robot.read(20);
        }
        else if(model=='2'){  // 预设动作 2 : 摇晃与旋转测试夹取牢固程度
            Robot.write("#R");
            getchar();
            Robot.read(20);
            string s = "";
            s = s + "#E"+(char)70+ "C" + (char)40+ "F" + (char)70+ "A" + (char)40 + "B" + (char)140;
            Robot.write(s);
            getchar();
            Robot.read(20);
            s = "";
            s = s + "#B" + (char)115 + "A" + (char)102;
            Robot.write(s);
            getchar();
            Robot.read(20);
            s = "";
            s = s + "#G" + (char)40;
            Robot.write(s);
            getchar();
            Robot.read(20);
            s = "";
            s = s + "#A" + (char)50 + "B" + (char)160;
            Robot.write(s);
            getchar();
            Robot.read(20);
            s = "";
            s = s + "#F" + (char)130;
            Robot.write(s);
            getchar();
            Robot.read(20);
            s = "";
            s = s + "#F" + (char)70+"E"+(char)180+"C"+(char)150;
            Robot.write(s);
            getchar();
            Robot.read(20);
            s = "";
            s = s + "#F" + (char)85+"E"+(char)0+"C"+(char)0;
            Robot.write(s);
            getchar();
            Robot.read(20);
            s = "";
            s = s + "#A" + (char)28+"B"+(char)160+"C"+(char)100+"E"+(char)80;
            Robot.write(s);
            getchar();
            s = "";
            Robot.read(20);
            s = s + "#A" + (char)80+"B"+(char)120+"E"+(char)40+"C"+(char)20;
            Robot.write(s);
            getchar();
            s = "";
            Robot.read(20);
            s = s + "#A" + (char)90+"B"+(char)105+"E"+(char)30+ "C" + (char)0;
            Robot.write(s);
            getchar();
            Robot.read(20);
            s = "";
            s = s + "#G" + (char)1;
            Robot.write(s);
            //Sleep(1000);
            getchar();
            s = "";
            Robot.read(20);
            s = s + "#A" + (char)80+"B"+(char)140;
            Robot.write(s);
            s = "";
            Robot.read(20);
            Robot.write(s+"#F"+(char)85+"A"+(char)70+"B"+(char)200);
            Robot.read(20);
        }
        else cout<<"invalid input"<<endl;
	}
}
