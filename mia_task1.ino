
#include "MPU6050_6Axis_MotionApps20.h"

#include <ros.h>
#include <std_msgs/Float32.h>

//intiallizing nodehandler
ros::NodeHandle  nh;
MPU6050 mpu;

//specifying type of data in messege
std_msgs::Float32 angle_msg;

//intiallizing topic
ros::Publisher chatter("chatter", &angle_msg);

uint8_t devStatus;  // return status after each device operation

// orientation/motion vars
Quaternion q;             // [w, x, y, z] 
VectorInt16 aa;           // [x, y, z]
VectorFloat gravity;      // [x, y, z]
VectorInt16 aaReal;       // [x, y, z]  
float ypr[3];             // [yaw, pitch, roll]

// MPU control/status vars
uint8_t fifoBuffer[64];   // FIFO storage buffer
 

void setup()
{
  //intiallizing node
  nh.initNode();
  nh.advertise(chatter);

  //intiallizing mpu
  mpu.initialize();
  
  // initialize serial communication
  Serial.begin(115200);
  
  // initialize i2c communication
  Wire.begin();
  
  // load and configure the DMP
  devStatus = mpu.dmpInitialize();

  // make sure it worked
  if (devStatus == 0) {
        // Calibration Time: generate offsets and calibrate our MPU6050
        mpu.CalibrateAccel(6);
        mpu.CalibrateGyro(6);
        mpu.PrintActiveOffsets();
  }
   else {
        // ERROR!
        // 1 = initial memory load failed
        // 2 = DMP configuration updates failed
        // (if it's going to break, usually the code will be 1)
        Serial.print(F("DMP Initialization failed (code "));
        Serial.print(devStatus);
        Serial.println(F(")"));
    }

}

void loop()
{
  //display yaw angle
   mpu.dmpGetQuaternion(&q, fifoBuffer);
   mpu.dmpGetGravity(&gravity, &q);
   mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
  
   //assignig angle to the messege
  angle_msg.data = ypr[2] * 180/M_PI;
  
  //publishing messege
  chatter.publish( &angle_msg );
  nh.spinOnce();
  delay(10);
}
