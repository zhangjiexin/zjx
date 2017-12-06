
#include "direction.h"
#include  "uart.h"
#include <unistd.h>
#include <pthread.h>

void send_takeoff( const int fd , const double height )
{
	unsigned char buffer[10];
	unsigned char CK_A = 0 , CK_B = 0;
	buffer[0] = 'A';	buffer[1] = 'C';
	
	int buffer_index = 2;
	#define write_new_byte(buffer,i,byte) buffer[i++]=byte;CK_A+=byte; CK_B+=CK_A
	write_new_byte( buffer , buffer_index , 'g' );
	write_new_byte( buffer , buffer_index , 2 );
	int height_int = (int)( height * 10 );
	union conv
	{
		int i;
		unsigned char b[4];
	};
	conv* height_conv = (conv*)&height_int;
	write_new_byte( buffer , buffer_index , height_conv->b[0] );
	write_new_byte( buffer , buffer_index , height_conv->b[1] );
	write_new_byte( buffer , buffer_index , height_conv->b[2] );
	write_new_byte( buffer , buffer_index , height_conv->b[3] );
	
	buffer[ buffer_index++ ] = CK_A;
	buffer[ buffer_index++ ] = CK_B;
	
	write(fd,buffer,buffer_index);
}

void send_route_line( const int fd , const double x , const double y , const double z )
{
	unsigned char buffer[18];
	unsigned char CK_A = 0 , CK_B = 0;
	buffer[0] = 'A';	buffer[1] = 'C';
	
	int buffer_index = 2;
	#define write_new_byte(buffer,i,byte) buffer[i++]=byte;CK_A+=byte; CK_B+=CK_A
	write_new_byte( buffer , buffer_index , 'g' );
	write_new_byte( buffer , buffer_index , 10 );
	int x_int = (int)( x * 10 );
	int y_int = (int)( y * 10 );
	int z_int = (int)( z * 10 );
	union conv
	{
		int i;
		unsigned char b[4];
	};
	conv* conv_xyz = (conv*)&x_int;
	write_new_byte( buffer , buffer_index , conv_xyz->b[0] );
	write_new_byte( buffer , buffer_index , conv_xyz->b[1] );
	write_new_byte( buffer , buffer_index , conv_xyz->b[2] );
	write_new_byte( buffer , buffer_index , conv_xyz->b[3] );
	conv_xyz = (conv*)&y_int;
	write_new_byte( buffer , buffer_index , conv_xyz->b[0] );
	write_new_byte( buffer , buffer_index , conv_xyz->b[1] );
	write_new_byte( buffer , buffer_index , conv_xyz->b[2] );
	write_new_byte( buffer , buffer_index , conv_xyz->b[3] );
	conv_xyz = (conv*)&z_int;
	write_new_byte( buffer , buffer_index , conv_xyz->b[0] );
	write_new_byte( buffer , buffer_index , conv_xyz->b[1] );
	write_new_byte( buffer , buffer_index , conv_xyz->b[2] );
	write_new_byte( buffer , buffer_index , conv_xyz->b[3] );
	
	buffer[ buffer_index++ ] = CK_A;
	buffer[ buffer_index++ ] = CK_B;
	
	write(fd,buffer,buffer_index);
}

void send_land( const int fd )
{
	unsigned char buffer[6];
	unsigned char CK_A = 0 , CK_B = 0;
	buffer[0] = 'A';	buffer[1] = 'C';
	
	int buffer_index = 2;
	#define write_new_byte(buffer,i,byte) buffer[i++]=byte;CK_A+=byte; CK_B+=CK_A
	write_new_byte( buffer , buffer_index , 'g' );
	write_new_byte( buffer , buffer_index , 3 );
	
	buffer[ buffer_index++ ] = CK_A;
	buffer[ buffer_index++ ] = CK_B;
	
	write(fd,buffer,buffer_index);
	for(int i=0;i<buffer_index;i++)
	printf("buffer%d\n",buffer[i]);
}

int current_use_frame = 0;
#define frame_count 4
Mat frame[frame_count];
int frame_index = 0;
VideoCapture* capture;
bool find_result[ frame_count ];
void* image_getting_thread(void* arg)
{
	Mat temp_frame;
	while(1)
	{
		(*capture) >> temp_frame;
		
		int next_frame_index;	
					
		next_frame_index = frame_index + 1;
		re_find_next_frame_index:	
		if( next_frame_index == frame_count	)
			next_frame_index = 0;
		if( next_frame_index == current_use_frame )
		{
			++next_frame_index;
			goto re_find_next_frame_index;
		}
			
		frame[ next_frame_index ].release();
		frame[ next_frame_index ] = temp_frame.clone();
		frame_index = next_frame_index;
	}
}

