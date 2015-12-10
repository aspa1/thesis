#ifndef SVM_H
#define SVM_H

#include <iostream>

#include <ros/ros.h>
#include <ros/package.h>
#include <ros/console.h>
#include <sensor_msgs/Image.h>

#include <opencv/ml.h>
#include <opencv2/highgui/highgui.hpp>

#include <boost/filesystem.hpp>

#include "svm_project/trainSvmSrv.h"
#include "svm_project/urlRetrieverSrv.h"

class SVM 
{		
	private:	
		ros::NodeHandle _n;
		ros::ServiceServer _uri_retriever_service;
		ros::ServiceServer _image_receiver_service;
		CvSVM _svm;
				
	public:
		SVM();
		bool fileExist( const std::string& imagePath );
		bool svmTrain( svm_project::trainSvmSrv::Request &req, svm_project::trainSvmSrv::Response &res );
		bool svmPredict( svm_project::urlRetrieverSrv::Request &req, svm_project::urlRetrieverSrv::Response &res );
		std::string type2str(int type);
		std::vector<std::vector<float> > getAllFilesFromDir(std::string dir);
		std::vector<float> imgRead(std::string path);
};



#endif