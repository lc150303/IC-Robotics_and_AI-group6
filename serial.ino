//********************************************
//* Robotic Arm with BLE control v1
//* for robotic arm 7
//* By Benny Lo
//* Jan 14 2018
//********************************************
#include <CurieBLE.h>
#include <Servo.h>  
#include <string.h>
#define ROBOT_NAME "BRobot-7"  
//***** define the range of every motor *******
#define CRAW_MIN 0 //open 
#define CRAW_MAX 65 //close
#define ELBOW_MIN   0
#define ELBOW_MAX 140
#define SHOULDER_MIN 0
#define SHOULDER_MAX 165
#define WRIST_X_MIN 0
#define WRIST_X_MAX 180
#define WRIST_Y_MIN 0
#define WRIST_Y_MAX 90
#define WRIST_Z_MIN 0
#define WRIST_Z_MAX 180
#define BASE_MIN 0
#define BASE_MAX 180
//***** define the default of every motor *******
#define ELBOW_DEFAULT 50
#define SHOULDER_DEFAULT 140
#define WRIST_X_DEFAULT 80
#define WRIST_Y_DEFAULT 90
#define WRIST_Z_DEFAULT 66
#define BASE_DEFAULT 80
#define CRAW_DEFAULT CRAW_MIN //fully opened
Servo myservoA;  
Servo myservoB;
Servo myservoC;
Servo myservoD;
Servo myservoE;
Servo myservoF;
Servo myservoG;//the craw
int i,pos,myspeed;
byte sea,seb,sec,sed,see,sef,seg,sea_new,seb_new,sec_new,sed_new,see_new,sef_new,seg_new;
String input = "";

void myMove(){
   sea=myservoA.read();
   seb=myservoB.read();
   sec=myservoC.read();
   sed=myservoD.read();
   see=myservoE.read();
   sef=myservoF.read();
   seg=myservoG.read();
   myspeed=500;
   for(pos=0;pos<=myspeed;pos+=1)
   {
    myservoA.write(int(map(pos,1,myspeed,sea,sea_new)));
    myservoB.write(int(map(pos,1,myspeed,seb,seb_new)));
    myservoC.write(int(map(pos,1,myspeed,sec,sec_new)));
    myservoD.write(int(map(pos,1,myspeed,sed,sed_new)));
    myservoE.write(int(map(pos,1,myspeed,see,see_new)));
    myservoF.write(int(map(pos,1,myspeed,sef,sef_new)));
    myservoG.write(int(map(pos,1,myspeed,seg,seg_new)));    
    delay(1);
   }
}


void myservosetup()  //set up the servo motors
{
	// get the present position of every motor
  sea_new = ELBOW_DEFAULT;
  seb_new = SHOULDER_DEFAULT;
  sec_new = WRIST_X_DEFAULT;
  sed_new = WRIST_Y_DEFAULT;
  see_new = WRIST_Z_DEFAULT;
  sef_new = BASE_DEFAULT;
  seg_new = CRAW_DEFAULT;
  myMove();
}

void setup() // setup the arduino
{ 
  Serial.begin(9600,SERIAL_8N1);  	// open serial port
  while(!Serial){};  				// wait until serial port is ready
  Serial.setTimeout(10);			// setup serial port
  
  // attach servo object to pins 将舵机对象连接到引脚
  myservoA.attach(2);  
  myservoB.attach(3); 
  myservoC.attach(4); 
  myservoD.attach(5); 
  myservoE.attach(6); 
  myservoF.attach(7);
  myservoG.attach(8); 
  
  // move the robot arm to default position
  myservosetup();
  
  // now it's ready
  Serial.println(("Only Serial port open"));
}

void loop() 
{ 
  if(Serial.available()>0){  // if receive data 
    input = Serial.readStringUntil('\n');  // legal instructions end with \r\n
    if(input.charAt(0)=='#'){              // legal instructions start with #
      if(input.charAt(1)=='R'){ // Reset
        myservosetup();
        Serial.println("reset");
      }
      else{	 // move motors, instructions are in format like #A2B9\r\n
        unsigned int i=1;
        while(input.charAt(i)!='\r' && i+1<input.length()){
			// save new motor positions into variables
          switch(input.charAt(i++)){
            case 'A':sea_new = input.charAt(i++);break;	//elbow
            case 'B':seb_new = input.charAt(i++);break;	//shoulder
            case 'C':sec_new = input.charAt(i++);break;	//wrist_x
            case 'D':sed_new = input.charAt(i++);break;	//wrist_y
            case 'E':see_new = input.charAt(i++);break;	//wrist_z
            case 'F':sef_new = input.charAt(i++);break;	//base
            case 'G':seg_new = input.charAt(i++);break;	//craw
          }
        }
        if(input.charAt(i)!='\r'){	
          Serial.println("unfinished instruction");
        }
        else{	// move motors, send signal to pins
          sea_new = sea_new>ELBOW_MAX?ELBOW_MAX:(sea_new<ELBOW_MIN?ELBOW_MIN:sea_new);
          seb_new = seb_new>SHOULDER_MAX?SHOULDER_MAX:(seb_new<SHOULDER_MIN?SHOULDER_MIN:seb_new);
          sec_new = sec_new>WRIST_X_MAX?WRIST_X_MAX:(sec_new<WRIST_X_MIN?WRIST_X_MIN:sec_new);
          sed_new = sed_new>WRIST_Y_MAX?WRIST_Y_MAX:(sed_new<WRIST_Y_MIN?WRIST_Y_MIN:sed_new);
          see_new = see_new>WRIST_Z_MAX?WRIST_Z_MAX:(see_new<WRIST_Z_MIN?WRIST_Z_MIN:see_new);
          sef_new = sef_new>BASE_MAX?BASE_MAX:(sef_new<BASE_MIN?BASE_MIN:sef_new);
          seg_new = seg_new>CRAW_MAX?CRAW_MAX:(seg_new<CRAW_MIN?CRAW_MIN:seg_new);
          myMove();
          Serial.println("finished moves");
        }
      }
    }
  }
  
}

