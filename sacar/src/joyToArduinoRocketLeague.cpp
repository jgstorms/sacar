#include "ros/ros.h"
#include "sacar/ESC.h"
#include <sensor_msgs/Joy.h>

int steering_left = 2000;
int steering_right = 1250;
int steering_neutral = 1625;

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

SACARTeleop::SACARTeleop()
{
  esc_pub = nh.advertise<sacar::ESC>("throttleandsteering", 10);
  joy_sub = nh.subscribe<sensor_msgs::Joy>("joy", 10, &SACARTeleop::joyCallback, this);
}



void SACARTeleop::joyCallback(const sensor_msgs::Joy::ConstPtr& joy){
  float  steering = joy->axes[0];
  float  forward  = -1*joy->axes[5]+1; //0 when not held down
  float  reverse  = -1*joy->axes[2]+1; //2 when held down fully
  sacar::ESC escmsg; 
  if(forward){
    escmsg.steering_pulse = steering*375+steering_neutral;
    escmsg.throttle_pulse = 1500+250*forward;
  }
  else{
    escmsg.steering_pulse = steering*375+steering_neutral;
    escmsg.throttle_pulse = 1500-250*reverse;
  }

  esc_pub.publish(escmsg); 
}

int main(int argc, char **argv)
{
 
  ros::init(argc, argv, "joy_listener");
  SACARTeleop lol;
  //ros::Rate loop_rate(100);
  ros::spin();

  return 0;
}
