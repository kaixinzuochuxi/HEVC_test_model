// TLibSaliency.cpp : Defines the entry point for the console application.
//


#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include "TLibSaliency.h"
#include <opencv2/imgcodecs.hpp>
#include "TLibCommon/TComDataCU.h"
#include "TLibCommon/TComMv.h"


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

// 按照每个CTU的顺序设置当前CTU的运动向量
/*
包含文件：
- TComDataCU.h,TComDataCU.cpp
输入：
步骤：
- slice编码中，CTU编码完成后，访问每个CTU，获取起始x，y，运动向量
- 对每个CTU，获取m_uiNumPartition，最小块划分的个数,m_unitSize最小块的边长。
- Z扫描顺序依次访问运动向量，非0，转换raster扫描顺序，获取位置，写入4*4块。x,y分开，最后合并成总共的。

*/

void TComSaliency::settemporal(TComDataCU* pcCu)
{
	//UInt ctustartx = pcCu->getCUPelX();
	//UInt ctustarty = pcCu->getCUPelY();
	UInt numpart = pcCu->getTotalNumPart();
	Int minsize = pcCu->getMinSize();
	for (int i = 0; i < numpart; i++)
	{
		UInt Hormv = pcCu->getCUMvField(REF_PIC_LIST_0)->getMv(i).getHor();
		UInt Vermv = pcCu->getCUMvField(REF_PIC_LIST_0)->getMv(i).getVer();
		if(Hormv != 0)
		{
			UInt uiLPelX = pcCu->getCUPelX() + g_auiRasterToPelX[g_auiZscanToRaster[i]];
			UInt uiTPelY = pcCu->getCUPelY() + g_auiRasterToPelY[g_auiZscanToRaster[i]];
			
			for (int xdir = 0; xdir < minsize; xdir++)
				for(int ydir = 0;ydir < minsize; ydir++)
				{
					m_ImgSalTemporalHor.data[uiLPelX+ xdir, uiTPelY+ ydir] = Hormv;
				}


		}
		else if (Vermv != 0)
		{
			UInt uiLPelX = pcCu->getCUPelX() + g_auiRasterToPelX[g_auiZscanToRaster[i]];
			UInt uiTPelY = pcCu->getCUPelY() + g_auiRasterToPelY[g_auiZscanToRaster[i]];
			
			for (int xdir = 0; xdir < minsize; xdir++)
				for (int ydir = 0; ydir < minsize; ydir++)
				{
					m_ImgSalTemporalVer.data[uiLPelX + xdir, uiTPelY + ydir] = Vermv;
				}
		}
	}
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
