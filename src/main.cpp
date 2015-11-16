#ifndef MAIN
#define MAIN


#include "svm.h"
#include "ros/ros.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>



int main( int argc, char** argv )
{

    ros::init(argc, argv, "main");
    
	//~ svm_project::images srv;
	//~ srv.request.positives = ("/samples/positives");
	//~ srv.request.negatives = ("/samples/negatives");

	SVM svm1;
	
	ros::spin();

	return 0;

}

#endif