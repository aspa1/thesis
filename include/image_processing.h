#ifndef IMAGE_PROCESSING_H
#define IMAGE_PROCESSING_H

#include "svm_project/images.h"
#include <ros/package.h>
#include "boost/filesystem.hpp"  
#include "ros/ros.h"
#include <opencv2/highgui/highgui.hpp>

bool image_callback ( 	svm_project::images::Request  &req,
						svm_project::images::Response &res 	);


#endif
