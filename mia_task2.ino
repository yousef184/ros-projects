
#include <ros.h>
#include <std_msgs/Int32.h>

//intiallizing nodehandler
ros::NodeHandle  nh;

//specifying type of data in messege
std_msgs::Int32 counts_msg;

//intiallizing topic
ros::Publisher chatter("chatter", &counts_msg);

int counterA =0;
int M1 = 8;//motor 1 pin 1 connected to motor driver
int M2 = 7;//motor 1 pin 2 connected to motor driver
int encoderA1 = 21;
int encoderA2 = 20;


void setup()
{
  //intiallizing node
  nh.initNode();
  nh.advertise(chatter);
  
  //intiallizing pins
  pinMode(encoderA1, INPUT_PULLUP);
  pinMode(encoderA2, INPUT_PULLUP);
  pinMode(M1, OUTPUT);
  pinMode(M2, OUTPUT);
  
  //interrupts to increment counter
  attachInterrupt(encoderA1, function1, CHANGE);
  attachInterrupt(encoderA2, function2, CHANGE);
}

void loop()
{
  //assignig counts of encoder to the messege
  counts_msg.data = counterA;
  //publishing messege
  chatter.publish( &counts_msg );
  nh.spinOnce();
  delay(10);
}
//function to compute motor 1 counter 
void function1 ()
{
  if(digitalRead(encoderA1) != digitalRead(encoderA2))
  {
    counterA++;
  }
  else{
    counterA--;
  }
}
//function to compute motor 1 counter 
void function2 ()
{
  if(digitalRead(encoderA1) == digitalRead(encoderA2))
  {
    counterA++;
  }
  else{
    counterA--;
  }
}
