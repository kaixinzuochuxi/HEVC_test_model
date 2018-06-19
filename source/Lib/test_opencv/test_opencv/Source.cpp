#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc.hpp>
#include <opencv2\imgcodecs.hpp>
#include <cstdio>
#include <iostream>
#include <vector>
#include <cmath>
#include "IKN.h"
using namespace cv;

int main()
{
	//Mat srcImage = imread("1.jpg");      //加载图像文件   "test_opencv\Resource Files\1.jpg"
	//namedWindow("lena", WINDOW_AUTOSIZE);   //设置显示图像的窗口标题为lena,属性为自动调整大小
	//std::vector<Mat> bgr;
	//Mat bgr[3];
	//split(srcImage, bgr);



	//imshow("lena", bgr[1]);               //显示图片

	//int a = srcImage.cols;
	//printf("%d %d %d", bgr[1].data[0,0]+ bgr[1].data[0, 0]+ bgr[2].data[0, 0]);
	//printf("%d", srcImage.data[1,1]);
	//waitKey(0);
	//return 0;

	//std::vector<std::vector<std::vector<int>>> c;
	//std::vector<std::vector<int>> a;
	//std::vector<int> b;
	//for (int i = 0; i < 3; i++)
	//{

	//	c.push_back(a);
	//	for (int j = 0; j < 3; j++)
	//	{
	//		c[i].push_back(b);
	//		for (int k = 0; k < 3; k++)
	//		{
	//			c[i][j].push_back(k);
	//		}
	//	}
	//}

	//for (int i = 0; i < 3; i++)
	//{
	//	for (int j = 0; j < 3; j++)
	//	{
	//		for (int k = 0; k < 3; k++)
	//		{
	//			std::cout << c[i][j][k];
	//		}
	//	}
	//}


	//Mat srcImage = imread("1.png",1);
	//int a = srcImage.data[10, 10];
	//srcImage.convertTo(srcImage,CV_32FC3,1/255.0);
	//srcImage.at<float>(10, 10) = 299;
	////namedWindow("lena", WINDOW_AUTOSIZE);
	////imshow("lena", srcImage);
	//srcImage = srcImage + srcImage;
	//float b= srcImage.at<float>(10, 10);
	//printf("%f", srcImage.at<float>(10, 10));
	//waitKey(0);



	//test IKN
	Mat image = imread("1.png", CV_32FC3);
	image.convertTo(image, CV_32FC3,1/255.0);
	//vector<Mat> temp;
	//temp.convertTo(temp, CV_32FC3,1/255.0);
	/*int p_s = pow(2,2);
	printf("%d", pow(2, p_s));
	pyrDown(image, temp, Size(image.cols / 4, image.rows /4));
	namedWindow("lena", WINDOW_AUTOSIZE);
	imshow("temp", temp);*/

	//buildPyramid(image, temp, 8);
	generate_Gaussian_pyramid( image,  pyramid_scale, Gaussian_pyramid,  I,  R,  G,  B, Y);
	generate_feature(Gaussian_pyramid, I, R, G, B, Y, center_scale, surrounding_scale, theat, I_feature,RG_feature, BY_feature, O_feature);
	
	IKN_combine(I_ave, C_ave, O_ave, S, center_scale, surrounding_scale, theat, I_feature, RG_feature, BY_feature, O_feature,image.size());
	//namedWindow("lena", WINDOW_AUTOSIZE);
	//imshow("0", I[0]);
	imshow("1", C_ave);
	//imshow("2", I[2]);
	//imshow("3", I[3]);
	//imshow("4", I[4]);
	//imshow("5", I[5]);
	//imshow("6", I[6]);
	//imshow("7", I[7]);
	//imshow("8", I[8]);

	waitKey(0);
}