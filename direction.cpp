
#include "direction.h"



direction::direction()
{
	kernel = getStructuringElement(MORPH_ELLIPSE,Size(5,5));

	colors.push_back(Scalar(175,255,255));
	colors.push_back(Scalar(60,255,255));
	colors.push_back(Scalar(110,255,255));

	color_level = 0;
}


direction::~direction()
{

}


bool direction::findDirection(const Mat& src,double theta,Point2f& center,Mat& d)
{
#define debug_findDirection 0
	// **������** //
	if (!src.data)
	{
#if debug_findDirection
		cout << ">ERROR: the image delivered to \"findDirection\" is empty!" << endl;
#endif
		return false;
	}
	if (theta>75 || theta<0)
	{
#if debug_findDirection
		cout << ">ERROR: the theta delivered to \"findDirection\" is illegal!" << endl;
#endif
		return false;
	}

#if debug_findDirection
	double t = getTickCount();
	cout << ">>>>**********findDirection**********<<<<" << endl;
#endif

	// **��ɫ�ռ�ת��** //
	Mat hsv;
	GaussianBlur(src,hsv,Size(7,7),0);
	cvtColor(hsv,hsv,COLOR_BGR2HSV);
	
	// **������ɫ����hsvͼ** //
	vector<Mat> directions;
	if (!splitDirection(hsv,colors,directions))
	{
#if debug_findDirection
		cout << ">ERROR: fail to split directions in function \"findDirection\" !" << endl;
#endif
		return false;
	}

#if debug_findDirection
	cout < < ">>>split:\t" << (getTickCount()-t)/getTickFrequency() << endl;
	t = getTickCount();
#endif

	// **���ݶ�ֵͼ��ȡ��ѡ�����ǵ�** //
	vector<Rect> dirTemp;
	vector<vector<Point2f> > keypoints;
	int i;
	for (i=0; !getCornerPoint(directions[i],dirTemp,keypoints,0.04) && i<2; i++);
	if (dirTemp.size()<1 || keypoints.size()!=dirTemp.size())
	{
#if debug_findDirection
		cout << ">WARMING: can not find direction through the camera!" << endl;
#endif
		return false;
	}
	
#if debug_findDirection
	cout << ">>>corner:\t" << (getTickCount()-t)/getTickFrequency() << endl;
	t = getTickCount();
#endif

	// **������ѡ�����ǵ�** //
	if (!analizeCornerPoint(directions[i],dirTemp,keypoints))
	{
#if debug_findDirection
		cout << ">WARMING: this direction is illegal!" << endl;
#endif
		return false;
	}
	color_level = i;

#if debug_findDirection
	cout << ">>>anal:\t" << (getTickCount()-t)/getTickFrequency() << endl;
	t = getTickCount();
#endif

	// **����ת��** //
	if (!transformCoordinate(keypoints[0],colors[color_level],theta,center,d))
	{
#if debug_findDirection
		cout << ">ERROR: fail to transform coordinate!" << endl;
#endif
		return false;
	}

#if debug_findDirection
	cout << ">>>trans:\t" << (getTickCount()-t)/getTickFrequency() << endl;
#endif

	trackBox.push_back(dirTemp[0]);
	return true;
}




