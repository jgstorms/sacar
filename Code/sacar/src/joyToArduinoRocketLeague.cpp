#include "ros/ros.h"
#include "sacar/ESC.h"
#include <sensor_msgs/Joy.h>

//The frequencies of the steering servo that dictate left, right, neutral
int steering_left = 2000;
int steering_right = 1250;
int steering_neutral = 1625;

//Made a class to contain everything, so just making an instance of this does almost everything needed 
// Keeps main cleaner
class SACARTeleop
{
public:
  SACARTeleop();

private:
  void joyCallback(const sensor_msgs::Joy::ConstPtr& joy);
  
  ros::NodeHandle nh;
  int forward, reverse;
  double steering;
  ros::Publisher esc_pub;
  ros::Subscriber joy_sub;
  
};

//Advertising on topic throttleandsteering with the appropriate frequencies for throttle and steering 
//Subscribing to the topic 'joy' which is being broadcasted to by the controller 

SACARTeleop::SACARTeleop()
{
  esc_pub = nh.advertise<sacar::ESC>("throttleandsteering", 10);
  joy_sub = nh.subscribe<sensor_msgs::Joy>("joy", 10, &SACARTeleop::joyCallback, this);
}

//The callback function, just takes the sensor msgs and puts them into local variables we can use
void SACARTeleop::joyCallback(const sensor_msgs::Joy::ConstPtr& joy){
  float  steering = joy->axes[0];
  float  forward  = -1*joy->axes[5]+1; //0 when not held down, the range of the triggers on the controller were -1 to 1, we made it 0 to 2
  float  reverse  = -1*joy->axes[2]+1; //2 when held down fully
  sacar::ESC escmsg; 
 
 //Can use something similar to this for when autonomy, checkout SemiAutonomous.cpp 
 if(forward){
    escmsg.steering_pulse = steering*375+steering_neutral;
    escmsg.throttle_pulse = 1500+250*forward; // throttle middle is 1500, we were using 2000 as full forward
  }
  else{
    escmsg.steering_pulse = steering*375+steering_neutral;
    escmsg.throttle_pulse = 1500-250*reverse;
  }

  esc_pub.publish(escmsg); //pushlish the new message
}

int main(int argc, char **argv)
{
 
  ros::init(argc, argv, "joy_listener"); 
  SACARTeleop lol; //instance of the class
  //ros::Rate loop_rate(100);
  ros::spin();

  return 0;
}