void* find_thread(void* arg)
{
	
	

}


int main()
{
	/*Mat mapx,mapy;
	FileStorage file("map.xml",FileStorage::READ);
	file["mapx"] >> mapx;
	file["mapy"] >> mapy;*/
	capture = new VideoCapture(0);
	if (!capture->isOpened())
	return -1;
	
	(*capture) >> frame[frame_index];
	(*capture) >> frame[frame_index];
	int cap_index = 0;
	char fname[256];
	
	capture->set( 3 , 640 );
	capture->set( 4 , 480 );
	
	(*capture) >> frame[frame_index];
	
	pthread_t image_create_thread_pointer;
	pthread_create( &image_create_thread_pointer , NULL , image_getting_thread , NULL );	
	
	/*
	while(1)
	{
		waitKey(10);

		int current_frame_index = frame_index;		
		image_read_completed = false;		
		imshow("ddd",frame[current_frame_index]);
		
		
		++cap_index;
		if( cap_index < 500 )
		{
			sprintf( fname , "images/image_%u.bmp" , cap_index );
			imwrite( fname , frame[frame_index] );
		}
		
	}
	*/
	
	
	int fd=open_port();
    int unuse=set_opt(fd,57600,8,'N',1);
    cout<<"take off"<<endl;
	sleep(40);
	double height_1 = 500.0;
	send_takeoff(fd,height_1);
	cout<<"rout_line-->x=500"<<endl;
	sleep(20);
	double x_1=500.0,y_1=0,z_1=0;
	send_route_line(fd,x_1,y_1,z_1);
	cout<<"rout_line-->z=-200"<<endl;
	sleep(5);
	x_1=0;y_1=0;z_1=-250.0;
	send_route_line(fd,x_1,y_1,z_1);
	
	cout<<"image processing"<<endl;	
	
	

	Mat temp;
	direction detectDirection;
	Point2f center;
	Mat dirVector;
	int row = 0;
	double realHeight,center_x,center_y;
	while (1)
	{
	
		current_use_frame = frame_index;
		int current_frame_index = current_use_frame;
		
		//imshow("camera",frame[current_frame_index]);
		//waitKey(1);
		
		if (!frame[current_frame_index].data)	break;
		
		//remap(frame,frame,mapx,mapy,INTER_LINEAR);

		cout<<row<<endl;
		if(row>=10)break;
		
		++cap_index;
		if( cap_index < 500 )
		{
			sprintf( fname , "images/image_%u.bmp" , cap_index );
			imwrite( fname , frame[current_frame_index] );
		}
		 
		if (detectDirection.trackBox.empty())
		{
			row = 0;
			center_x = center_y = 0;
			if (detectDirection.findDirection(frame[current_frame_index],0,center,dirVector))
			{
				cout << ">>----------寻找成功----------<<" << endl;
			}
			else
				cout << ">>----------寻找失败----------<<" << endl;
		}
		else
		{
			if (!detectDirection.trackDirection(frame[current_frame_index],0,center,dirVector) || !detectDirection.ensureDirection(center,0))
			{
				detectDirection.trackBox.clear();
				row = 0;
				center_x = center_y = 0;
			}
			else
			{
				cout<<">>----------跟踪成功----------<<" << endl;
				center_x += center.x;
				center_y += center.y;
				
				++row;
			}
		}
		
	 }
/*	//开始计算realHeight,center.x,center.y的平均值
	ifstream fin("data.txt");
	if(!fin)
        {  
            cerr<<"can not open file"<<endl;  
            return -1;  
        }  

	int l=0,z,x,y;
	while(fin>>a1[l]>>a2[l]>>a3[l]) { l++;}
	fin.close();


	//清空txt文件
    FILE *fp=NULL;
	fp=fopen("data.txt","w");fclose(fp);
	

	for(;l>0;l--)
       {
	realHeight+=a1[l-1];
	center_x  +=a2[l-1];
	center_y  +=a3[l-1];
       }
 */
	realHeight=realHeight/5.0;
	center_x=center_x/5.0;
	center_y=center_y/5.0;

	printf("realHeight:->%lf",realHeight);
	cout<<"center.x , center.y:  "<<fixed<<center_x<<","<<center_y<<endl; 
	cout<<"rout_line-->center_x,center_y"<<center_x<<","<<center_y<<endl;
		
	send_route_line(fd,center_x,center_y,0);
	sleep(2);
	
	cout<<"send_land"<<endl;
	send_land(fd);
		
	
	return 0;
}