bool direction::trackDirection(const Mat& src,double theta,Point2f& center,Mat& d)
{
#define debug_trackDirection 0
	//**������**//
	if (!src.data)
	{
#if debug_trackDirection
		cout << "ERROR: the image delivered to \"trackDirection\" is empty!" << endl;
#endif
		return false;
	}
	if (theta>75 || theta<0)
	{
#if debug_trackDirection
		cout << "ERROR: the theta delivered to \"trackDirection\" is illegal!" << endl;
#endif
		return false;
	}

#if debug_trackDirection
	double t = getTickCount();
	cout << ">>>**********trackDirection**********<<<" << endl;
#endif
	{
		float button,left,top,right,add_size;
		int ave_size = (trackBox[0].height+trackBox[0].width)/2;
		//add_size = (int)(-0.1143*ave_size+55.7143);
		add_size = (int)(0.2*ave_size+20);
		button = trackBox[0].y>add_size ? trackBox[0].y-add_size:0;
		left = trackBox[0].x>add_size ? trackBox[0].x-add_size:0;
		top = trackBox[0].y+trackBox[0].height+add_size<src.rows ? trackBox[0].y+trackBox[0].height+add_size:src.rows;
		right = trackBox[0].x+trackBox[0].width+add_size<src.cols ? trackBox[0].x+trackBox[0].width+add_size:src.cols;
		trackBox[0] = Rect(Point(left,button),Point(right,top));
	}

	//**��ɫ�ռ�ת��**//
	
	Point2f offset = trackBox[0].tl();//Rect.tl()ȡ�����Ͻǵĵ�����

	Mat hsv = src(trackBox[0]).clone();
#if debug_trackDirection
	//imshow("trackBox",hsv);
#endif
	GaussianBlur(hsv,hsv,Size(7,7),0);
	cvtColor(hsv,hsv,COLOR_BGR2HSV);

	//**������ɫ����hsvͼ**//
	vector<Mat> directions;
	if (!splitDirection(hsv,colors,directions))
	{
#if debug_trackDirection
		cout << "ERROR: fail to split directions in function \"trackDirection\" !" << endl;
#endif
		return false;
	}

#if debug_trackDirection
	cout << ">>>split:\t" << (getTickCount()-t)/getTickFrequency() << endl;
	t = getTickCount();
#endif

	//**���ݶ�ֵͼ��ȡ��ѡ�����ǵ�**//
	vector<Rect> dirTemp;
	vector<vector<Point2f> > keypoints;
	int i;
	for (i=color_level; !getCornerPoint(directions[i],dirTemp,keypoints,0.04) && i<2; i++);
	if (dirTemp.size()<1 || keypoints.size()!=dirTemp.size())
	{
#if debug_trackDirection
		cout << "WARMING: can not find direction through the camera!" << endl;
#endif
		return false;
	}

#if debug_trackDirection
	cout << ">>>corner:\t" << (getTickCount()-t)/getTickFrequency() << endl;
	t = getTickCount();
#endif

	//**������ѡ�����ǵ�**//
	if (!analizeCornerPoint(directions[i],dirTemp,keypoints))
	{
#if debug_trackDirection
		cout << "WARMING: this direction is illegal!" << endl;
#endif
		return false;
	}
	color_level = i;

#if debug_trackDirection
	cout << ">>>anal:\t" << (getTickCount()-t)/getTickFrequency() << endl;
	t = getTickCount();
#endif

	//**������ӳ��**//
	for (i=0; i<keypoints[0].size(); i++)
		keypoints[0][i] += offset;
	Point off_temp = Point( (int)offset.x,(int)offset.y );		
	dirTemp[0] = Rect(dirTemp[0].tl()+off_temp,dirTemp[0].br()+off_temp);

	//**����ת��**//
	if (!transformCoordinate(keypoints[0],colors[color_level],theta,center,d))
	{
#if debug_trackDirection
		cout << "ERROR: fail to transform coordinate!" << endl;
#endif
		return false;
	}

#if debug_trackDirection
	cout << ">>>trans:\t" << (getTickCount()-t)/getTickFrequency() << endl;
#endif

	trackBox[0] = dirTemp[0];
	return true;
}



///-------------------------------\//
//|��������splitDirection		  |//
//|���ܣ����벻ͬ��ɫ��С�����	  |//
//|���룺1��ԭͼ                  |//
//|		 2���������ɫ��HSV��     |//
//|���������������ֵͼ		  |//
//\-------------------------------///
bool direction::splitDirection(const Mat& src,vector<Scalar>& colors,vector<Mat>& directions)
{
#define debug_splitDirection 1
	//**������**//
	if (!src.data)
	{
#if debug_splitDirection
		cout << ">>ERROR: the image delivered to \"splitDirection\" is empty!" << endl;
#endif
		return false;
	}
	if (colors.empty())
	{
#if debug_splitDirection
		cout << ">>ERROR: the colors delivered to \"splitDirection\" is empty!" << endl;
#endif
		return false;
	}

	int i,j,k;
	const int num=colors.size();

	//**������ֵͼ������ʼ��**//
	directions.clear();
	for (i=0; i<num; i++)
		directions.push_back(Mat::zeros(src.size(),CV_8UC1));

	//**ԭͼ�ͷ�����ֵͼָ���ʼ��**//
	uchar* pSRC = src.data;
	vector<uchar*> pDirections;
	for (i=0; i<num; i++)
	{
		uchar* pD = directions[i].data;
		pDirections.push_back(pD);
	}

	//**����ԭͼ���뷽���**//
	bool flag=false;
	double radius,t;
	for (i=0; i<src.rows; i++)	//��ѭ��
	{
		for (j=0; j<src.cols; j++)	//��ѭ��
		{
			for (k=num-1; k>=0; k--)	//�����ѭ��
			{
				t = *pSRC-colors[k][0];
				t = (t<0 ? -t:t);
				radius = (t>=150 ? 180-t:t);

				if ((radius<=18 || flag) && *(pSRC+1)>60  && *(pSRC+2)>85)	//��Ӧ��ɫ�������
				{
					*(pDirections[k])=255;
					flag = true;
				}
				pDirections[k]++;
			}
			pSRC+=3;
			flag = false;
		}
	}

#if debug_splitDirection
	//imshow("red",directions[0]);
#endif

	return true;
}



