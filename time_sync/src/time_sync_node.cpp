#include <message_filters/subscriber.h>
#include <message_filters/synchronizer.h>
#include <message_filters/sync_policies/approximate_time.h>
#include <message_filters/sync_policies/exact_time.h>
#include <sensor_msgs/PointCloud2.h>
#include <sensor_msgs/Image.h>
#include <ros/ros.h>
#include <iostream>

using namespace std;
using namespace sensor_msgs;
using namespace message_filters;

PointCloud2 syn_pointcloud;
Image syn_image;

void callback(const PointCloud2ConstPtr& ori_pointcloud, const ImageConstPtr& ori_image)
{
	syn_pointcloud = *ori_pointcloud;
	syn_image = *ori_image;
	ROS_INFO("pointcloud timestamp value is %lf", syn_pointcloud.header.stamp.toSec());
	ROS_INFO("   Image   timestamp value is %lf", syn_image.header.stamp.toSec());
}

int main(int argc, char** argv)
{
	ros::init(argc, argv, "msg_sync");
	ros::NodeHandle nh;
	ROS_INFO("NODE INIT SUCCE");
	
	message_filters::Subscriber<Image> image_sub(nh, "/image_publisher/camera/image", 1);
	message_filters::Subscriber<PointCloud2> point_sub(nh, "/velodyne_points", 1);
	
	typedef sync_policies::ApproximateTime<PointCloud2, Image> MySyncPolicy;
	ROS_INFO(" POLICY INIT SUCCE");
	
	Synchronizer<MySyncPolicy> sync(MySyncPolicy(100), point_sub, image_sub);
	ROS_INFO("SYNC ING");
	sync.registerCallback(boost::bind(&callback, _1, _2));
	ros::spin();
	return 0;
}
