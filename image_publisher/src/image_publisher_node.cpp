#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

int main(int argc, char** argv)
{
	ros::init(argc, argv, "image_publisher");
	ros::NodeHandle nh("~");
	string Image_name_path = "";
	string Image_path = ""; 

	nh.getParam("Image_name_path",Image_name_path);

	nh.getParam("Image_path",Image_path);
	
	image_transport::ImageTransport it(nh);
	image_transport::Publisher pub = it.advertise("camera/image", 1);
	
	ros::Rate loop_rate(10);
	
	ifstream file;
	file.open(Image_name_path, ios_base::out);
	if (!file){
		ROS_INFO("Open file fail!");
		return -1;
	}
	string name;
	string file_path = string(Image_path);
	while (nh.ok() && !file.eof()){
	
	getline(file,name);
	ROS_INFO("Read %s successful",name.c_str());
	
	string full_path;
	full_path = file_path + "/" + name;
	string time_str = name.substr(27,17);
	ROS_INFO("The TimeStamp of the Image is %s",time_str.c_str());
	
	cv::Mat image = cv::imread(full_path,1);
	ROS_INFO("Read %s successful",full_path.c_str());
	
	sensor_msgs::ImagePtr msg = cv_bridge::CvImage(std_msgs::Header(),"bgr8",image).toImageMsg();
	msg->header.stamp = ros::Time(atof(time_str.c_str()));

	ROS_INFO("Change message timestamp as %lf",msg->header.stamp.toSec());
	pub.publish(msg);
	
	loop_rate.sleep();
	}
	ROS_INFO("Publish finished");
	return 0;
}