///-------------------------------\//
//|��������getCornerPoint		  |//
//|���ܣ���ø���������ֵͼ�ǵ� |//
//|���룺1������������ֵͼ	  |//
//|		 2���ǵ���ϵ��		  |//
//|		 3���������ɫ��HSV��	  |//
//|��������������5���Ľǵ�	  |//
//\-------------------------------///
bool direction::getCornerPoint(Mat& src,vector<Rect>& rect,\
                                vector<vector<Point2f>>& keypoints,double c)
{
#define debug_getCornerPoint_cout 0
#define debug_getCornerPoint_show 1
	//**������**//
	if (!src.data)
	{
#if debug_getCornerPoint_cout
		cout << ">>ERROR: the image delivered to \"getCornerPoint\" is empty!" << endl;
#endif
		return false;
	}
	int i,j,k;

	//**�����˲�**//
	morphologyEx(src,src,MORPH_CLOSE,kernel);	//��̬ѧ�˲���������

	//**�ų���ͨ��**//
	Mat temp = src.clone();
	src.setTo(0);
	vector<Point> contours_poly;
	vector<vector<Point> > contours,poly;
	findContours(temp,contours,RETR_EXTERNAL,CHAIN_APPROX_SIMPLE);

	int button,left,top,right,add_size;
	for (i=0; i<contours.size(); i++)
	{
		double area = contourArea(Mat(contours[i])), accuracy=0.001143*fabs(area)+0.42857;
		if (fabs(area)>625 && fabs(area)<40000)
		{
			//�ؽ�������ͨ����
			approxPolyDP(Mat(contours[i]),contours_poly,accuracy,true);
			poly.push_back(contours_poly);
			contours_poly.clear();

			//��չ���򲢼�¼
			Rect rTemp = boundingRect(Mat(contours[i]));
			add_size = (int)(0.000127*area+4.92);
			button = rTemp.y>add_size ? rTemp.y-add_size:0;
			left = rTemp.x>add_size ? rTemp.x-add_size:0;
			top = rTemp.y+rTemp.height+add_size<src.rows ? rTemp.y+rTemp.height+add_size:src.rows;
			right = rTemp.x+rTemp.width+add_size<src.cols ? rTemp.x+rTemp.width+add_size:src.cols;
			rTemp = Rect(Point(left,button),Point(right,top));
			rect.push_back(rTemp);
		}
	}
	fillPoly(src,poly,Scalar(255));
#if debug_getCornerPoint_show
	//imshow("gray",src);
#endif
	// **�ҳ����ʵķ�����ѡ��** //
	temp.setTo(0);
	vector<Point2f> keypoint;
	for (i=0; i<poly.size(); i++)
	{
		for (j=0; j<poly[i].size(); j++)
		{
			circle(temp,poly[i][j],3,Scalar(255),-1);
			Point2f t = Point(poly[i][j].x,poly[i][j].y);
			keypoint.push_back(t);
		}

		if (keypoint.size()==4)
		{
			keypoints.push_back(keypoint);
#if debug_getCornerPoint_show
			rectangle(temp,rect[i],Scalar(127));
#endif
		}
		else
		{
			poly.erase(poly.begin()+i);
			rect.erase(rect.begin()+i);
			i--;
		}
		keypoint.clear();
	}
#if debug_getCornerPoint_show
	//imshow("corners",temp);
#endif


	if (keypoints.empty())
	{
#if debug_getCornerPoint_cout
		cout << ">>ERROR: their is no legal temp direction!" << endl;
#endif
		return false;
	}
	return true;
}



