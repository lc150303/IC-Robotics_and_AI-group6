#include <iostream>
#include <stdio.h>
#include <Windows.h>
#include<string>
#include<thread>
#include <sstream>
using namespace std;

class Serial { // �����࣬���ڹ����ڣ����д��ڵĴ򿪡�����д����
    HANDLE hCom; // ����ָ��
public:
	Serial(char *port_name) {
		hCom = CreateFile(port_name,//�����豸����
			GENERIC_READ | GENERIC_WRITE, //�������д
			0, //��ռ��ʽ
			NULL,
			OPEN_EXISTING, //�򿪶����Ǵ���
			0, //ͬ����ʽ
			NULL);

		//if (hCom == (HANDLE)-1)
		if (hCom == INVALID_HANDLE_VALUE)
		{
			printf("��COMʧ��!\n");
		}
		else
		{
			printf("COM�򿪳ɹ���\n");
		}

		SetupComm(hCom, 1024, 1024); //���뻺����������������Ĵ�С����1024
		COMMTIMEOUTS TimeOuts;
		//�趨����ʱ
		TimeOuts.ReadIntervalTimeout = 200;
		TimeOuts.ReadTotalTimeoutMultiplier = 20;
		TimeOuts.ReadTotalTimeoutConstant = 50;
		//�趨д��ʱ
		TimeOuts.WriteTotalTimeoutMultiplier = 10;
		TimeOuts.WriteTotalTimeoutConstant = 10;
		SetCommTimeouts(hCom, &TimeOuts); //���ó�ʱ
		DCB dcb;
		GetCommState(hCom, &dcb);
		dcb.BaudRate = (unsigned int)9600; //������Ϊ9600
		dcb.ByteSize = 8; //ÿ���ֽ���8λ
		dcb.Parity = NOPARITY; //����żУ��λ
		dcb.StopBits = ONESTOPBIT; //1��ֹͣλ
		SetCommState(hCom, &dcb);
	}
    string read(int n){
		// ��ȡ���ݲ������ַ���
        char buf[100] = {0};
        DWORD wrsize = 0;
        if(n>100) n=100;
        ReadFile(hCom,buf,n,&wrsize,NULL);
        string s = buf;
        return s;
    }
    bool write(string s0){
		// �򴮿�д���ַ���
        string s = s0+"\r\n";  // ����ָ�������־
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
        if(model=='0'){    // �ֶ������������ڵ���
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
        else if(model=='1'){  // Ԥ�趯�� 1 : ʰȡ�����
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
        else if(model=='2'){  // Ԥ�趯�� 2 : ҡ������ת���Լ�ȡ�ι̶̳�
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
