#ifndef HDFOperat_H_
#define HDFOperat_H_

class HDFOperat
{
public:
	/*
	*filename �ļ�����·��+�ļ�����
	*pData     ͼ�����ݻ���������DIB����
	*width ���ؿ��
	*height ���ظ߶�
	*bytes ÿ�������ֽ���
	*/
	static int Data2Raw(const char *filename,void *pData,int width,int height,int bytes);
	static int Data2Tiff(const char* filename,void *pData,int width,int height,int bytes);
	static int Data2GeoTiff(const char* filename,void *pData,int width,int height,int bytes);
};
#endif