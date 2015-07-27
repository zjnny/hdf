#ifndef Enhance_h_
#define Enhance_h_
namespace HDFTools
{
	class Enhance
	{
	public:
		typedef unsigned short ushort;
		typedef unsigned char uchar;
		static void ImageEnhance(ushort *pSrc,int width,int height);
		static void Nearest_interpolation(ushort *pSrc,int width,int height);
		static void Bilinear_interpolation(ushort *pSrc,int width,int height); 
		static bool nearest_fill_unshort(unsigned short *pData, int width, int height, unsigned short fill_value, int nloop, int slide_window_width);
		static void Median_Filter(unsigned short *src,int width,int height);
		static void Median_Filter2(unsigned short *src,int width,int height);
		static void Average_Smooth(ushort *src,int width,int height);
		static void Cast16To8(ushort *src,uchar* des,int width,int height);
		static void Cast12To8(ushort *src,uchar* des,int width,int height);
	protected:
		static void Horizontal_Linear_fill(unsigned short *src,int width,int height);
		static void Vertical_Linear_fill(unsigned short *src,int width,int height);
		static void Vertical_Linear_fill2(ushort *src,int width,int height);
	protected:
		//需要外部提前申请temp内存,该函数会修改pSrc的内容，pSrc内容失效
		static void Bilinear_interpolation(ushort *pSrc,ushort* dest,int width,int height); 
		//
		static void Average_Smooth(ushort *src,ushort* dest,int width,int height);

		static void Convolution(ushort *src,int width,int height,ushort tplW,ushort tplH,ushort tplMX,ushort tplMY,int *tplArr,float fac);
		static void Convolution(ushort *src,ushort *des,int width,int height,ushort tplW,ushort tplH,ushort tplMX,ushort tplMY,int *tplArr,float fac);
		static void Fill_HorizontalLine(unsigned short *start,unsigned short*end,unsigned short filledVal);
		static void Fill_VerticalLine(ushort *start,ushort*end,ushort width,ushort filledVal);
		static void Add_VerticalHorLine(unsigned short *src,int width,int height,unsigned short filledVal);
	};
}

#endif