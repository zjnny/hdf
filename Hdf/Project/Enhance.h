#ifndef Enhance_h_
#define Enhance_h_
class Enhance
{
public:
	typedef unsigned short ushort;
	//ͳһ
	static void ImageEnhance(ushort *pSrc,int width,int height);

protected://��ֵ���
	static void Bicubic_Interpolation(ushort *pSrc,ushort* dest,int width,int height);
	static void Horizontal_Linear_fill(unsigned short *src,int width,int height);
	static void Horizontal_Linear_fill2(unsigned short *src,int width,int height);
	static void Vertical_Linear_fill(unsigned short *src,int width,int height);//ռ���ڴ�
	static void Vertical_Linear_fill2(ushort *src,int width,int height);
	static void Fill_HorizontalLine(unsigned short *start,unsigned short*end,unsigned short filledVal);
	static void Fill_VerticalLine(ushort *start,ushort*end,ushort width,ushort filledVal);
	static void Add_VerticalHorLine(unsigned short *src,int width,int height,unsigned short filledVal);
	static void Add_HorizontalVerLine(ushort *src,int width,int height,unsigned short filledVal);
	 
	static void Fill_HorizontalLine2(unsigned short *start,unsigned short*end,unsigned short filledVal);
	static void Fill_VerticalLine2(ushort *start,ushort*end,ushort width,ushort filledVal);
	static void Add_VerticalHorLine2(unsigned short *src,int width,int height,unsigned short filledVal);
	static void Add_HorizontalVerLine2(ushort *src,int width,int height,unsigned short filledVal);
public://��ֵ����
	//��Ҫ�ⲿ��ǰ����temp�ڴ�,�ú������޸�pSrc�����ݣ�pSrc����ʧЧ
	static void Bilinear_interpolation(ushort *pSrc,ushort* dest,int width,int height); 
	//�������+�������
	static void Bilinear_interpolation2(ushort *pSrc,ushort* dest,int width,int height); 
	static void Nearest_interpolation(ushort *pSrc,int width,int height);

	static bool nearest_fill_unshort(unsigned short *pData, int width, int height, unsigned short fill_value, int nloop, int slide_window_width);

public://ͼ����ǿ
	static void Gauss_Smooth(ushort *src,ushort* dest,int width,int height);
	static void Laplace_Sharp(ushort *src,ushort* dest,int width,int height);
	static void Convolution(ushort *src,int width,int height,ushort tplW,ushort tplH,ushort tplMX,ushort tplMY,int *tplArr,float fac);
	static void Convolution(ushort *src,ushort *des,int width,int height,ushort tplW,ushort tplH,ushort tplMX,ushort tplMY,int *tplArr,float fac);
	static void Median_Filter(unsigned short *src,int width,int height);
	static void Median_Filter2(unsigned short *src,int width,int height);
	static void Gauss_Smooth(ushort *src,int width,int height);
	static void CLAHE(ushort *pSrc,int width,int height);
	static void Histogram_Equalization(ushort *src,int width,int height);
};
#endif