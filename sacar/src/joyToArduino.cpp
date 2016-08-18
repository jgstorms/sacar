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
  float steering = joy->axes[0];
  int   forward  = joy->buttons[0];
  int   reverse  = joy->buttons[1];
  sacar::ESC escmsg; 
  if(forward){
    escmsg.steering_pulse = steering*375+steering_neutral;
    escmsg.throttle_pulse = 1500+500*forward;
  }
  else{
    escmsg.steering_pulse = steering*375+steering_neutral;
    escmsg.throttle_pulse = 1500-500*reverse;
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
