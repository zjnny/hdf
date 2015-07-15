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
		std::string m_outfile;//����ļ�ȫ·����
		void *m_pData;//���ݻ�����ָ��
		int m_width;//ͼ����pixel
		int m_height;//ͼ��߶�pixel
		int m_zcoord;//
		int m_bytes;//ÿ�������ֽ���
		DataSet *m_pDataSet;
	};
}

#endif