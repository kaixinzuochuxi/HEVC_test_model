#include <cstdlib>
#include <assert.h>
#include <memory.h>

#ifdef __APPLE__
#include <malloc/malloc.h>
#else
#include <malloc.h>
#endif

#include "../TLibCommon/TComPicYuv.h"
#include "../TLibVideoIO/TVideoIOYuv.h"
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "../test_opencv/test_opencv/IKN.h"
#include "ReadYuv.h"


//void convertYUVtoRGB(cv::Mat &img_rgb, TComPicYuv *pcPicCurr, const BitDepths &bitDepths, const Bool bAppend, const Bool bForceTo8Bit)
void convertYUVtoRGB(TComPicYuv *pcPicCurr, const BitDepths &bitDepths, const Bool bAppend, const Bool bForceTo8Bit)
{
	Bool is16bit = false;

	//检查bits-recon0，luma，recon2，chorma
	for (Int comp = 0; comp < pcPicCurr->getNumberValidComponents() && !bForceTo8Bit; comp++)
	{
		if (bitDepths.recon[toChannelType(ComponentID(comp))]>8)	////////luma,chorma
		{
			is16bit = true;
		}
	}

	/////////////////////////
	Int comp = 0;
	const ComponentID  compId = ComponentID(comp);
	const Pel         *pi = pcPicCurr->getAddr(compId);
	const Int          stride = pcPicCurr->getStride(compId);
	const Int          h = pcPicCurr->getHeight(compId);
	const Int          w = pcPicCurr->getWidth(compId);
	
	
	int bufLen = w*h * 3 / 2;
	//int bufLen = w*h;
	UChar* pYuvBuf = new UChar[bufLen];
	UChar *temp = pYuvBuf;
	

	/////////////////////////

	for (Int comp = 0; comp < pcPicCurr->getNumberValidComponents(); comp++)
	{
		const ComponentID  compId = ComponentID(comp);
		const Pel         *pi = pcPicCurr->getAddr(compId);
		const Int          stride = pcPicCurr->getStride(compId);
		const Int          height = pcPicCurr->getHeight(compId);
		const Int          width = pcPicCurr->getWidth(compId);
		//Y:wh原始大小，UV大小变为一般
		
		
		//YUV格式，行width，列hight，先按行存储，再按列存储，先索引列，再索引行
		if (is16bit)
		{
			for (Int y = 0; y < height; y++)
			{
				for (Int x = 0; x < width; x++)
				{
					//img_rgb.at<UChar>(x,y) = (UChar)((pi[x] >> 0) & 0xff);
					//fwrite(&uc, sizeof(UChar), 1, pFile);
					//uc = (UChar)((pi[x] >> 8) & 0xff);
					//fwrite(&uc, sizeof(UChar), 1, pFile);
				}
				pi += stride;
			}
		}
		else
		{
			const Int shift = bitDepths.recon[toChannelType(compId)] - 8;
			const Int offset = (shift>0) ? (1 << (shift - 1)) : 0;
			for (Int y = 0; y < height; y++)
			{
				for (Int x = 0; x < width; x++)
				{
					///找中间值
					*(temp++) = (UChar)Clip3<Pel>(0, 255, (pi[x] + offset) >> shift);
					//fwrite(&uc, sizeof(UChar), 1, pFile);
				}
				pi += stride;
			}
		}


	}
	/////////////
	Mat yuvImg;
	yuvImg.create(h * 3 / 2, w, CV_8UC1);
	//yuvImg.create(h , w, CV_8UC3);
	//////memcpy!!!
	memcpy(yuvImg.data, pYuvBuf, bufLen * sizeof(UChar));
	Mat rgbImg(h, w, CV_8UC3);
	//yuvImg.convertTo(rgbImg, CV_YUV2RGB_YV12,1/255.0);
	cvtColor(yuvImg, rgbImg, CV_YUV2RGB_YV12, 3);

	//////////
	vector<Mat> gp;
	buildPyramid(rgbImg, gp, 2);
	rgbImg.convertTo(rgbImg, CV_32FC3, 1 / 255.0);
	generate_Gaussian_pyramid(rgbImg, pyramid_scale, Gaussian_pyramid, I, R, G, B, Y);
	generate_feature(Gaussian_pyramid, I, R, G, B, Y, center_scale, surrounding_scale, theat, I_feature, RG_feature, BY_feature, O_feature);

	IKN_combine(I_ave, C_ave, O_ave, S, center_scale, surrounding_scale, theat, I_feature, RG_feature, BY_feature, O_feature, rgbImg.size());
	//////////

	imshow("img", I_ave);
	//printf("%d\n", 1);
	waitKey(5);
}


void DisplayYUV()
{
	int w = 416;
	int h = 240;
	printf("yuv file w: %d, h: %d \n", w, h);

	FILE* pFileIn = fopen("test.yuv", "rb+");
	int bufLen = w*h * 3 / 2;
	//int bufLen = w*h;
	unsigned char* pYuvBuf = new unsigned char[bufLen];
	int iCount = 0;


	//for (int i = 0; i<1; i++)
	
	fread(pYuvBuf, bufLen * sizeof(unsigned char), 1, pFileIn);

	cv::Mat yuvImg;
	yuvImg.create(h * 3 / 2, w, CV_8UC1);
	//yuvImg.create(h , w, CV_8UC3);
	//////memcpy!!!
	memcpy(yuvImg.data, pYuvBuf, bufLen * sizeof(unsigned char));
	cv::Mat rgbImg(h,w, CV_8UC3);
	//yuvImg.convertTo(rgbImg, CV_YUV2RGB_YV12,1/255.0);
	cv::cvtColor(yuvImg, rgbImg, CV_YUV2RGB_YV12, 3);

	cv::imshow("img", rgbImg);
	cv::waitKey(1);

	printf("%d \n", iCount++);
	
	//cv::Mat rgbImg(h, w, CV_8UC3);
	//for (int y = 0; y < h; y++)
	//{
	//	for (int x = 0; x < w; x++)
	//	{

	//	}

	//}
	delete[] pYuvBuf;


	fclose(pFileIn);
}

