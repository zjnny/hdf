#ifndef FuncRaw_h_
#define FuncRaw_h_
#include "FuncObject.h"
#include <string>
class DataSet;
namespace HDFTools
{
	class FuncRaw:public FuncObject
	{
	public:
		FuncRaw();
		~FuncRaw();
		int Execute();
	protected:
		int Data2Raw();
		int Data2Raw2();
	public:
		void SetParam(const char * file,void *pData,int width,int height,int bytes);
		virtual void SetParam(void * lparam,void *rparam);
	private:
		std::string m_outfile;//输出文件全路径名
		void *m_pData;//数据缓冲区指针
		int m_width;//图像宽度pixel
		int m_height;//图像高度pixel
		int m_zcoord;//
		int m_bytes;//每个像素字节数
		DataSet *m_pDataSet;
	};
}

#endif