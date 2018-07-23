
#ifndef __TCOMSALIENCY__
#define __TCOMSALIENCY__

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include "TLibCommon/TComDataCU.h"
#include "TLibCommon/TComMv.h"

class TComSaliency
{
private:
	cv::Mat m_ImgRgb;
	cv::Mat m_ImgSalSpatial;
	cv::Mat m_ImgSalTemporal;
	cv::Mat m_ImgSalTemporalHor;
	cv::Mat m_ImgSalTemporalVer;
	cv::Mat m_ImgSalAll;
	
public:
	//TComSaliency();
	TComSaliency(cv::Mat img);
	//virtual ~TComSaliency();
	// ∑√Œ 
	cv::Mat getSpatialSalencyMap() { return m_ImgSalSpatial; }
	cv::Mat getTemporalSalencyMap() { return m_ImgSalTemporal; }
	cv::Mat getAllSalencyMap() { return m_ImgSalAll; };
	cv::Mat getRGB() { return m_ImgRgb; };
	
	// …Ë÷√
	//void setRGB(cv::Mat img) { m_ImgRgb = img; }
	void settemporal(TComDataCU * pcCu);
	void setspatial(cv::Mat img) { m_ImgSalSpatial = img; }
	void generateSpatialSaliency();
	void generateAllSaliency();
};

#endif // __TCOMSALIENCY__