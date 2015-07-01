#ifndef HDFOperat_H_
#define HDFOperat_H_

class HDFOperat
{
public:
	/*
	*filename 文件名（路径+文件名）
	*pData     图像数据缓冲区，非DIB数据
	*width 像素宽度
	*height 像素高度
	*bytes 每个像素字节数
	*/
	static int Data2Raw(const char *filename,void *pData,int width,int height,int bytes);
	static int Data2Tiff(const char* filename,void *pData,int width,int height,int bytes);
	static int Data2GeoTiff(const char* filename,void *pData,int width,int height,int bytes);
};
#endif