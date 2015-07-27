#include <stdio.h>
#include "cv.h"
#include "cxcore.h"
#include "highgui.h"
#include "opencv2/opencv.hpp"

/**************************************************************************
//����Ҷ�任
//src IPL_DEPTH_8U
//dst IPL_DEPTH_64F
/**************************************************************************/


void fft2(IplImage *src, IplImage *dst)
{  
	IplImage *image_Re = 0, *image_Im = 0, *Fourier = 0; //ʵ�����鲿


	image_Re = cvCreateImage(cvGetSize(src), IPL_DEPTH_64F, 1);  //ʵ��


	image_Im = cvCreateImage(cvGetSize(src), IPL_DEPTH_64F, 1);  //�鲿


	Fourier = cvCreateImage(cvGetSize(src), IPL_DEPTH_64F, 2);//2 channels (image_Re, image_Im)


	cvConvertScale(src, image_Re, 1, 0);// Real part conversion from u8 to 64f (double)


	cvZero(image_Im);// Imaginary part (zeros)


	cvMerge(image_Re, image_Im, 0, 0, Fourier);// Join real and imaginary parts and stock them in Fourier image


	cvDFT(Fourier, dst, CV_DXT_FORWARD);// Application of the forward Fourier transform


	cvReleaseImage(&image_Re);
	cvReleaseImage(&image_Im);
	cvReleaseImage(&Fourier);
}


/**************************************************************************
//���Ļ�
//src IPL_DEPTH_64F
//dst IPL_DEPTH_8U
**************************************************************************/
void fft2shift(IplImage *src, IplImage *dst)
{
	IplImage *image_Re = 0, *image_Im = 0;
	int nRow, nCol, i, j, cy, cx;
	double scale, shift, tmp13, tmp24;


	image_Re = cvCreateImage(cvGetSize(src), IPL_DEPTH_64F, 1);


	image_Im = cvCreateImage(cvGetSize(src), IPL_DEPTH_64F, 1);
	cvSplit( src, image_Re, image_Im, 0, 0 );


	//����ԭ���������˹����ͼ����p123
	// Compute the magnitude of the spectrum Mag = sqrt(Re^2 + Im^2)
	//���㸵��Ҷ��
	cvPow( image_Re, image_Re, 2.0);
	cvPow( image_Im, image_Im, 2.0);
	cvAdd( image_Re, image_Im, image_Re);
	cvPow( image_Re, image_Re, 0.5 );


	//�����任����ǿ�Ҷȼ�ϸ��(���ֱ任ʹ��խ���ͻҶ�����ͼ��ֵӳ��һ������ֵ������ɼ�������˹����ͼ����p62)
	// Compute log(1 + Mag);
	cvAddS( image_Re, cvScalar(1.0), image_Re ); // 1 + Mag
	cvLog( image_Re, image_Re ); // log(1 + Mag)


	//Rearrange the quadrants of Fourier image so that the origin is at the image center
	nRow = src->height; nCol = src->width;
	cx = nCol/2; cy = nRow/2; // image center


	//CV_IMAGE_ELEMΪOpenCV����ĺ꣬������ȡͼ�������ֵ����һ���־��ǽ������ı任
	for( j = 0; j < cy; j++ ){
		for( i = 0; i < cx; i++ ){
			//���Ļ���������ݳ��Ŀ���жԽǽ���
			tmp13 = CV_IMAGE_ELEM( image_Re, double, j, i);
			CV_IMAGE_ELEM( image_Re, double, j, i) = CV_IMAGE_ELEM(image_Re, double, j+cy, i+cx);
			CV_IMAGE_ELEM( image_Re, double, j+cy, i+cx) = tmp13;


			tmp24 = CV_IMAGE_ELEM( image_Re, double, j, i+cx);
			CV_IMAGE_ELEM( image_Re, double, j, i+cx) =CV_IMAGE_ELEM( image_Re, double, j+cy, i);
			CV_IMAGE_ELEM( image_Re, double, j+cy, i) = tmp24;
		}
	}
	//��һ�����������Ԫ��ֵ��һΪ[0,255]
	//[(f(x,y)-minVal)/(maxVal-minVal)]*255
	double minVal = 0, maxVal = 0;
	// Localize minimum and maximum values
	cvMinMaxLoc( image_Re, &minVal, &maxVal );
	// Normalize image (0 - 255) to be observed as an u8 image
	scale = 255/(maxVal - minVal);
	shift = -minVal * scale;
	cvConvertScale(image_Re, dst, scale, shift);
	cvReleaseImage(&image_Re);
	cvReleaseImage(&image_Im);


}


/***********************************************************************/
int main()
{
	IplImage *src;          //Դͼ��
	IplImage *Fourier;   //����Ҷϵ��
	IplImage *dst ;


	IplImage *ImageRe;
	IplImage *ImageIm;


	IplImage *Image;
	IplImage *ImageDst;


	double m,M;
	double scale;
	double shift;
	src = cvLoadImage("F:\\temp\\out.jpg",0);   //����Դͼ�񣬵ڶ���������ʾ�������ͼƬתΪ���ŵ� 
	Fourier = cvCreateImage(cvGetSize(src),IPL_DEPTH_64F,2);
	dst = cvCreateImage(cvGetSize(src),IPL_DEPTH_64F,2);
	ImageRe = cvCreateImage(cvGetSize(src),IPL_DEPTH_64F,1);
	ImageIm = cvCreateImage(cvGetSize(src),IPL_DEPTH_64F,1);
	Image = cvCreateImage(cvGetSize(src),src->depth,src->nChannels);
	ImageDst = cvCreateImage(cvGetSize(src),src->depth,src->nChannels);
	fft2(src,Fourier);                  //����Ҷ�任
	fft2shift(Fourier, Image);          //���Ļ�
	cvDFT(Fourier,dst,CV_DXT_INV_SCALE);//ʵ�ָ���Ҷ��任�����Խ����������
	cvSplit(dst,ImageRe,ImageIm,0,0);


	cvNamedWindow("Դͼ��",0);
	cvShowImage("Դͼ��",src);             
	//������ÿ��Ԫ��ƽ�����洢�ڵڶ���������
	cvPow(ImageRe,ImageRe,2);               
	cvPow(ImageIm,ImageIm,2);
	cvAdd(ImageRe,ImageIm,ImageRe,NULL);
	cvPow(ImageRe,ImageRe,0.5);
	cvMinMaxLoc(ImageRe,&m,&M,NULL,NULL);
	scale = 255/(M - m);
	shift = -m * scale;
	//��shift����ImageRe��Ԫ�ذ��������ŵĽ���ϣ��洢ΪImageDst
	cvConvertScale(ImageRe,ImageDst,scale,shift);


	cvNamedWindow("����Ҷ��",0);
	cvShowImage("����Ҷ��",Image);
	cvNamedWindow("����Ҷ��任",0);
	cvShowImage("����Ҷ��任",ImageDst);


	cvWaitKey(0);


	cvReleaseImage(&src);
	cvReleaseImage(&Image);
	cvReleaseImage(&ImageIm);
	cvReleaseImage(&ImageRe);
	cvReleaseImage(&Fourier);
	cvReleaseImage(&dst);
	cvReleaseImage(&ImageDst);
        cvDestroyAllWindows();
	return 0;
}