///-------------------------------\//
//|��������analizeCornerPoint	  |//
//|���ܣ�������÷���������뷽�� |//
//|���룺1�����������Ķ�ֵͼ	  |//
//|		 2��������������ɫ	  |//
//|		 2�����������Ľǵ�	  |//
//|�����1���������������		  |//
//|		 2����������귽������	  |//
//\-------------------------------///
bool direction::analizeCornerPoint(Mat& src,vector<Rect>& rect,vector<vector<Point2f> >& keypoints)
{
#define debug_analizeCornerPoint 0
	//**������**//
	if (!src.data)
	{
#if debug_analizeCornerPoint
		cout << ">>ERROR: the image delivered to \"analizeCornerPoint\" is empty!" << endl;
#endif
		return false;
	}
	int num = rect.size();
	if (num<1 || num!=keypoints.size())
	{
#if debug_analizeCornerPoint
		cout << ">>ERROR: the rect or keypoints delivered to \"analizeCornerPoint\" is empty!" << endl;
#endif
		return false;
	}
	int i,j,k,l,m;
	for (i=0; i<num; i++)
		if (keypoints[i].size()!=4)
			return false;

	//**����������ĵ�����**//i
	vector<Point2f> center;
	for (i=0; i<num; i++)
	{
		int t[4]={0,0,0,0};
		float minP[2][2] = { {10000,0},{10000,0} };
		float maxP[2][2] = { {0,0},{0,0} };
		for (j=0; j<4; j++)	//��õ��x��y���������Сֵʱ�ĵ�
		{
			if (keypoints[i][j].x>maxP[0][0])
			{
				maxP[0][1] = j;
				maxP[0][0] = keypoints[i][j].x;
			}
			if (keypoints[i][j].x<minP[0][0])
			{
				minP[0][1] = j;
				minP[0][0] = keypoints[i][j].x;
			}

			if (keypoints[i][j].y>maxP[1][0])
			{
				maxP[1][1] = j;
				maxP[1][0] = keypoints[i][j].y;
			}
			if (keypoints[i][j].y<minP[1][0])
			{
				minP[1][1] = j;
				minP[1][0] = keypoints[i][j].y;
			}
		}

		//ͨ�������ò㷽����Ľǵ��x��y���������Сֵ��ȷ�����ĵ�
		t[(int)minP[0][1]]++;
		t[(int)minP[1][1]]++;
		t[(int)maxP[0][1]]++;
		t[(int)maxP[1][1]]++;

		k = center.size();
		for (j=0; j<4; j++)
			if (!t[j])
				center.push_back(keypoints[i][j]);
		if (k==center.size())
		{
			rect.erase(rect.begin()+i);
			keypoints.erase(keypoints.begin()+i);
			num--;
			i--;
		}
	}

	//**���������Ŀռ�ֲ����ж����Ƿ�Ϸ��Լ�ȷ���䷽��**//
	uchar* pTail;
	int nonZeroPoints;
	int t[4]={0,0,0,0};
	int buf,neibor=3;
	vector<Rect> rect_temp;
	vector<vector<Point2f> > keypoints_temp;
	for (i=0; i<num; i++)
	{
		for (j=0; j<3; j++)
		{
			//��ȡÿ�����������ĵ���ɵ��������������겢�����Ƿ�Ϊ���������
			for (k=j+1; k<4; k++)
			{
				if (keypoints[i][j]!=center[i] && keypoints[i][k]!=center[i])
				{
					nonZeroPoints = 0;
					Point2f centert = (keypoints[i][j]+keypoints[i][k]+center[i])*0.3333;

					//�ڴ�СΪ9*9�����������ڷ���
					for (l=-neibor; l<=neibor; l++)
					{
						buf = centert.y+l;
						if (buf<0 || buf>=src.rows)	break;
						pTail = src.data + (int)(centert.x-neibor) + (int)(centert.y+l)*src.cols;
						for (m=-neibor; m<=neibor; m++)
						{
							buf = centert.x+m;
							if (buf<0 || buf>=src.cols)	break;
							if (*pTail > 128)
								nonZeroPoints++;
							pTail++;
						}
					}
					//���������������ȷ��Ϊ�����������������Ǽ���
					if (nonZeroPoints>=(neibor*2+1)*neibor*2)
					{
						t[j]++;
						t[k]++;
					}
				}
			}
		}

		//**�����ǵ��־��ȷ������¼�䷽��**//
		bool isRealDir = true;
		vector<Point2f> kptemp;
		for (j=0; j<4; j++)
		{
			if (t[j]==2)	//��������εĽǵ������ĵ㹹�ɷ����ķ�������
			{
				for (k=0; k<4; k++)
				{
					if (k!=j && t[k]!=1 && t[k]!=0)
					{
						isRealDir=false;
						break;
					}
				}
				if (isRealDir)
				{
					//���ĵ��±�Ϊ0������ǵ��±�Ϊ1
					kptemp.push_back(center[i]);
					kptemp.push_back(keypoints[i][j]);
					for (k=0; k<4; k++)
						if (k!=j && keypoints[i][k]!=center[i])
							kptemp.push_back(keypoints[i][k]);
					keypoints_temp.push_back(kptemp);
					rect_temp.push_back(rect[i]);
					kptemp.clear();
				}
			}
		}
	}
	if (rect_temp.empty() || keypoints_temp.empty())
	{
#if debug_analizeCornerPoint
		cout << "ERROR: the rect_temp or keypoints_temp is empty!" << endl;
#endif
		return false;
	}

	rect.clear();
	keypoints.clear();
	rect = rect_temp;
	keypoints = keypoints_temp;
	if (rect.size()!=keypoints.size())
	{
#if debug_analizeCornerPoint
		cout << ">>ERROR: the rect or keypoints measured is illegal!" << endl;
#endif
		return false;
	}

	return true;
}



