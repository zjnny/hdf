#include<cv.h>
#include<highgui.h>
#include<math.h>
#include<time.h>
#include<iostream>       //�洢int�ͱ�����32λ
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
	BYTE data1;
	for(int i=0;i<Gray->height;i++)
	{
		for(int j=0;j<Gray->width;j++)
		{
			data1=(BYTE)Gray->imageData[i*Gray->widthStep+j];
		}
	}
//	IplImage *Gray = cvCreateImage(cvGetSize(RGB),RGB->depth,1);
	//cvCvtColor(RGB,Gray,CV_BGR2GRAY);
	//cvCopyImage(RGB,Gray);
	int lHeight = Gray->height;
	int lWidth = Gray->width;
	int lLineBytes = Gray->widthStep;
	int lw = 1;
	int lh = 1;
	int wp = 0;
	int hp = 0;
    //��֤��ɢ����Ҷ�任�Ŀ�Ⱥ͸߶�Ϊ2��������
	while(lw*2 <= lLineBytes)
	{
		lw = lw*2;
		wp++;
	}
	while(lh*2 <= lHeight)
	{
		lh = lh*2;
		hp++;
	}
	//�����˻�ͼ��ĳ��Ϳ����Ϊ2����������
    if(lw != (int)lLineBytes)
	{
		return;
	}
	if(lh != (int)lHeight)
	{
		return;
	}
	//������FFT������

			// ָ��Դͼ������j�У���i�����ص�ָ��   
			
	double *fftSrc, *fftKernel;
	fftSrc = new double [lHeight*lLineBytes*2+1];
	fftKernel = new double [lHeight*lLineBytes*2+1];
	unsigned long nn[3] = {0};
	nn[1] = lHeight;
	nn[2] = lLineBytes;
	LPSTR lpSrc;
	unsigned char pixel;
	for (int j = 0;j < lHeight ;j++)
	{
		for(int i = 0;i < lLineBytes ;i++)
		{
			// ָ��Դͼ������j�У���i�����ص�ָ��   
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
		}
	}

	//��Դͼ�����FFT
	fourn(fftSrc,nn,2,1);
	//�Ծ����ͼ�����FFT
	fourn(fftKernel,nn,2,1);
	double a,b,c,d;
	for (int j = 0;j < lHeight ;j++)
	{
		for(int i = 0;i < lLineBytes ;i++)
		{
			a = fftSrc[(2*lLineBytes)*j + 2*i + 1];
			b = fftSrc[(2*lLineBytes)*j + 2*i + 2];
			c = fftKernel[(2*lLineBytes)*j + 2*i + 1];
			d = fftKernel[(2*lLineBytes)*j + 2*i + 2];
			if (c*c + d*d > 1e-3)
			{
				fftSrc[(2*lLineBytes)*j + 2*i + 1] = ( a*c + b*d ) / ( c*c + d*d );
				fftSrc[(2*lLineBytes)*j + 2*i + 2] = ( b*c - a*d ) / ( c*c + d*d );
			}
		}
	}

	//�Խ��ͼ����з�FFT
	fourn(fftSrc,nn,2,-1);

	//ȷ����һ������
	//ͼ���һ������
	double MaxNum;
	MaxNum = 0.0;
	for (int j = 0;j < lHeight ;j++)
	{
		for(int i = 0;i < lLineBytes ;i++)
		{
			fftSrc[(2*lLineBytes)*j + 2*i + 1] = 
				sqrt(fftSrc[(2*lLineBytes)*j + 2*i + 1] * fftSrc[(2*lLineBytes)*j + 2*i + 1]/
				+fftSrc[(2*lLineBytes)*j + 2*i + 2] * fftSrc[(2*lLineBytes)*j + 2*i + 2]);
			if( MaxNum < fftSrc[(2*lLineBytes)*j + 2*i + 1])
				MaxNum = fftSrc[(2*lLineBytes)*j + 2*i + 1];
		}
	}
	//ת��Ϊͼ��
	for (int j = 0;j < lHeight ;j++)
	{
		for(int i = 0;i < lLineBytes ;i++)
		{
			// ָ��Դͼ������j�У���i�����ص�ָ��   
			lpSrc = (char *)Gray->imageData + lLineBytes * j + i;

			*lpSrc = (unsigned char) (fftSrc[(2*lLineBytes)*j + 2*i + 1]*255.0/MaxNum);
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




/*

//����˵��:
//LPSTR lpDIBBits    - ָ��ԴDIBͼ��ָ��
//LONG  lWidth       - Դͼ���ȣ���������������4�ı�����
//LONG  lHeight      - Դͼ��߶ȣ���������
/
//����˵��:
//�ú���������BlurDIB()���ɵ�DIBͼ����и�ԭ������
BOOL WINAPI RestoreDIB (LPSTR lpDIBBits, LONG lWidth, LONG lHeight)
{
	// ָ��Դͼ���ָ��
	LPSTR lpSrc;
	//ѭ������
	long i;
	long j;
	//����ֵ
	unsigned char pixel;
	// ͼ��ÿ�е��ֽ���
	LONG lLineBytes;
	//������FFT������
	double *fftSrc,*fftKernel;
	double a,b,c,d;
	//��άFFT�ĳ��ȺͿ��
	unsigned long nn[3];
	//ͼ���һ������
	double MaxNum;
	// ����ͼ��ÿ�е��ֽ���
	lLineBytes = WIDTHBYTES(lWidth * 8);
	double dPower = log((double)lLineBytes)/log(2.0);
	if(dPower != (int) dPower)
	{
		return false;
	}
	dPower = log((double)lHeight)/log(2.0);
	if(dPower != (int) dPower)
	{
		return false;
	}
	fftSrc = new double [lHeight*lLineBytes*2+1];
	fftKernel = new double [lHeight*lLineBytes*2+1];
	nn[1] = lHeight;
	nn[2] = lLineBytes;
	for (j = 0;j < lHeight ;j++)
	{
		for(i = 0;i < lLineBytes ;i++)
		{
			// ָ��Դͼ������j�У���i�����ص�ָ��   
			lpSrc = (char *)lpDIBBits + lLineBytes * j + i;

			pixel = (unsigned char)*lpSrc;

			fftSrc[(2*lLineBytes)*j + 2*i + 1] = (double)pixel;
			fftSrc[(2*lLineBytes)*j + 2*i + 2] = 0.0;

			if(i < 5 && j == 0)
			{
				fftKernel[(2*lLineBytes)*j + 2*i + 1] = 1/5.0;
			}
			else
			{
				fftKernel[(2*lLineBytes)*j + 2*i + 1] = 0.0;
			}
			fftKernel[(2*lLineBytes)*j + 2*i + 2] = 0.0;
		}
	}

	//��Դͼ�����FFT
	fourn(fftSrc,nn,2,1);
	//�Ծ����ͼ�����FFT
	fourn(fftKernel,nn,2,1);
	for (j = 0;j < lHeight ;j++)
	{
		for(i = 0;i < lLineBytes ;i++)
		{
			a = fftSrc[(2*lLineBytes)*j + 2*i + 1];
			b = fftSrc[(2*lLineBytes)*j + 2*i + 2];
			c = fftKernel[(2*lLineBytes)*j + 2*i + 1];
			d = fftKernel[(2*lLineBytes)*j + 2*i + 2];
			if (c*c + d*d > 1e-3)
			{
				fftSrc[(2*lLineBytes)*j + 2*i + 1] = ( a*c + b*d ) / ( c*c + d*d );
				fftSrc[(2*lLineBytes)*j + 2*i + 2] = ( b*c - a*d ) / ( c*c + d*d );
			}
		}
	}
	//�Խ��ͼ����з�FFT
	fourn(fftSrc,nn,2,-1);

	//ȷ����һ������
	MaxNum = 0.0;
	for (j = 0;j < lHeight ;j++)
	{
		for(i = 0;i < lLineBytes ;i++)
		{
			fftSrc[(2*lLineBytes)*j + 2*i + 1] = 
				sqrt(fftSrc[(2*lLineBytes)*j + 2*i + 1] * fftSrc[(2*lLineBytes)*j + 2*i + 1]/
				+fftSrc[(2*lLineBytes)*j + 2*i + 2] * fftSrc[(2*lLineBytes)*j + 2*i + 2]);
			if( MaxNum < fftSrc[(2*lLineBytes)*j + 2*i + 1])
				MaxNum = fftSrc[(2*lLineBytes)*j + 2*i + 1];
		}
	}

	//ת��Ϊͼ��
	for (j = 0;j < lHeight ;j++)
	{
		for(i = 0;i < lLineBytes ;i++)
		{
			// ָ��Դͼ������j�У���i�����ص�ָ��   
			lpSrc = (char *)lpDIBBits + lLineBytes * j + i;

			*lpSrc = (unsigned char) (fftSrc[(2*lLineBytes)*j + 2*i + 1]*255.0/MaxNum);
		}
	}
	delete fftSrc;
	delete fftKernel;
	// ����
	return true;
}

*/

//BOOL fourn(double * data/*psrc*/, unsigned long nn[]/*w*/, int ndim/*2*/, int isign)
/*{
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
			if (i2 < i2rev) {
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

*/