
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

#define filepath_map "map.xml"	//保存摄像头矫正映射的文件路径
#define pi 3.1415926535898
#define angle_x 65.56	//摄像头x轴方向视场角
#define angle_y 51.73		//摄像头y轴方向视场角
//#define angle_x 60		//摄像头x轴方向视场角
//#define angle_y 48.98		//摄像头y轴方向视场角
#define size_r 42.5			//红色方向标尺寸，单位cm
#define size_g 26.0			//绿色方向标尺寸，单位cm
#define size_b 13.0			//蓝色方向标尺寸，单位cm
#define	HEIGHT 480			//图片的高
#define	WIDTH 640			//图片的宽


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
