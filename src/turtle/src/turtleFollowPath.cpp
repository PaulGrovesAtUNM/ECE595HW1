#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "nav_msgs/Odometry.h"
#include <tf/tf.h>

ros::Publisher *ctrl;

void ctrlCallback(nav_msgs::Odometry msg)
{
	//Publish a message to TurtleSim	

	

	geometry_msgs::Twist cmd;

	cmd.angular.z = .1;
	

	double roll, pitch, yaw;
	tf::Quaternion q;
	tf::quaternionMsgToTF(msg.pose.pose.orientation, q);
	tf::Matrix3x3(q).getEulerYPR(yaw, pitch, roll);

	ROS_INFO("Odometry: (%f, %f) YPR:%f,%f,%f", msg.pose.pose.position.x, msg.pose.pose.position.y, yaw, roll, pitch);
	

	ctrl->publish(cmd);
}

int main(int argc, char **argv)
{
	ros::init(argc, argv, "pathFollower");
	ros::NodeHandle n;

	ROS_INFO("Starting...");

	ros::Publisher c = n.advertise<geometry_msgs::Twist>("mobile_base/commands/velocity", 10);	
	ctrl = &c;	

	ros::Rate loop_rate(10);

	//Setup our subscribers.
	ros::Subscriber	ctrler = n.subscribe("/odom", 10, ctrlCallback);	

	geometry_msgs::Twist cmd;

	cmd.linear.x = 1;
	cmd.linear.y = 0;
	cmd.linear.z = 0;
	cmd.angular.x = 0;
	cmd.angular.y = 0;
	cmd.angular.z = 0;

	ROS_INFO("Moving the Turtle...");

	c.publish(cmd); //Begin moving our turtle.
	ROS_INFO("Entering our loop...");
	ros::spin();

	
	//while( ros::ok )
	//{
//		ros::spinOnce();
		//loop_rate.sleep();
	//}

	return 0;
}
