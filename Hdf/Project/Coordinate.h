/*
*用于存储经纬度坐标系
*/
#ifndef Coordinate_h_
#define Coordinate_h_
class Coordinate
{
public:
	typedef unsigned short ushort;
	Coordinate();
	~Coordinate();
	//初始化经度数据集，pDada源数据仍需手动释放
	void Init(int width,int height,void *pDada,int metre,unsigned char bytes=4);
	//获取最终应生成的图像大小
	int GetDestLength();
	ushort GetWidth();
	ushort GetHeight();
	void* GetCoord();
	void Convert();
	float GetResolution(){return m_pixelUnit;}
protected:
	//经度向坐标系转化，是否还保存原始坐标系？
	
	void FindMaxAndMin();
private:
	ushort m_width;
	ushort m_height;
	void *m_pSrcData;
	void *m_pData;//转换后像素坐标数据
	float m_pixelUnit;//每像素对应的度数
	ushort m_bytes;//字节数，根据最终坐标系的大小确定使用int 或short
public:
	float m_max_degree;//最大度数
	float m_min_degree;//最小度数
};


#endif