#include "ros/ros.h"
#include <tf/transform_broadcaster.h>
#include <nav_msgs/Odometry.h>
#include "sacar/Encoder.h"
int    update_rate=100; //same as arduino (in ms)
double vx=0;
double wheel_circumference=30; // cm

//Similar to joyToArduino, everything needed for the Pushlisher and Subscriber are done in the class
class SACARSpeedPublisher
{
public:
  SACARSpeedPublisher();

private:
  void encoderCallback(const sacar::Encoder::ConstPtr& msg);
  ros::NodeHandle n;
  ros::Publisher odom_pub;
  ros::Subscriber encoder_sub;
  
};

//publishes to sacar/odom and subscribes to encoder_speed
SACARSpeedPublisher::SACARSpeedPublisher()
{
  odom_pub = n.advertise<nav_msgs::Odometry>("sacar/odom", 50);
  encoder_sub= n.subscribe<sacar::Encoder>("encoder_speed", 50, &SACARSpeedPublisher::encoderCallback, this);
}

//Call back function, takes the encoder messages on topics and puts them into useable local variables
// Then uses these to calculate the odom data, and publishes this 
void SACARSpeedPublisher::encoderCallback(const sacar::Encoder::ConstPtr& msg){
    ros::Time current_time;
    current_time = ros::Time::now();
    vx = 10*wheel_circumference*(msg->BL + msg->BR)/4/update_rate; //in m/s
    //next, we'll publish the odometry message over ROS
    nav_msgs::Odometry odom;
    odom.header.stamp = current_time;
    odom.header.frame_id = "odom";

    //set the position
    //odom.pose.pose.position.x = 0.0;
    //odom.pose.pose.position.y = 0.0;
    //odom.pose.pose.position.z = 0.0;
    //odom.pose.pose.orientation = odom_quat;

    //set the velocity
    odom.child_frame_id = "base_link";
    odom.twist.twist.linear.x = vx;
    odom.twist.twist.linear.y = 0;
    odom.twist.twist.angular.z = 0;
    //publish the message
    odom_pub.publish(odom);         
}


int main(int argc, char** argv){
  ros::init(argc, argv, "encoderToOdom");
  SACARSpeedPublisher sp;
  ros::spin();              
}
