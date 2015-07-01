#ifndef FuncProject_h_
#define FuncProject_h_
#include "FuncObject.h"
namespace HDFTools
{
	typedef float DEGREE_TYPE;
	struct Vertor2
	{
		int x;
		int y;
	};
	class FuncProject:public FuncObject
	{
	public:
		FuncProject();
		~FuncProject();
		int Execute();
		void SetParam(void *pDataSet,DEGREE_TYPE*pLongitude,DEGREE_TYPE *pLatitude,int srcwidth,int srcheight,int bytes);
	protected:
		int Project();
		void MakeVector();
		void Convert();
		void FindMaxMin();
		void FindMaxMin(DEGREE_TYPE& min_val,DEGREE_TYPE& max_val,DEGREE_TYPE* pBuff);
	private:
		void *m_pDataSet;//Դ���ݼ���Ϣ
		void *m_pDesSet;//���մ洢���ݼ�
		DEGREE_TYPE	*m_pLongitude;//���ȼ�
		DEGREE_TYPE *m_pLatitude;//γ�ȼ�
		Vertor2 * m_pVecBuf;
		int m_srcwidth;
		int m_srcheight;
		int m_deswidth;//���ɺ�����ݼ���
		int m_desheight;//���ɺ�����ݼ���
		int m_bytes;
		//���ȷ�Χ
		DEGREE_TYPE m_max_lon;
		DEGREE_TYPE m_min_lon;
		
		//γ�ȷ�Χ
		DEGREE_TYPE m_max_lat;
		DEGREE_TYPE m_min_lat;
		//��γ��ת��������ϵ��λ
		DEGREE_TYPE m_unit_lon;
		DEGREE_TYPE m_unit_lat;
		//
		

	};
}

#endif