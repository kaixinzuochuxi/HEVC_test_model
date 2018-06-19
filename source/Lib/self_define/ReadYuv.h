#pragma once
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



//void convertYUVtoRGB(cv::Mat &img_rgb, TComPicYuv *pcPicCurr, const BitDepths &bitDepths, const Bool bAppend, const Bool bForceTo8Bit)
void convertYUVtoRGB(TComPicYuv *pcPicCurr, const BitDepths &bitDepths, const Bool bAppend, const Bool bForceTo8Bit);



void DisplayYUV();

