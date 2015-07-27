#include "Enhance.h"
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
using namespace HDFTools;
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

	Horizontal_Linear_fill(dest,width,height);
	Vertical_Linear_fill2(pSrc,width,height);
	
	ushort valH=filledVal;
	ushort valV=filledVal;
	for(int y=0;y<height;++y )
		for(int x=0;x<width;++x)
		{
			valH=*ptempH;
			valV=*ptempV;
			if(valV==filledVal&&valH!=filledVal)
				*dest=valH;
			else if(valV!=filledVal&&valH==filledVal)
				*dest=valV;
			else if(valV!=filledVal&&valH!=filledVal)
				*dest=ushort((valH+valV)/2.0f+0.5f);
			++dest;
			++ptempH;
			++ptempV;
		}
}

void Enhance::Bilinear_interpolation(ushort *pSrc,int width,int height)
{
	ushort filledVal=0;
	//ˮƽ��ֱ�ֱ���в�ֵ����
	ushort *pHBuf=new ushort[width*height];
	if(pHBuf==NULL)
		return;

	ushort *pVBuf=new ushort[width*height];
	if(pVBuf==NULL)
	{
		delete []pHBuf;
		return ;
	}
	ushort *ptempH=pHBuf;
	ushort *ptempV=pVBuf;
	memcpy(pHBuf,pSrc,width*height*sizeof(ushort));
	memcpy(pVBuf,pSrc,width*height*sizeof(ushort));
	Horizontal_Linear_fill(pVBuf,width,height);
	Vertical_Linear_fill(pHBuf,width,height);
	ushort valH=filledVal;
	ushort valV=filledVal;
	for(int y=0;y<height;++y )
		for(int x=0;x<width;++x)
		{
			valH=*ptempH;
			valV=*ptempV;
			if(valV==filledVal&&valH!=filledVal)
				*pSrc=valH;
			else if(valV!=filledVal&&valH==filledVal)
				*pSrc=valV;
			else if(valV!=filledVal&&valH!=filledVal)
				*pSrc=ushort((valH+valV)/2.0f+0.5f);
			++pSrc;
			++ptempH;
			++ptempV;
		}
	delete []pHBuf;
	delete []pVBuf;
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
void Enhance::Horizontal_Linear_fill(unsigned short *src,int width,int height)
{
	//��ʱ��ԭͼ����䣬��ʹ��des
	//memcpy(des,src,width*height*sizeof(unsigned short));
	unsigned short filledVal=0;
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
		Fill_HorizontalLine(pStart,pEnd,filledVal);
		y+=linepartition;
	}
}
void Enhance::Vertical_Linear_fill2(ushort *src,int width,int height)
{
	//���������γ������ʱ����Ҫ����γ�����ݲ��侭����Ϣ�����Ծ������ǰ�ȸ�n�����γ������侭��
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
void Enhance::Vertical_Linear_fill(unsigned short *src,int width,int height)
{
	//���������γ������ʱ����Ҫ����γ�����ݲ��侭����Ϣ�����Ծ������ǰ�ȸ�n�����γ������侭��
	ushort filledVal=0;
	Add_VerticalHorLine(src,width,height,filledVal);
	//���ط�ת��ˮƽ
	unsigned short *back=new unsigned short[width*height];
	memset(back,0,width*height*sizeof(unsigned short));
	unsigned short *temp=back;
	for(int x=0;x<width;++x)
		for(int y=0;y<height;++y)
			*(temp++)=*(src+x+y*width);
	
	//����ˮƽ��������
	Horizontal_Linear_fill(back,height,width);
	//���ػ�ԭ��ֱ
	temp=back;
	for(int y=0;y<height;++y)//���
		for(int x=0;x<width;++x)//�߶�
			*(src++)=*(temp+y+x*height);
	delete[] back;
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
 void Enhance::Average_Smooth(ushort *src,ushort* dest,int width,int height)
 {
	 memcpy(dest,src,sizeof(ushort)*width*height);
	 int arr[25]=  {
		 0,1,2,1,0,
		 1,2,4,2,1,
		 2,4,8,4,2,
		 1,2,4,2,1,
		 0,1,2,1,0
	 };
	 ushort templateMidX=1,templateMidY=1;
	 float factor=0.02083f;
	 Convolution(src,dest,width,height,5,5,2,2,arr,factor);
 }
  void Enhance::Average_Smooth(ushort *src,int width,int height)
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
	  Enhance::Bilinear_interpolation(pSrc,dest,width,height);//���ɽ�����浽dest�У�����pSrc�Ѿ����޸Ĺ���
	  //Enhance::Horizontal_Linear_fill(tempBuf,m_width,m_height);
	  //Enhance::Vertical_Linear_fill(tempBuf,m_width,m_height);
	  //Enhance::nearest_fill_unshort(tempBuf,m_width,m_height,0,4,3);
	  ///ƽ��ȥ��
	  //Enhance::Median_Filter2(tempBuf,m_width,m_height);//̫��ʱ
	  Enhance::Average_Smooth(dest,pSrc,width,height);
	  //memcpy(pSrc,dest,sizeof(ushort)*width*height);
	  delete []dest;
  }


void Enhance::Cast16To8(ushort *src,uchar* des,int width,int height)
{
	//ushort16 to uchar8


}
void Enhance::Cast12To8(ushort *src,uchar* des,int width,int height)
{
	//ushort12 to uchar8
	const ushort max_val12=4095;
	const ushort min_val12=0;
	const uchar max_val8=255;
	const uchar min_val8=0;
	int len=height*width;
	ushort *pData=src;
	uchar* pDest=des;
	//�������ֵ�Ƿ��ڷ�Χ�ڣ������ٸ������ֵcast
	memset(des,0,width*height);
	for(int i=0;i<len;++i)
		if(*pData++>max_val12)
		{
			//����14��16����
			return ;//���ز���12λ��Χ��
		}
	pData=src;
	for(int i=0;i<len;++i)
	{
		ushort temp=(*pData)*256.0/4096.0+0.5;
		if(temp>max_val8)
			temp=max_val8;
		*pDest=(uchar)temp;
		pData++;
		pDest++;
	}
}