#include<cv.h>
#include<highgui.h>
#include<math.h>
#include<time.h>
#include<iostream>       //存储int型变量用32位
//#include<vector>
#include <stdlib.h>

#define SWAP(a,b) tempr=(a);(a)=(b);(b)=tempr

# pragma comment(linker, "/NODEFAULTLIB:atlthunk.lib")
# pragma comment(linker, "/NODEFAULTLIB:LIBCMT")
# pragma comment(linker, "/NODEFAULTLIB:MSVCRTD")
BOOL fourn(double * data/*psrc*/, unsigned long nn[]/*w*/, int ndim/*2*/, int isign);
void main(int argc, char argv[])
{
	IplImage *Gray = cvLoadImage("ceshi1.jpg",-1);
	cvNamedWindow("src");
	cvShowImage("src",Gray);
	int lHeight = Gray->height;
	int lWidth = Gray->width;
	int lLineBytes = Gray->widthStep;
	int lw = 1;
	int lh = 1;
	int wp = 0;
	int hp = 0;
	//保证离散傅里叶变换的宽度和高度为2的整数幂
	while(lw*2 <= lLineBytes)
	{
		lw = lw*2;0;  
		wp++;
	}
	while(lh*2 <= lHeight)
	{
		lh = lh*2;
		hp++;
	}
	//输入退化图像的长和宽必须为2的整数倍；
	if(lw != (int)lLineBytes)
	{
		return;
	}
	if(lh != (int)lHeight)
	{
		return;
	}
	double *fftSrc, *fftKernel,*fftNoise;
	fftSrc = new double [lHeight*lLineBytes*2+1];
	fftKernel = new double [lHeight*lLineBytes*2+1];
        fftNoise = new double [lHeight*lLineBytes*2+1];
	unsigned long nn[3] = {0};
	nn[1] = lHeight;
	nn[2] = lLineBytes;
	LPSTR lpSrc;
	unsigned char pixel;
	for (int j = 0;j < lHeight ;j++)
	{
		for(int i = 0;i < lLineBytes ;i++)
		{
			// 指向源图像倒数第j行，第i个象素的指针   
			lpSrc = (char *)Gray->imageData + lLineBytes * j + i;

			pixel = (unsigned char)*lpSrc;

			fftSrc[(2*lLineBytes)*j + 2*i + 1] = (double)pixel;
			fftSrc[(2*lLineBytes)*j + 2*i + 2] = 0.0;

			if(i < 15 && j == 0)
			{
				fftKernel[(2*lLineBytes)*j + 2*i + 1] = 0.0667;
			}
			else
			{
				fftKernel[(2*lLineBytes)*j + 2*i + 1] = 0.0;
			}
			fftKernel[(2*lLineBytes)*j + 2*i + 2] = 0.0;
			if( i + j == ((int)((i+j)/8))*8)
			{
				fftNoise[(2*lLineBytes)*j + 2*i +1] = -16.0;
			}
			else
			{
				fftNoise[(2*lLineBytes)*j + 2*i +1] = 0.0;
			}
			fftNoise[(2*lLineBytes)*j + 2*i +2] = 0.0;
		}
	}
	//srand((unsigned)time(NULL));
	//对源图像进行FFT
	fourn(fftSrc,nn,2,1);
	//对卷积核图像进行FFT
	fourn(fftKernel,nn,2,1);
	//对噪声图像进行FFT
	fourn(fftNoise,nn,2,1);
	double a,b,c,d,e,f,multi;
	for (int i = 1;i <lHeight*lLineBytes*2;i+=2)
	{
		a = fftSrc[i];
		b = fftSrc[i+1];
		c = fftKernel[i];
		d = fftKernel[i+1];
		e = fftNoise[i];
		f = fftNoise[i+1];
		multi = (a*a + b*b)/(a*a + b*b + e*e + f*f);
		if (c*c + d*d > 1e-3)
		{
			fftSrc[i] = ( a*c + b*d ) / ( c*c + d*d )/multi;
			fftSrc[i+1] = ( b*c - a*d ) / ( c*c + d*d )/multi;
		}
	}

	//对结果图像进行反FFT
	fourn(fftSrc,nn,2,-1);
	//确定归一化因子
	//图像归一化因子
	double MaxNum;
	MaxNum = 0.0;
	for (int j = 0;j < lHeight ;j++)
	{
		for(int i = 0;i < lLineBytes ;i++)
		{
			fftSrc[(2*lLineBytes)*j + 2*i + 1] = 
				sqrt(fftSrc[(2*lLineBytes)*j + 2*i + 1] * fftSrc[(2*lLineBytes)*j + 2*i + 1]\
				+fftSrc[(2*lLineBytes)*j + 2*i + 2] * fftSrc[(2*lLineBytes)*j + 2*i + 2]);
			if( MaxNum < fftSrc[(2*lLineBytes)*j + 2*i + 1])
				MaxNum = fftSrc[(2*lLineBytes)*j + 2*i + 1];
		}
	}
	//转换为图像
	for (int j = 0;j < lHeight ;j++)
	{
		for(int i = 0;i < lLineBytes ;i++)
		{
			// 指向源图像倒数第j行，第i个象素的指针   
			lpSrc = (char *)Gray->imageData + lLineBytes * j + i;

			*lpSrc = (unsigned char) (fftSrc[(2*lLineBytes)*j + 2*i + 1]*255.0/MaxNum/2);
		}
	}
	cvNamedWindow("dst");
	cvShowImage("dst",Gray);
	cvWaitKey(0);

	
}



