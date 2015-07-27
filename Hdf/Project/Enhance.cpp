#include "Enhance.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include "CLAHE.h"
using namespace std;
void Enhance::Nearest_interpolation(ushort *pSrc,int width,int height)
{

}
void Enhance::Bilinear_interpolation(ushort *pSrc,ushort* dest,int width,int height)
{
	//temp指向与pSrc相同大小内存，已经分配
	if(pSrc==NULL||dest==NULL||width*height==0)
		return ;
	ushort filledVal=0;
	//水平垂直分别进行插值处理
	ushort *ptempH=dest;
	ushort *ptempV=pSrc;
	memcpy(dest,pSrc,width*height*sizeof(ushort));

	//Horizontal_Linear_fill(dest,width,height);
	//Vertical_Linear_fill(pSrc,width,height);
	//下面增加了对垂直方向的补充
	Horizontal_Linear_fill(dest,width,height);
	Vertical_Linear_fill(pSrc,width,height);
	
	ushort valH=filledVal;
	ushort valV=filledVal;
	for(int y=0;y<height;++y )
		for(int x=0;x<width;++x)
		{
			valH=*ptempH;
			valV=*ptempV;
			if(valV==filledVal&&valH!=filledVal)//是否保留，造成多出边缘？
				*dest=valH;
			else if(valV!=filledVal&&valH==filledVal)//是否保留，多出边缘？
				*dest=valV;
			else if(valV!=filledVal&&valH!=filledVal)
				*dest=ushort((valH+valV)/2.0f+0.5f);
			++dest;
			++ptempH;
			++ptempV;
		}
}
void Enhance::Bilinear_interpolation2(ushort *pSrc,ushort* dest,int width,int height)
{
	//temp指向与pSrc相同大小内存，已经分配
	if(pSrc==NULL||dest==NULL||width*height==0)
		return ;
	ushort filledVal=0;
	//水平垂直分别进行插值处理
	ushort *ptempH=dest;
	ushort *ptempV=pSrc;
	ushort *pDest=dest;
	memcpy(dest,pSrc,width*height*sizeof(ushort));

	//Horizontal_Linear_fill(dest,width,height);
	//Vertical_Linear_fill(pSrc,width,height);
	//下面增加了对垂直方向的补充
	Horizontal_Linear_fill2(dest,width,height);
	Vertical_Linear_fill2(pSrc,width,height);

	ushort valH=filledVal;
	ushort valV=filledVal;
	for(int y=0;y<height;++y )
		for(int x=0;x<width;++x)
		{
			valH=*ptempH;
			valV=*ptempV;
		//缺少一个纬度的信息不填充，使用其他方法填充剩余的空白
			/*	if(valV!=filledVal&&valH!=filledVal)
			*pDest=ushort((valH+valV)/2.0f+0.5f);*/
			if(valV!=filledVal&&valH!=filledVal)
					*pDest=ushort((valH*(float)width/(width+height)+valV*(float)height/(width+height))+0.5f);
			++pDest;
			++ptempH;
			++ptempV;
		}
	//临近值填充，初步
	pDest=dest;
	nearest_fill_unshort(pDest, width, height,0,3, 3);
}
void Enhance::Bicubic_Interpolation(ushort *pSrc,ushort* dest,int width,int height)
{

}
bool Enhance::nearest_fill_unshort(unsigned short *pData, int width, int height, unsigned short fill_value, int nloop, int slide_window_width)
{
	cout << "Process Fill Nearest Unshort!" << endl;
	int rtn=0;
	int i = 0;
	int j = 0;
	int k = 0;
	int window_row = 0;
	int window_col = 0;
	unsigned int data_row = 0;
	unsigned int data_col = 0;
	unsigned int window_index = 0;	
	unsigned int data_index = 0;
	vector <float> coeff;
	vector <float> distance;
	vector <float> window_data;
	float distance_sum = 0.0;
	for(k=0; k<nloop; k++) 
	{
		cout << "Filling Loop " << k << " !" << endl;
		//用滑动窗口反距离加权平均值法进行插值
		// 		for(data_row = 0; data_row < height; ++data_row)
		// 		{
		// 			for(data_col = 0; data_col < width; ++data_col)
		// 			{
		// 				//计算数据下标
		// 				data_index = data_row * width + data_col;
		// 				//已有值的不做插值
		// 				if (pData[data_index] != 0)
		// 				{
		// 					continue;
		// 				}
		// 				//计算滑动窗口在数据中的起始行号
		// 				window_row = data_row - (slide_window_width/2);
		// 				int obs_row_temp = window_row;	
		// 				//双重循环，扫描该数据周围slide_window_width范围内地面数据中的每一个点
		// 				for(; window_row < obs_row_temp + slide_window_width; ++window_row)
		// 				{
		// 					
		// 					//剔除无效的行
		// 					if(window_row < 0 || window_row >= height)
		// 					{
		// 						continue;
		// 					}
		// 					//计算滑动窗口在地面数据中的起始列号
		// 					window_col = data_col - (slide_window_width/2);
		// 					int obs_col_temp = window_col;
		// 					for(; window_col < obs_col_temp + slide_window_width; ++window_col)
		// 					{					
		// 						//剔除无效的列
		// 						if(window_col < 0 || window_col >= width)
		// 						{
		// 							continue;
		// 						}
		// 						//计算窗口数据下标
		// 						window_index = window_row * width + window_col;
		// 						//剔除无效值
		// 						if(pData[window_index] == 0)
		// 						{
		// 							continue;
		// 						}
		// 						else
		// 						{	 
		// 							int temp_1= abs((int)(data_col - window_col));
		// 							int temp_2 = abs((int)(data_row - window_row));
		// 							int temp_distance = temp_1*temp_1 +temp_2*temp_2;
		// 							if(temp_distance != 0)
		// 							{
		// 								distance.push_back(1.0/temp_distance);
		// 								window_data.push_back(pData[window_index]);
		// 							}
		// 							else
		// 							{
		// 								continue;
		// 							}
		// 							
		// 						}
		// 						
		// 					}
		// 				}
		// 				if(distance.size() > 0)
		// 				{
		// 					for(i = 0; i < distance.size(); ++i)
		// 					{
		// 						distance_sum += distance[i];
		// 					}
		// 					for(i = 0; i < distance.size(); ++i)
		// 					{
		// 						coeff.push_back(distance[i]/distance_sum);
		// 					}
		// 					float IDDI_temp_zero = 0.0;
		// 					for(i = 0; i < distance.size(); ++i)
		// 					{
		// 						IDDI_temp_zero += coeff[i] * window_data[i];
		// 					}
		// 					pData[data_index] = IDDI_temp_zero;
		// 				}
		// 				//清理数据
		// 				distance_sum = 0;
		// 				distance.clear();
		// 				window_data.clear();
		// 				coeff.clear();
		// 			}
		// 		}

		//临近值插值法
		{

			// 		// Upward
			for(i=height-1; i>0; i--) {
				for(j=0; j<width; j++) {				
					if(pData[i*width+j]==fill_value && pData[(i-1)*width+j]!=fill_value) {
						pData[i*width+j] = pData[(i-1)*width+j];
					}
				}
			}

			// Rightward
			for(i=0; i<height; i++) {
				for(j=0; j<width-1; j++) {			
					if(pData[i*width+j]==fill_value && pData[i*width+j+1]!=fill_value) {
						pData[i*width+j] = pData[i*width+j+1];
					}
				}
			}

			// Downward
			for(i=0; i<height-1; i++) {
				for(j=0; j<width; j++) {			
					if(pData[i*width+j]==fill_value && pData[(i+1)*width+j]!=fill_value) {
						pData[i*width+j] = pData[(i+1)*width+j];
					}
				}
			}

			// Leftward
			for(i=0; i<height; i++) {
				for(j=width-1; j>0; j--) {			
					if(pData[i*width+j]==fill_value && pData[i*width+j-1]!=fill_value) {
						pData[i*width+j] = pData[i*width+j-1];
					}
				}
			}
		}
	}
	cout << "Success!" << endl;
	return true;
}
static void Bubble_sort(unsigned short *a,int n)
{
		int i, j, temp;
		for (j = 0; j < n - 1; j++)
			for (i = 0; i < n - 1 - j; i++)
			{
				if(a[i] > a[i + 1])
				{
					temp = a[i];
					a[i] = a[i + 1];
					a[i + 1] = temp;
				}
			}
	
}
void Enhance::Median_Filter(unsigned short *src,int width,int height)
{
	ushort *temp=new ushort[width*height];
	ushort* des=temp;
	//算法不合适使用Median_Filter2
	memcpy(des,src,width*height*sizeof(ushort));
	int tempW=3;//模板宽度
	int tempH=3;//模板高度
	int offx=width-tempW+1;//水平移动
	int offy=height-tempH+1;//垂直移动

	ushort *tempstart=NULL;
	ushort *outstart=NULL;
	ushort *sortspace=new ushort[tempW*tempH];
	for(int y=0;y<offy;++y)
	{
		for(int x=0;x<offx;++x)
		{
			tempstart=src+y*width+x;
			outstart=des+y*width+x;
			ushort *tempspace=sortspace;
			for(int seedh=0;seedh<tempH;++seedh)
			{
				for(int i=0;i<tempW;++i)
					*tempspace++=*(tempstart+width*seedh+i);
			}
			Bubble_sort(sortspace,tempW*tempH);
			*(outstart+(tempH/2/*+1*/)*width+tempW/2/*+1*/)=sortspace[int(tempW*tempH/2)];
		}
	}
	delete []sortspace;

	memcpy(src,temp,width*height*sizeof(ushort));
	delete []temp;
}
void Enhance::Median_Filter2(unsigned short *src,int width,int height)
{
	ushort *temp=new ushort[width*height];
	ushort* des=temp;
	memcpy(des,src,width*height*sizeof(ushort));
	int tempW=3;//模板宽度
	int tempH=3;//模板高度
	int offx=width-tempW+1;//水平移动
	int offy=height-tempH+1;//垂直移动
	const ushort filledVal=0;
	ushort *tempstart=NULL;
	ushort *outstart=NULL;
	ushort *sortspace=new ushort[tempW*tempH];
	for(int y=0;y<offy;++y)
	{
		for(int x=0;x<offx;++x)
		{
			tempstart=src+y*width+x;
			outstart=des+y*width+x;
			ushort *tempspace=sortspace;
			for(int seedh=0;seedh<tempH;++seedh)
			{
				for(int i=0;i<tempW;++i)
					*tempspace++=*(tempstart+width*seedh+i);
			}
			Bubble_sort(sortspace,tempW*tempH);
			int pos=tempW*tempH/2;
			ushort val=*(sortspace+pos);
			//去除填充值的影响
			if(val==filledVal&&pos<tempH*tempW)
			{
				for(int i=pos;i<tempW*tempH;++i)
				{
					val=*(sortspace+pos);
					if(val==filledVal)
					{
						pos++;
					}

				}
			}

			*(outstart+(tempH/2/*+1*/)*width+tempW/2/*+1*/)=val;
		}
	}
	delete []sortspace;
	memcpy(src,temp,width*height*sizeof(ushort));
	delete []temp;
}
void Enhance::Fill_HorizontalLine2(unsigned short *start,unsigned short*end,unsigned short filledVal)
{
		//首尾必定都有数据值
	unsigned short *pos=start;
	unsigned short startVal=*pos;
	unsigned short endVal=*pos;
	while(pos<end)
	{
	
		if(*pos!=filledVal)
		{
			++pos;
		}
		else
		{
			unsigned short *gapStart=pos;
			unsigned short *gapEnd=pos;
			while(*pos==filledVal)
				++pos;
			gapEnd=pos;
			startVal=*(gapStart-1);
			endVal=*gapEnd;
			int len=gapEnd-gapStart;
			if(len<20)//长度过长可能失真
			{
				float inc=(endVal-startVal)/(float)(len+1.0f);
				for(int i=0;i<len;++i)
				{
					int temp=startVal+(int)(inc*(i+1)+0.5f);
					/*	if(inc>0)
					temp=startVal+(int)(inc*(i+1)+0.5f);
					else
					temp=startVal+(int)(inc*(i+1)-0.5f);*/
					if(temp>65535)
						temp=65535;
					if(temp<0)
						temp=0;
					*(gapStart+i)=temp;
				}
			}
			

		}
	}
}
void Enhance::Fill_VerticalLine2(ushort *start,ushort*end,ushort width,ushort filledVal)
{
	//start,end是一列有值位置的其实,从s到e开始查找空白填充
	ushort *gapStart,*gapEnd;
	ushort startVal,endVal;
	startVal=*start;
	endVal=startVal;
	ushort *pos=start;
	while(pos<end)
	{
	
		if(*pos!=filledVal)
		{
			pos+=width;
		}
		else
		{
			gapStart=pos;
			gapEnd=pos;
			while(*pos==filledVal)
				pos+=width;
			gapEnd=pos;
			startVal=*(gapStart-width);
			endVal=*gapEnd;
			int len=(gapEnd-gapStart)/width;
			if(len<20)
			{
				float inc=(endVal-startVal)/(float)(len+1.0f);
				for(int i=0;i<len;++i)
				{
					int temp=startVal+(int)(inc*(i+1)+0.5f);
					/*	if(inc>0)
					temp=startVal+(int)(inc*(i+1)+0.5f);
					else
					temp=startVal+(int)(inc*(i+1)-0.5f);*/
					if(temp>65535)
						temp=65535;
					if(temp<0)
						temp=0;
					*(gapStart+i*width)=temp;
				}
			}
		

		}
	}
}
void Enhance::Add_VerticalHorLine2(unsigned short *src,int width,int height,unsigned short filledVal)
{
	int linepartition=5;
	for(int y=0;y<height;)
	{
		//寻找两端起点[start,end]
		unsigned short *pStart=src+width*y;
		unsigned short *pEnd=pStart+width-1;
		while(pEnd>pStart)
		{
			if(*pStart==filledVal)
				++pStart;
			if(*pEnd==filledVal)
				--pEnd;
			if(*pStart!=filledVal&&*pEnd!=filledVal)
				break;
		}
		if(pEnd<=pStart)
		{
			y+=linepartition;
			continue;
		}
		pEnd+=1;//线性填充一行(start,end)
		//
		if(pEnd-pStart<20)//50个像素的长度
			Fill_HorizontalLine(pStart,pEnd,filledVal);
		y+=linepartition;
	}
}
void Enhance::Add_HorizontalVerLine2(ushort *src,int width,int height,unsigned short filledVal)
{
	int linepartition=5;
	int step=width;
	for(int y=0;y<width;)
	{
		//寻找两端起点[start,end]
		unsigned short *pStart=src+y;
		unsigned short *pEnd=pStart+width*(height-1);
		while(pEnd>pStart)
		{
			if(*pStart==filledVal)
				pStart+=step;
			if(*pEnd==filledVal)
				pEnd-=step;
			if(*pStart!=filledVal&&*pEnd!=filledVal)
				break;
		}
		if(pEnd<=pStart)
		{
			y+=linepartition;
			continue;
		}
		//线性填充一列[start,end],
		pEnd+=step;
		//如果长度过长说明填充会失真放弃该行
		if(pEnd-pStart<20*step)//50个像素的长度
			Fill_VerticalLine(pStart,pEnd,width,filledVal);
		y+=linepartition;
	}
}
void Enhance::Fill_HorizontalLine(unsigned short *start,unsigned short*end,unsigned short filledVal)
{
	//首尾必定都有数据值
	unsigned short *pos=start;
	unsigned short startVal=*pos;
	unsigned short endVal=*pos;
	while(pos<end)
	{
	
		if(*pos!=filledVal)
		{
			++pos;
		}
		else
		{
			unsigned short *gapStart=pos;
			unsigned short *gapEnd=pos;
			while(*pos==filledVal)
				++pos;
			gapEnd=pos;
			startVal=*(gapStart-1);
			endVal=*gapEnd;
			int len=gapEnd-gapStart;
			float inc=(endVal-startVal)/(float)(len+1.0f);
			for(int i=0;i<len;++i)
			{
				int temp=startVal+(int)(inc*(i+1)+0.5f);
				/*	if(inc>0)
				temp=startVal+(int)(inc*(i+1)+0.5f);
				else
				temp=startVal+(int)(inc*(i+1)-0.5f);*/
				if(temp>65535)
					temp=65535;
				if(temp<0)
					temp=0;
				*(gapStart+i)=temp;
			}

		}
	}
}
void Enhance::Fill_VerticalLine(ushort *start,ushort*end,ushort width,ushort filledVal)
{
	//start,end是一列有值位置的其实,从s到e开始查找空白填充
	ushort *gapStart,*gapEnd;
	ushort startVal,endVal;
	startVal=*start;
	endVal=startVal;
	ushort *pos=start;
	while(pos<end)
	{
	
		if(*pos!=filledVal)
		{
			pos+=width;
		}
		else
		{
			gapStart=pos;
			gapEnd=pos;
			while(*pos==filledVal)
				pos+=width;
			gapEnd=pos;
			startVal=*(gapStart-width);
			endVal=*gapEnd;
			int len=(gapEnd-gapStart)/width;
			float inc=(endVal-startVal)/(float)(len+1.0f);
			for(int i=0;i<len;++i)
			{
				int temp=startVal+(int)(inc*(i+1)+0.5f);
				/*	if(inc>0)
				temp=startVal+(int)(inc*(i+1)+0.5f);
				else
				temp=startVal+(int)(inc*(i+1)-0.5f);*/
				if(temp>65535)
					temp=65535;
				if(temp<0)
					temp=0;
				*(gapStart+i*width)=temp;
			}

		}
	}
}
void Enhance::Horizontal_Linear_fill2(unsigned short *src,int width,int height)
{
	unsigned short filledVal=0;
	//纬度拉伸，需要依靠经度数据作为基准补充经度信息，所以纬度填充前先隔n行填充经度再填充纬度
	Add_HorizontalVerLine2(src,width,height,filledVal);
	//暂时在原图上填充，不使用des
	//memcpy(des,src,width*height*sizeof(unsigned short));
	for(int y=0;y<height;++y)
	{
		//寻找两端起点[start,end]
		unsigned short *pStart=src+width*y;
		unsigned short *pEnd=pStart+width-1;
		while(pEnd>pStart)
		{
			if(*pStart==filledVal)
				++pStart;
			if(*pEnd==filledVal)
				--pEnd;
			if(*pStart!=filledVal&&*pEnd!=filledVal)
				break;
		}
		if(pEnd<=pStart)
			continue;
		pEnd+=1;//线性填充一行[start,end),不包含end指向
		
		Fill_HorizontalLine2(pStart,pEnd,filledVal);
	}
}
void Enhance::Horizontal_Linear_fill(unsigned short *src,int width,int height)
{
	unsigned short filledVal=0;
	//纬度拉伸，需要依靠经度数据作为基准补充经度信息，所以纬度填充前先隔n行填充经度再填充纬度
	Add_HorizontalVerLine(src,width,height,filledVal);
	//暂时在原图上填充，不使用des
	//memcpy(des,src,width*height*sizeof(unsigned short));
	for(int y=0;y<height;++y)
	{
		//寻找两端起点[start,end]
		unsigned short *pStart=src+width*y;
		unsigned short *pEnd=pStart+width-1;
		while(pEnd>pStart)
		{
			if(*pStart==filledVal)
				++pStart;
			if(*pEnd==filledVal)
				--pEnd;
			if(*pStart!=filledVal&&*pEnd!=filledVal)
				break;
		}
		if(pEnd<=pStart)
			continue;
		pEnd+=1;//线性填充一行[start,end),不包含end指向

		Fill_HorizontalLine(pStart,pEnd,filledVal);
	}
}
void  Enhance::Add_HorizontalVerLine(ushort *src,int width,int height,unsigned short filledVal)
{
	int linepartition=5;
	int step=width;
	for(int y=0;y<width;)
	{
		//寻找两端起点[start,end]
		unsigned short *pStart=src+y;
		unsigned short *pEnd=pStart+width*(height-1);
		while(pEnd>pStart)
		{
			if(*pStart==filledVal)
				pStart+=step;
			if(*pEnd==filledVal)
				pEnd-=step;
			if(*pStart!=filledVal&&*pEnd!=filledVal)
				break;
		}
		if(pEnd<=pStart)
		{
			y+=linepartition;
			continue;
		}
		//线性填充一列[start,end],
		pEnd+=step;
		//如果长度过长说明填充会失真放弃该行
		if(pEnd-pStart<50*step)//50个像素的长度
			Fill_VerticalLine(pStart,pEnd,width,filledVal);
		y+=linepartition;
	}
}
void Enhance::Add_VerticalHorLine(unsigned short *src,int width,int height,unsigned short filledVal)
{
	int linepartition=5;
	for(int y=0;y<height;)
	{
		//寻找两端起点[start,end]
		unsigned short *pStart=src+width*y;
		unsigned short *pEnd=pStart+width-1;
		while(pEnd>pStart)
		{
			if(*pStart==filledVal)
				++pStart;
			if(*pEnd==filledVal)
				--pEnd;
			if(*pStart!=filledVal&&*pEnd!=filledVal)
				break;
		}
		if(pEnd<=pStart)
		{
			y+=linepartition;
			continue;
		}
		pEnd+=1;//线性填充一行(start,end)
		//
		//if(pEnd-pStart<400)//50个像素的长度
			Fill_HorizontalLine(pStart,pEnd,filledVal);
		y+=linepartition;
	}
}
void Enhance::Vertical_Linear_fill2(ushort *src,int width,int height)
{
	//经度拉伸，需要依靠纬度数据作为基准补充经度信息，所以经度填充前先隔n行填充纬度再填充经度
	ushort filledVal=0;
	Add_VerticalHorLine2(src,width,height,filledVal);
	int step=width;
	for(int y=0;y<width;++y)
	{
		//寻找两端起点[start,end]
		unsigned short *pStart=src+y;
		unsigned short *pEnd=pStart+width*(height-1);
		while(pEnd>pStart)
		{
			if(*pStart==filledVal)
				pStart+=step;
			if(*pEnd==filledVal)
				pEnd-=step;
			if(*pStart!=filledVal&&*pEnd!=filledVal)
				break;
		}
		if(pEnd<=pStart)
			continue;
		//线性填充一列[start,end],
		pEnd+=step;
		
		Fill_VerticalLine2(pStart,pEnd,width,filledVal);
	}
	
}
void Enhance::Vertical_Linear_fill(unsigned short *src,int width,int height)
{
	//经度拉伸，需要依靠纬度数据作为基准补充经度信息，所以经度填充前先隔n行填充纬度再填充经度
	ushort filledVal=0;
	Add_VerticalHorLine(src,width,height,filledVal);
	int step=width;
	for(int y=0;y<width;++y)
	{
		//寻找两端起点[start,end]
		unsigned short *pStart=src+y;
		unsigned short *pEnd=pStart+width*(height-1);
		while(pEnd>pStart)
		{
			if(*pStart==filledVal)
				pStart+=step;
			if(*pEnd==filledVal)
				pEnd-=step;
			if(*pStart!=filledVal&&*pEnd!=filledVal)
				break;
		}
		if(pEnd<=pStart)
			continue;
		//线性填充一列[start,end],
		pEnd+=step;

		Fill_VerticalLine(pStart,pEnd,width,filledVal);
	}

}
void Enhance::Convolution(ushort *src,ushort *des,int width,int height,ushort tplW,ushort tplH,ushort tplMX,ushort tplMY,int *tplArr,float fac)
{
	////卷积运算
	float fResult=0.0f;
	ushort *p=src;
	ushort *ndata=des;
	memcpy(ndata,src,width*height*sizeof(ushort));
	for (int a=tplMY;a<height-tplH+tplMX+1;a++)
	{
		for (int b=tplMX;b<width-tplW+tplMX+1;b++)
		{
			//a,b模板中心位置
			int pixel_start=(a-tplMY)*width+(b-tplMX);
			fResult = 0;
			for(int k=0;k<tplH;k++){
				for(int l=0;l<tplW;l++){
					fResult+=p[pixel_start+k*width+l]*tplArr[k*tplW+l];
				}
			}
			fResult *= fac;
			if(fResult<0.0f)
				fResult=0.00001f;
			if(fResult>4094.99f)
				fResult=4094.99f;
			ndata[a*width+b]=ushort(fResult+0.5f);
		}
	}
}
 void Enhance::Convolution(ushort *src,int width,int height,ushort tplW,ushort tplH,ushort tplMX,ushort tplMY,int *tplArr,float fac)
 {
	 ////卷积运算
	 float fResult=0.0f;
	 ushort *p=src;
	 ushort *ndata=new ushort[width*height];
	 memcpy(ndata,src,width*height*sizeof(ushort));
	 for (int a=tplMY;a<height-tplH+tplMX+1;a++)
	 {
		 for (int b=tplMX;b<width-tplW+tplMX+1;b++)
		 {
				//a,b模板中心位置
				int pixel_start=(a-tplMY)*width+(b-tplMX);
				fResult = 0;
				for(int k=0;k<tplH;k++){
					for(int l=0;l<tplW;l++){
									fResult+=p[pixel_start+k*width+l]*tplArr[k*tplW+l];
								}
							}
				fResult *= fac;
				ndata[a*width+b]=ushort(fResult+0.5f);
		}
	 }
	 memcpy(src,ndata,width*height*sizeof(ushort));
	 delete []ndata;

 }
 void Enhance::Laplace_Sharp(ushort *src,ushort* dest,int width,int height)
 {
	 memcpy(dest,src,sizeof(ushort)*width*height);
	 int arr[9]=  {
		-1,-1,-1,
		 -1,9,-1,
		 -1,-1,-1
	 };
	 float factor=1.0f;
	 Convolution(src,dest,width,height,3,3,1,1,arr,factor);
 }
 void Enhance::Gauss_Smooth(ushort *src,ushort* dest,int width,int height)
 {
	 //memcpy(dest,src,sizeof(ushort)*width*height);
	 //int arr[25]=  {
		// 0,1,2,1,0,
		// 1,2,4,2,1,
		// 2,4,8,4,2,
		// 1,2,4,2,1,
		// 0,1,2,1,0
	 //};
	 //float factor=0.02083f;
	 //Convolution(src,dest,width,height,5,5,2,2,arr,factor);
	 ///Gaussian Blur///////////////////////////////////////////////////////////////////////
	 memcpy(dest,src,sizeof(ushort)*width*height);
	 int arr[9]=  {
		 1,2,1,
		 2,4,2,
		 1,2,1
	 };
	 float factor=0.0625f;
	 Convolution(src,dest,width,height,3,3,1,1,arr,factor);
 }
  void Enhance::Gauss_Smooth(ushort *src,int width,int height)
  {
	  /* int arr[9]=  {
	  1,2,1,
	  2,4,2,
	  1,2,1
	  };*/
	  int arr[25]=  {
		  0,1,2,1,0,
		  1,2,4,2,1,
		  2,4,8,4,2,
		  1,2,4,2,1,
		  0,1,2,1,0
	  };
	  ushort templateMidX=1,templateMidY=1;
	  float factor=0.02083f;
	  Convolution(src,width,height,5,5,2,2,arr,factor);
  }
  void Enhance::ImageEnhance(ushort *pSrc,int width,int height)
  {
	  ///插值
	  ushort *dest=new ushort[width*height];
	  //生成结果保存到dest中，但是pSrc已经被修改过了(为了利用空间，避免图像过大内存不足)
	 // Enhance::Bilinear_interpolation2(pSrc,dest,width,height);
	 // memcpy(pSrc,dest,width*height*sizeof(ushort));
	  Enhance::nearest_fill_unshort(pSrc,width,height,0,4,3);
	  ///平滑去噪
	 // Enhance::Median_Filter2(dest,width,height);//太耗时
	 //  memcpy(dest,pSrc,sizeof(ushort)*width*height);
	 // Enhance::Gauss_Smooth(pSrc,dest,width,height);
	 //  Enhance::Laplace_Sharp(dest,pSrc,width,height);
	  //memcpy(pSrc,dest,sizeof(ushort)*width*height);
	  delete []dest;
  }
  void  Enhance::Histogram_Equalization(ushort *src,int width,int height)
  {
	  // 指向源图像的指针
	  //const int max_val=16383;//14位
	  const int max_val=4095;//12位
	   //const int max_val=65535;//16位
	  ushort*	lpSrc;
		 // // 临时变量
		int	lTemp;
		 // // 循环变量
		int	i;
		int	j;
		// 灰度映射表
		ushort	*bMap=new ushort[max_val+1];
		 memset(bMap,0, (max_val+1)*sizeof(ushort));
		 // // 灰度映射表
		 int	*lCount=new int[max_val+1];
		 memset(lCount,0,(max_val+1)*sizeof(int));

		  // 计算各个灰度值的计数
		  for (i = 0; i < height; i ++)
		  {
			  for (j = 0; j < width; j ++)
			  {
				  lpSrc = src + width * i + j;

				  // 计数加1
#ifdef _DEBUG
				  if(*(lpSrc)>max_val)
					  cout<<"像素值超出范围"<<endl;
#endif
				  lCount[*(lpSrc)]++;
			  }
		  }
		  int sum=0;
		  for(int i=0;i<max_val+1;++i)
		  {
			  sum+=lCount[i];

		  }
		  // 计算灰度映射表
		  for (i = 0; i <max_val+1; i++)
		  {
			  // 初始为0
			  lTemp = 0;

			  for (j = 0; j <= i ; j++)
			  {
				  lTemp += lCount[j];
			  }

			  // 计算对应的新灰度值
			  int tp=lTemp * (float)max_val / (height *width)+0.5f;
			 
			  if(tp>max_val)
				  tp=max_val;

			  bMap[i] = (ushort) tp;
		  }

		  // 每行
		  for(i = 0; i <height; i++)
		  {
			  // 每列
			  for(j = 0; j <width; j++)
			  {
				  // 指向DIB第i行，第j个象素的指针
				  lpSrc = src +width * (height - 1 - i) + j;

				  // 计算新的灰度值
				  *lpSrc = bMap[*lpSrc];
			  }
		  }

		  delete [] lCount;
		  delete [] bMap;
  };
void Enhance::CLAHE(ushort *pSrc,int width,int height)
{
	::CLAHE(pSrc,width,height, 
		0,4095, 8, 8,
		0,0.1);
}