#ifndef ROBOT_PERCEPTION_H
#define ROBOT_PERCEPTION_H

#include "nav_msgs/OccupancyGrid.h"
#include "sensor_msgs/LaserScan.h"
#include <ros/ros.h>
#include <ros/package.h>


class RobotPerception {
	private:
		ros::NodeHandle _n;
		std::string _map_topic_param;
		std::string _laser_topic_param;
		ros::Subscriber _map_sub;
		ros::Subscriber _laser_sub;
		unsigned int _map_width;
		unsigned int _map_height;
		int** _map_data; 
		float* _laser_ranges;
	
	
	public:
		RobotPerception();
		void mapCallback(nav_msgs::OccupancyGrid occupancy_grid_msg);
		void laserRangesCallback(sensor_msgs::LaserScan laser_scan_msg);
		int** getMapData();
		float* getLaserRanges();
};

#endif