BOOL fourn(double * data/*psrc*/, unsigned long nn[]/*w*/, int ndim/*2*/, int isign)
{
	int idim;
	unsigned long i1,i2,i3,i2rev,i3rev,ip1,ip2,ip3,ifp1,ifp2;
	unsigned long ibit,k1,k2,n,nprev,nrem,ntot;
	double tempi,tempr;
	double theta,wi,wpi,wpr,wr,wtemp;

	for (ntot=1,idim=1;idim<=ndim;idim++)
		ntot *= nn[idim];
	nprev=1;
	for (idim=ndim;idim>=1;idim--) {
		n=nn[idim];
		nrem=ntot/(n*nprev);
		ip1=nprev << 1;
		ip2=ip1*n;
		ip3=ip2*nrem;
		i2rev=1;
		for (i2=1;i2<=ip2;i2+=ip1) {
			if (i2  < i2rev) {
				for (i1=i2;i1<=i2+ip1-2;i1+=2) {
					for (i3=i1;i3<=ip3;i3+=ip2) {
						i3rev=i2rev+i3-i2;
						SWAP(data[i3],data[i3rev]);
						SWAP(data[i3+1],data[i3rev+1]);
					}
				}
			}
			ibit=ip2 >> 1;
			while (ibit >= ip1 && i2rev > ibit) {
				i2rev -= ibit;
				ibit >>= 1;
			}
			i2rev += ibit;
		}
		ifp1=ip1;
		while (ifp1 < ip2) {
			ifp2=ifp1 << 1;
			theta=isign*6.28318530717959/(ifp2/ip1);
			wtemp=sin(0.5*theta);
			wpr = -2.0*wtemp*wtemp;
			wpi=sin(theta);
			wr=1.0;
			wi=0.0;
			for (i3=1;i3<=ifp1;i3+=ip1) {
				for (i1=i3;i1<=i3+ip1-2;i1+=2) {
					for (i2=i1;i2<=ip3;i2+=ifp2) {
						k1=i2;
						k2=k1+ifp1;
						tempr=wr*data[k2]-wi*data[k2+1];
						tempi=wr*data[k2+1]+wi*data[k2];
						data[k2]=data[k1]-tempr;
						data[k2+1]=data[k1+1]-tempi;
						data[k1] += tempr;
						data[k1+1] += tempi;
					}
				}
				wr=(wtemp=wr)*wpr-wi*wpi+wr;
				wi=wi*wpr+wtemp*wpi+wi;
			}
			ifp1=ifp2;
		}
		nprev *= n;
	}
	return true;
}