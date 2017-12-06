
#ifndef __DIRECTION__
#define __DIRECTION__

#include <cmath>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <stdlib.h>
#include <queue>
#include <vector>
#include <pthread.h>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/gpu/gpu.hpp>
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;
using namespace cv;

#define filepath_map "map.xml"	//��������ͷ����ӳ����ļ�·��
#define pi 3.1415926535898
#define angle_x 65.56	//����ͷx�᷽���ӳ���
#define angle_y 51.73		//����ͷy�᷽���ӳ���
//#define angle_x 60		//����ͷx�᷽���ӳ���
//#define angle_y 48.98		//����ͷy�᷽���ӳ���
#define size_r 42.5			//��ɫ�����ߴ磬��λcm
#define size_g 26.0			//��ɫ�����ߴ磬��λcm
#define size_b 13.0			//��ɫ�����ߴ磬��λcm
#define	HEIGHT 480			//ͼƬ�ĸ�
#define	WIDTH 640			//ͼƬ�Ŀ�


class direction
{
public:
	direction();
	~direction();

public:
	bool findDirection(const Mat& src,double theta,Point2f& center,Mat& d);
	bool trackDirection(const Mat& src,double theta,Point2f& center,Mat& d);
	bool ensureDirection(Point2f& center,double theta);

public:
	bool splitDirection(const Mat& src,vector<Scalar>& colors,vector<Mat>& directions);
	bool getCornerPoint(Mat& src,vector<Rect>& rect,vector<vector<Point2f> >& keypoints,double c);
	bool analizeCornerPoint(Mat& src,vector<Rect>& rect,vector<vector<Point2f> >& keypoints);
	bool transformCoordinate(vector<Point2f>& keypoint,Scalar& colors,double theta,Point2f& center,Mat& d);
	int rows();

public:
	vector<Rect> trackBox;

private:
	Mat kernel;
	vector<Scalar> colors;

private:
	int color_level;
	vector<Point2f> real_center_history;
	vector<float> relative_height_history;
};



#endif
