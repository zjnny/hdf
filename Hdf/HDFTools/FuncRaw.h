#ifndef FuncRaw_h_
#define FuncRaw_h_
#include "FuncObject.h"
#include <string>
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
	public:
		void SetParam(const char * file,void *pData,int width,int height,int bytes);
	private:
		std::string m_outfile;//����ļ�ȫ·����
		void *m_pData;//���ݻ�����ָ��
		int m_width;//ͼ����pixel
		int m_height;//ͼ��߶�pixel
		int m_bytes;//ÿ�������ֽ���
	};
}

#endif