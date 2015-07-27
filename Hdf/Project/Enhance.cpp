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
	//tempָ����pSrc��ͬ��С�ڴ棬�Ѿ�����
	if(pSrc==NULL||dest==NULL||width*height==0)
		return ;
	ushort filledVal=0;
	//ˮƽ��ֱ�ֱ���в�ֵ����
	ushort *ptempH=dest;
	ushort *ptempV=pSrc;
	memcpy(dest,pSrc,width*height*sizeof(ushort));

	//Horizontal_Linear_fill(dest,width,height);
	//Vertical_Linear_fill(pSrc,width,height);
	//���������˶Դ�ֱ����Ĳ���
	Horizontal_Linear_fill(dest,width,height);
	Vertical_Linear_fill(pSrc,width,height);
	
	ushort valH=filledVal;
	ushort valV=filledVal;
	for(int y=0;y<height;++y )
		for(int x=0;x<width;++x)
		{
			valH=*ptempH;
			valV=*ptempV;
			if(valV==filledVal&&valH!=filledVal)//�Ƿ�������ɶ����Ե��
				*dest=valH;
			else if(valV!=filledVal&&valH==filledVal)//�Ƿ����������Ե��
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
	//tempָ����pSrc��ͬ��С�ڴ棬�Ѿ�����
	if(pSrc==NULL||dest==NULL||width*height==0)
		return ;
	ushort filledVal=0;
	//ˮƽ��ֱ�ֱ���в�ֵ����
	ushort *ptempH=dest;
	ushort *ptempV=pSrc;
	ushort *pDest=dest;
	memcpy(dest,pSrc,width*height*sizeof(ushort));

	//Horizontal_Linear_fill(dest,width,height);
	//Vertical_Linear_fill(pSrc,width,height);
	//���������˶Դ�ֱ����Ĳ���
	Horizontal_Linear_fill2(dest,width,height);
	Vertical_Linear_fill2(pSrc,width,height);

	ushort valH=filledVal;
	ushort valV=filledVal;
	for(int y=0;y<height;++y )
		for(int x=0;x<width;++x)
		{
			valH=*ptempH;
			valV=*ptempV;
		//ȱ��һ��γ�ȵ���Ϣ����䣬ʹ�������������ʣ��Ŀհ�
			/*	if(valV!=filledVal&&valH!=filledVal)
			*pDest=ushort((valH+valV)/2.0f+0.5f);*/
			if(valV!=filledVal&&valH!=filledVal)
					*pDest=ushort((valH*(float)width/(width+height)+valV*(float)height/(width+height))+0.5f);
			++pDest;
			++ptempH;
			++ptempV;
		}
	//�ٽ�ֵ��䣬����
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
		//�û������ڷ������Ȩƽ��ֵ�����в�ֵ
		// 		for(data_row = 0; data_row < height; ++data_row)
		// 		{
		// 			for(data_col = 0; data_col < width; ++data_col)
		// 			{
		// 				//���������±�
		// 				data_index = data_row * width + data_col;
		// 				//����ֵ�Ĳ�����ֵ
		// 				if (pData[data_index] != 0)
		// 				{
		// 					continue;
		// 				}
		// 				//���㻬�������������е���ʼ�к�
		// 				window_row = data_row - (slide_window_width/2);
		// 				int obs_row_temp = window_row;	
		// 				//˫��ѭ����ɨ���������Χslide_window_width��Χ�ڵ��������е�ÿһ����
		// 				for(; window_row < obs_row_temp + slide_window_width; ++window_row)
		// 				{
		// 					
		// 					//�޳���Ч����
		// 					if(window_row < 0 || window_row >= height)
		// 					{
		// 						continue;
		// 					}
		// 					//���㻬�������ڵ��������е���ʼ�к�
		// 					window_col = data_col - (slide_window_width/2);
		// 					int obs_col_temp = window_col;
		// 					for(; window_col < obs_col_temp + slide_window_width; ++window_col)
		// 					{					
		// 						//�޳���Ч����
		// 						if(window_col < 0 || window_col >= width)
		// 						{
		// 							continue;
		// 						}
		// 						//���㴰�������±�
		// 						window_index = window_row * width + window_col;
		// 						//�޳���Чֵ
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
		// 				//��������
		// 				distance_sum = 0;
		// 				distance.clear();
		// 				window_data.clear();
		// 				coeff.clear();
		// 			}
		// 		}

		//�ٽ�ֵ��ֵ��
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
	//�㷨������ʹ��Median_Filter2
	memcpy(des,src,width*height*sizeof(ushort));
	int tempW=3;//ģ����
	int tempH=3;//ģ��߶�
	int offx=width-tempW+1;//ˮƽ�ƶ�
	int offy=height-tempH+1;//��ֱ�ƶ�

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
	int tempW=3;//ģ����
	int tempH=3;//ģ��߶�
	int offx=width-tempW+1;//ˮƽ�ƶ�
	int offy=height-tempH+1;//��ֱ�ƶ�
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
			//ȥ�����ֵ��Ӱ��
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
		//��β�ض���������ֵ
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
			if(len<20)//���ȹ�������ʧ��
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
	//start,end��һ����ֵλ�õ���ʵ,��s��e��ʼ���ҿհ����
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
		//Ѱ���������[start,end]
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
		pEnd+=1;//�������һ��(start,end)
		//
		if(pEnd-pStart<20)//50�����صĳ���
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
		//Ѱ���������[start,end]
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
		//�������һ��[start,end],
		pEnd+=step;
		//������ȹ���˵������ʧ���������
		if(pEnd-pStart<20*step)//50�����صĳ���
			Fill_VerticalLine(pStart,pEnd,width,filledVal);
		y+=linepartition;
	}
}
void Enhance::Fill_HorizontalLine(unsigned short *start,unsigned short*end,unsigned short filledVal)
{
	//��β�ض���������ֵ
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
	//start,end��һ����ֵλ�õ���ʵ,��s��e��ʼ���ҿհ����
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
	//γ�����죬��Ҫ��������������Ϊ��׼���侭����Ϣ������γ�����ǰ�ȸ�n����侭�������γ��
	Add_HorizontalVerLine2(src,width,height,filledVal);
	//��ʱ��ԭͼ����䣬��ʹ��des
	//memcpy(des,src,width*height*sizeof(unsigned short));
	for(int y=0;y<height;++y)
	{
		//Ѱ���������[start,end]
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
		pEnd+=1;//�������һ��[start,end),������endָ��
		
		Fill_HorizontalLine2(pStart,pEnd,filledVal);
	}
}
void Enhance::Horizontal_Linear_fill(unsigned short *src,int width,int height)
{
	unsigned short filledVal=0;
	//γ�����죬��Ҫ��������������Ϊ��׼���侭����Ϣ������γ�����ǰ�ȸ�n����侭�������γ��
	Add_HorizontalVerLine(src,width,height,filledVal);
	//��ʱ��ԭͼ����䣬��ʹ��des
	//memcpy(des,src,width*height*sizeof(unsigned short));
	for(int y=0;y<height;++y)
	{
		//Ѱ���������[start,end]
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
		pEnd+=1;//�������һ��[start,end),������endָ��

		Fill_HorizontalLine(pStart,pEnd,filledVal);
	}
}
void  Enhance::Add_HorizontalVerLine(ushort *src,int width,int height,unsigned short filledVal)
{
	int linepartition=5;
	int step=width;
	for(int y=0;y<width;)
	{
		//Ѱ���������[start,end]
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
		//�������һ��[start,end],
		pEnd+=step;
		//������ȹ���˵������ʧ���������
		if(pEnd-pStart<50*step)//50�����صĳ���
			Fill_VerticalLine(pStart,pEnd,width,filledVal);
		y+=linepartition;
	}
}
void Enhance::Add_VerticalHorLine(unsigned short *src,int width,int height,unsigned short filledVal)
{
	int linepartition=5;
	for(int y=0;y<height;)
	{
		//Ѱ���������[start,end]
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
		pEnd+=1;//�������һ��(start,end)
		//
		//if(pEnd-pStart<400)//50�����صĳ���
			Fill_HorizontalLine(pStart,pEnd,filledVal);
		y+=linepartition;
	}
}
void Enhance::Vertical_Linear_fill2(ushort *src,int width,int height)
{
	//�������죬��Ҫ����γ��������Ϊ��׼���侭����Ϣ�����Ծ������ǰ�ȸ�n�����γ������侭��
	ushort filledVal=0;
	Add_VerticalHorLine2(src,width,height,filledVal);
	int step=width;
	for(int y=0;y<width;++y)
	{
		//Ѱ���������[start,end]
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
		//�������һ��[start,end],
		pEnd+=step;
		
		Fill_VerticalLine2(pStart,pEnd,width,filledVal);
	}
	
}
void Enhance::Vertical_Linear_fill(unsigned short *src,int width,int height)
{
	//�������죬��Ҫ����γ��������Ϊ��׼���侭����Ϣ�����Ծ������ǰ�ȸ�n�����γ������侭��
	ushort filledVal=0;
	Add_VerticalHorLine(src,width,height,filledVal);
	int step=width;
	for(int y=0;y<width;++y)
	{
		//Ѱ���������[start,end]
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
		//�������һ��[start,end],
		pEnd+=step;

		Fill_VerticalLine(pStart,pEnd,width,filledVal);
	}

}
void Enhance::Convolution(ushort *src,ushort *des,int width,int height,ushort tplW,ushort tplH,ushort tplMX,ushort tplMY,int *tplArr,float fac)
{
	////�������
	float fResult=0.0f;
	ushort *p=src;
	ushort *ndata=des;
	memcpy(ndata,src,width*height*sizeof(ushort));
	for (int a=tplMY;a<height-tplH+tplMX+1;a++)
	{
		for (int b=tplMX;b<width-tplW+tplMX+1;b++)
		{
			//a,bģ������λ��
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
	 ////�������
	 float fResult=0.0f;
	 ushort *p=src;
	 ushort *ndata=new ushort[width*height];
	 memcpy(ndata,src,width*height*sizeof(ushort));
	 for (int a=tplMY;a<height-tplH+tplMX+1;a++)
	 {
		 for (int b=tplMX;b<width-tplW+tplMX+1;b++)
		 {
				//a,bģ������λ��
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
	  ///��ֵ
	  ushort *dest=new ushort[width*height];
	  //���ɽ�����浽dest�У�����pSrc�Ѿ����޸Ĺ���(Ϊ�����ÿռ䣬����ͼ������ڴ治��)
	 // Enhance::Bilinear_interpolation2(pSrc,dest,width,height);
	 // memcpy(pSrc,dest,width*height*sizeof(ushort));
	  Enhance::nearest_fill_unshort(pSrc,width,height,0,4,3);
	  ///ƽ��ȥ��
	 // Enhance::Median_Filter2(dest,width,height);//̫��ʱ
	 //  memcpy(dest,pSrc,sizeof(ushort)*width*height);
	 // Enhance::Gauss_Smooth(pSrc,dest,width,height);
	 //  Enhance::Laplace_Sharp(dest,pSrc,width,height);
	  //memcpy(pSrc,dest,sizeof(ushort)*width*height);
	  delete []dest;
  }
  void  Enhance::Histogram_Equalization(ushort *src,int width,int height)
  {
	  // ָ��Դͼ���ָ��
	  //const int max_val=16383;//14λ
	  const int max_val=4095;//12λ
	   //const int max_val=65535;//16λ
	  ushort*	lpSrc;
		 // // ��ʱ����
		int	lTemp;
		 // // ѭ������
		int	i;
		int	j;
		// �Ҷ�ӳ���
		ushort	*bMap=new ushort[max_val+1];
		 memset(bMap,0, (max_val+1)*sizeof(ushort));
		 // // �Ҷ�ӳ���
		 int	*lCount=new int[max_val+1];
		 memset(lCount,0,(max_val+1)*sizeof(int));

		  // ��������Ҷ�ֵ�ļ���
		  for (i = 0; i < height; i ++)
		  {
			  for (j = 0; j < width; j ++)
			  {
				  lpSrc = src + width * i + j;

				  // ������1
#ifdef _DEBUG
				  if(*(lpSrc)>max_val)
					  cout<<"����ֵ������Χ"<<endl;
#endif
				  lCount[*(lpSrc)]++;
			  }
		  }
		  int sum=0;
		  for(int i=0;i<max_val+1;++i)
		  {
			  sum+=lCount[i];

		  }
		  // ����Ҷ�ӳ���
		  for (i = 0; i <max_val+1; i++)
		  {
			  // ��ʼΪ0
			  lTemp = 0;

			  for (j = 0; j <= i ; j++)
			  {
				  lTemp += lCount[j];
			  }

			  // �����Ӧ���»Ҷ�ֵ
			  int tp=lTemp * (float)max_val / (height *width)+0.5f;
			 
			  if(tp>max_val)
				  tp=max_val;

			  bMap[i] = (ushort) tp;
		  }

		  // ÿ��
		  for(i = 0; i <height; i++)
		  {
			  // ÿ��
			  for(j = 0; j <width; j++)
			  {
				  // ָ��DIB��i�У���j�����ص�ָ��
				  lpSrc = src +width * (height - 1 - i) + j;

				  // �����µĻҶ�ֵ
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