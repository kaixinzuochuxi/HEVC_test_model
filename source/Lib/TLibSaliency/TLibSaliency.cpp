// TLibSaliency.cpp : Defines the entry point for the console application.
//


#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include "TLibSaliency.h"
#include <opencv2/imgcodecs.hpp>

TComSaliency::TComSaliency(cv::Mat img)
{
	img.convertTo(m_ImgRgb, CV_32FC3,1.0/255.0);
	m_ImgSalSpatial = cv::Mat::zeros(cv::Size(img.cols,img.rows),CV_32FC1);
	m_ImgSalTemporal= cv::Mat::zeros(cv::Size(img.cols, img.rows), CV_32FC1);
	m_ImgSalAll= cv::Mat::zeros(cv::Size(img.cols, img.rows), CV_32FC1);
	m_ImgSalTemporalHor = cv::Mat::zeros(cv::Size(img.cols, img.rows), CV_32FC1);
	m_ImgSalTemporalVer = cv::Mat::zeros(cv::Size(img.cols, img.rows), CV_32FC1);
}
//TComSaliency::~TComSaliency(){}

void TComSaliency::generateSpatialSaliency()
{
	cv::Mat ImgBlurred;
	// sigmax=sigmay=1
	cv::GaussianBlur(TComSaliency::getRGB(), ImgBlurred, cv::Size(5, 5),1,1);
	cv::Mat ImgLab;
	cv::cvtColor(ImgBlurred, ImgLab, CV_BGR2Lab);
	cv::Scalar mean_val = cv::mean(ImgLab);
	cv::Mat mean_matrix(ImgLab.rows, ImgLab.cols,CV_32FC3,mean_val);
	cv::Mat temp[3]; 
	cv::split((ImgLab - mean_val).mul((ImgLab - mean_val)), temp);
	
	cv::Mat finalsal;
	cv::normalize(temp[0] + temp[1] + temp[2], finalsal,0,1, cv::NORM_MINMAX);
	TComSaliency::setspatial(finalsal);
}

void TComSaliency::generateAllSaliency()
{

}


int main()
{
	cv::Mat img = cv::imread("D:/Projects/temporal_saliency/pic/BasketballDrill_832x480_50/1.png", cv::IMREAD_COLOR);
	TComSaliency test(img);
	test.generateSpatialSaliency();
	cv::namedWindow("saliency", cv::WINDOW_AUTOSIZE);
	cv::imshow("saliency", test.getSpatialSalencyMap());
	cv::waitKey();
	return 0;
}
