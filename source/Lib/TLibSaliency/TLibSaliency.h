

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>



class TComSaliency
{
private:
	cv::Mat m_ImgRgb;
	cv::Mat m_ImgSalSpatial;
	cv::Mat m_ImgSalTemporal;
	cv::Mat m_ImgSalAll;
	
public:
	TComSaliency(cv::Mat img);
	//virtual ~TComSaliency();
	// ����
	cv::Mat getSpatialSalencyMap() { return m_ImgSalSpatial; }
	cv::Mat getTemporalSalencyMap() { return m_ImgSalTemporal; }
	cv::Mat getAllSalencyMap() { return m_ImgSalAll; };
	cv::Mat getRGB() { return m_ImgRgb; };
	
	// ����
	//void setRGB(cv::Mat img) { m_ImgRgb = img; }
	void settemporal(cv::Mat img) { m_ImgSalTemporal = img; }
	void setspatial(cv::Mat img) { m_ImgSalSpatial = img; }
	void generateSpatialSaliency();
	void generateAllSaliency();
};

