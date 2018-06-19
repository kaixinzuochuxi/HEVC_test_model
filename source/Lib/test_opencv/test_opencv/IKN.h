#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc.hpp>
#include <opencv2\imgcodecs.hpp>
#pragma once
#include<vector>
#include <cmath>

// todo : resize change to pyrup
// normalization

using namespace cv;
using namespace std;
class IKN
{
	
};

int pyramid_scale = 8;
vector<int> center_scale = { 2,3,4 };
vector<int> surrounding_scale = {3,4};
vector<double> theat = {0, CV_PI * 0.25,CV_PI * 0.5,CV_PI * 0.75 };
std::vector<Mat> Gaussian_pyramid;
std::vector<Mat> I;
std::vector<Mat> R;
std::vector<Mat> G;
std::vector<Mat> B;
std::vector<Mat> Y;
vector<vector<Mat>> I_feature;
vector<vector<Mat>> RG_feature;
vector<vector<Mat>> BY_feature;
//vector<vector<vector<Mat>>> O_feature;
vector<Mat> None;
vector<vector<Mat>> O_feature;
int img_width;
int img_height;
Mat I_ave;
Mat C_ave;
Mat O_ave;
Mat S;
//using input image to generate a g_p, return a vector of mat
void generate_Gaussian_pyramid(Mat image, int pyramid_scale, std::vector<Mat> &Gaussian_pyramid, std::vector<Mat> &I, std::vector<Mat> &R, std::vector<Mat> &G, std::vector<Mat> &B, std::vector<Mat> &Y)
{

	//first row, picture pyramid
	
	img_width = image.cols;
	img_height = image.rows;
	//Gaussian_pyramid.push_back(image); ??? 可能有所改变
	buildPyramid(image, Gaussian_pyramid, pyramid_scale);
	for (int p_s = 0; p_s <= pyramid_scale; p_s++)
	{
		Mat temp = Gaussian_pyramid[p_s];
		Mat bgr[3];
		split(temp, bgr);
		I.push_back((bgr[0] + bgr[1] + bgr[2]) / 3);
		R.push_back(bgr[2] - (bgr[0] + bgr[1]) / 2);
		G.push_back(bgr[1] - (bgr[0] + bgr[2]) / 2);
		B.push_back(bgr[0] - (bgr[1] + bgr[2]) / 2);
		Y.push_back(((bgr[1] + bgr[2]) / 2 - max(bgr[1] - bgr[2], 0) / 2 - bgr[0]));
	}


	
}




Mat Across_Scale_Difference(Mat Scale_c, Mat Scale_s)
{
	Mat Scale_s_resize;
	resize(Scale_s, Scale_s_resize, Size(Scale_c.cols, Scale_c.rows));
	return (Scale_c -Scale_s_resize);
}

Mat Across_scale_addition(Mat Scale_c, Mat Scale_s, Size size_scale)
{
	Mat Scale_c_resize;
	Scale_c_resize.convertTo(Scale_c_resize, CV_32F, 1 / 255.0);
	Mat Scale_s_resize;
	Scale_s_resize.convertTo(Scale_s_resize, CV_32F, 1 / 255.0);
	resize(Scale_c, Scale_c_resize, size_scale);
	resize(Scale_s, Scale_s_resize, size_scale);
	return (Scale_c_resize + Scale_s_resize);
}

void IKN_normalization(Mat image)
{
	
}


void generate_feature(std::vector<Mat> Gaussian_pyramid, std::vector<Mat> I, std::vector<Mat> R, std::vector<Mat> G, std::vector<Mat> B, std::vector<Mat> Y, vector<int> center_scale, vector<int> surrounding_scale, vector<double> theat, vector<vector<Mat>> &I_feature, vector<vector<Mat>> &RG_feature, vector<vector<Mat>> &BY_feature, vector<vector<Mat>> &O_feature)
{
	for (int c_val : center_scale)
	{
		I_feature.push_back(None);
		RG_feature.push_back(None);
		BY_feature.push_back(None);
		O_feature.push_back(None);
		for (int s_val : surrounding_scale)
		{
			I_feature[c_val - center_scale[0]].push_back(Across_Scale_Difference(I[c_val], I[c_val + s_val]));
			RG_feature[c_val - center_scale[0]].push_back(Across_Scale_Difference(R[c_val] - G[c_val], G[c_val + s_val] - R[c_val + s_val]));
			BY_feature[c_val - center_scale[0]].push_back(Across_Scale_Difference(B[c_val] - Y[c_val], Y[c_val + s_val] - B[c_val + s_val]));
			//O_feature[c_val - center_scale[0]].push_back(None);

			for (double theta_i : theat)
			{
				Mat gabor_c;
				Mat gabor_s;
				//tuning parameters
				filter2D(I[c_val], gabor_c, CV_32F, getGaborKernel(Size(5, 5), 10, theta_i, 10, 0));
				filter2D(I[c_val + s_val], gabor_s, CV_32F, getGaborKernel(Size(5, 5), 10, theta_i, 10, 0));
				O_feature[c_val - center_scale[0]].push_back(Across_Scale_Difference(gabor_c, gabor_s));
			}
			
		}
		
	}
	




}


void IKN_combine(Mat &I_ave, Mat &C_ave, Mat &O_ave,Mat &S, vector<int> center_scale, vector<int> surrounding_scale, vector<double> theat,vector<vector<Mat>> I_feature, vector<vector<Mat>> RG_feature, vector<vector<Mat>> BY_feature, vector<vector<Mat>> O_feature,Size size)
{
	Mat temp(size, CV_32F, Scalar(0));
	I_ave = temp;
	C_ave = temp;
	O_ave = temp;
	S = temp;
	for (int c_val : center_scale)
	{
		for (int s_val : surrounding_scale)
		{
			I_ave = Across_scale_addition(I_ave, I_feature[c_val - center_scale[0]][s_val - surrounding_scale[0]], size);
			C_ave = Across_scale_addition(C_ave,
				Across_scale_addition(RG_feature[c_val - center_scale[0]][s_val - surrounding_scale[0]], BY_feature[c_val - center_scale[0]][s_val - surrounding_scale[0]], size),
				size);
			for (int theta_i=0; theta_i< (surrounding_scale.size())*(theat.size()); theta_i++)
			{
				O_ave = Across_scale_addition(O_ave, O_feature[c_val - center_scale[0]][theta_i], size);
			}
				
		}
	}
	S = (I_ave + C_ave + O_ave) / 3;
	
}