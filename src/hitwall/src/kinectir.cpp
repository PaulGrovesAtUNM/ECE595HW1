#include <iostream>
#include <math.h>
#include "ros/ros.h"
#include "sensor_msgs/Image.h"
#include "std_msgs/Empty.h"
#include "geometry_msgs/Twist.h"

bool KeepGoing = true;
bool ShouldMeasureInitialPosition = true;
float InitialPosition = 0;
float xpos;

void subFunc(sensor_msgs::Image eventMsg)
{
	int dataHeight = eventMsg.height;
	int dataWidth = eventMsg.width;
	int dataStep = eventMsg.step;
	
	// This message contains an uncompressed image
	// (0, 0) is at top-left corner of image

	//uint32 height         # image height, that is, number of rows
	//uint32 width          # image width, that is, number of columns

	//uint8 is_bigendian    # is this data bigendian?
	//uint32 step           # Full row length in bytes
	//uint8[] data          # actual matrix data, size is (step * rows)
	
	int centerInd = floor((dataStep*dataHeight/2.0)-(dataStep/2.0));
	int centerValue = eventMsg.data[centerInd];
	//KeepGoing = false;
	std::cout << " \n h: " << dataHeight << " w: " << dataWidth <<  " s: " << dataStep  << " i: " << centerInd << " v: " << centerValue;
}

int main(int argc, char **argv)
{

	ros::init(argc, argv, "kinectir");
	ros::NodeHandle nodeName;
	ros::Rate loop_rate(10);
	
	ros::Publisher pubName = nodeName.advertise<geometry_msgs::Twist>("/mobile_base/commands/velocity", 1000);
	
		ros::Subscriber subName = nodeName.subscribe("/camera/ir/image_raw", 100, subFunc);
		
	while (ros::ok())
  	{
  	
  		if(KeepGoing == true){
  		geometry_msgs::Twist twistMsg;
		twistMsg.linear.x=0.0;
		twistMsg.linear.y=0.0;
		twistMsg.linear.z=0.0;
		twistMsg.angular.x=0.0;
		twistMsg.angular.y=0.0;
		twistMsg.angular.z=0.0;
	  	
	  	pubName.publish(twistMsg);
	  	} else {
	  	//std::cout << " Really Stopped!!!";
	  	}
	  	
    	ros::spinOnce();
    	loop_rate.sleep();
  	}

}