///-------------------------------\//
//|��������transformCoordinate	  |//
//|���ܣ��ռ�����ת��			  |//
//|���룺1����������������	  |//
//|		 2�����������			  |//
//|		 3����������ɫ		  |//
//|		 4�����������нǵ�����  |//
//|		 5������ͷ����ֱ����н�  |//
//|�����1��ת�������������	  |//
//|		 2��ת����ķ��������	  |//
//\-------------------------------///
bool direction::transformCoordinate(vector<Point2f>& keypoints,Scalar& colors,double theta,Point2f& center,Mat& d)
{
#define debug_transformCoordinate_cout 1
#define debug_transformCoordinate_show 0
	//**������**//
	if (keypoints.size()!=4 /*|| dirVector.empty()*/)
		return false;
	if (theta<0 || theta>90)
		return false;

	//**ԭ�����Ļ�**//
	center.x = keypoints[0].x - (float)WIDTH/2;
	center.y = (float)HEIGHT/2 - keypoints[0].y;

	//**����ǵ����ƽ��ӳ������**//
	vector<Point2f> kpTemp;
	float rumda;
	for (int i=0; i<4; i++)
	{
		//���Ļ�
		keypoints[i].y = (float)HEIGHT/2 - keypoints[i].y;
		keypoints[i].x -= (float)WIDTH/2;
		Point2f t;
		rumda = atan2(2*fabs(keypoints[i].y)*tan((double)angle_y*pi/360),(double)HEIGHT);
		
		if (keypoints[i].y<0)
		{
			t.y = keypoints[i].y * sin( (float)pi/2 - rumda ) / sin( (float)pi/2 + rumda - theta*pi/180 );
			t.x = keypoints[i].y * sin( theta*pi/180 ) / sin( (float)pi/2 + rumda - theta*pi/180 );
		}
		else
		{
			t.y = keypoints[i].y * sin( (float)pi/2 + rumda ) / sin( (float)pi/2 - rumda - theta*pi/180 );
			t.x = keypoints[i].y * sin( theta*pi/180 ) / sin( (float)pi/2 - rumda - theta*pi/180 );
		}
		t.x = keypoints[i].x + t.x * 2*keypoints[i].x*tan( (float)angle_x*pi/360 )/WIDTH;

		kpTemp.push_back(t);
	}


	//**��������ͷ���־����ʵ�߶Ȳ�**//
	Point2f c = (kpTemp[2]+kpTemp[3])*0.5;
	float dia;
	dia = sqrt( (kpTemp[2].x-kpTemp[3].x)*(kpTemp[2].x-kpTemp[3].x) + (kpTemp[2].y-kpTemp[3].y)*(kpTemp[2].y-kpTemp[3].y) );
	dia = dia*0.5 + 
		sqrt( (kpTemp[1].x-kpTemp[0].x)*(kpTemp[1].x-kpTemp[0].x) + (kpTemp[1].y-kpTemp[0].y)*(kpTemp[1].y-kpTemp[0].y) )*0.5774 + 
		sqrt( (kpTemp[1].x-c.x)*(kpTemp[1].x-c.x) + (kpTemp[1].y-c.y)*(kpTemp[1].y-c.y) )*0.2887;

	float realDia;	//��ʵ���緽���ĳߴ�
	if (colors==this->colors[0])
		realDia = size_r;
	else if (colors==this->colors[1])
		realDia = size_g;
	else if (colors==this->colors[2])
		realDia = size_b;
	else
		return false;
	//��ʵ���緽���������ͷ�ĸ߶Ȳ�
	float realHeight = ( (float)HEIGHT/tan((float)angle_y*pi/360) + (float)WIDTH/tan((float)angle_x*pi/360) )  * cos(theta*pi/180)*realDia/dia/4;
	//realHeight *= 0.0862*log(dia) + 0.4759;

#if debug_transformCoordinate_show
	int H = dia*2, W = dia*2;
	Mat remapDirection = Mat::zeros(H,W,CV_8UC1);
	for (int j=0; j<kpTemp.size(); j++)
		circle(remapDirection,Point2f(kpTemp[j].x-kpTemp[0].x+W/2,H/2-kpTemp[j].y+kpTemp[0].y),3,Scalar(255),-1);
//	imshow("remap",remapDirection);
#endif
#if debug_transformCoordinate_cout
	//cout << endl << ">>center��" << center << "\tdirVector��" << d << "\tdia��" << dia << endl;
#endif
	
	//**�������Ŀռ�����ת��**//
	rumda = atan2(2*center.y*tan((double)angle_y*pi/360),(double)HEIGHT);
	center.y = realHeight * tan( theta*pi/180 + rumda );
	center.x = realHeight * (2*center.x*tan((float)angle_x*pi/360)/WIDTH) / cos( theta*pi/180 + rumda );
	for (int i=0; i<4; i++)
	{
		rumda = atan2(2*keypoints[i].y*tan((double)angle_y*pi/360),(double)HEIGHT);
		keypoints[i].y = realHeight * tan( theta*pi/180 + rumda );
		keypoints[i].x = realHeight * (2*keypoints[i].x*tan((float)angle_x*pi/360)/WIDTH) / cos( theta*pi/180 + rumda );
	}
	{
		c = (keypoints[2]+keypoints[3])*0.5;
		center = center*0.5 + (keypoints[1]+c)*0.25;

		d = Mat::zeros(1,2,CV_32FC1);
		float dist_x,dist_y;
		dist_x = keypoints[1].x*2 - center.x - c.x;
		dist_y = keypoints[1].y*2 - center.y - c.y;
		float len = sqrt( dist_x*dist_x + dist_y*dist_y );

		d.at<float>(0,0) = dist_x/len;
		d.at<float>(0,1) = dist_y/len;
	}
/*
	if (real_center_history.size()>=10)
		real_center_history.erase(real_center_history.begin());
	if (relative_height_history.size()>=10)
		relative_height_history.erase(relative_height_history.begin());
	real_center_history.push_back(center);
	relative_height_history.push_back(realHeight);*/
#if debug_transformCoordinate_cout
	cout << ">>realHeight��" << realHeight << "\tcenter��" << center << "\tdirVector��" << d << endl;

	//���뵽data.txt
	ofstream outfile;
	outfile.open("data.txt",ios::app);
	outfile<<realHeight<<" "<<center.x<<" "<<center.y<<endl;
	outfile.close();




#endif
	return true;
}




bool direction::ensureDirection(Point2f& center,double theta)
{
	if (!real_center_history.empty())
	{
		Point2f hc(0,0);
		float quan=0;
		for (int i=0; i<real_center_history.size(); i++)
		{
			hc += real_center_history[i]*(i+1);
			quan += i+1;
		}
		hc *= 1.0/quan;

		float dist_x,dist_y;
		dist_x = center.x - hc.x;
		dist_y = center.y - hc.y;

		if (dist_x*dist_x + dist_y*dist_y > 400)
			return false;
	}

	if (real_center_history.size()>=10)
		real_center_history.erase(real_center_history.begin());
	real_center_history.push_back(center);
	return true;
}

int direction::rows()	 
{  
    fstream fin("data.txt",ios::in);  
        if(!fin)  
        {  
            cerr<<"can not open file"<<endl;  
            return -1;  
        }  
      
      
    char c;  
    int lineCnt=0;  
    while(fin.get(c))  
    {     
        if(c=='\n')  
        lineCnt++;  
    }  
    cout<<lineCnt<<endl;  
    fin.close();  
    return lineCnt;  
}  
