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
		void *m_pDataSet;//源数据集信息
		void *m_pDesSet;//最终存储数据集
		DEGREE_TYPE	*m_pLongitude;//经度集
		DEGREE_TYPE *m_pLatitude;//纬度集
		Vertor2 * m_pVecBuf;
		int m_srcwidth;
		int m_srcheight;
		int m_deswidth;//生成后的数据集宽
		int m_desheight;//生成后的数据集高
		int m_bytes;
		//经度范围
		DEGREE_TYPE m_max_lon;
		DEGREE_TYPE m_min_lon;
		
		//纬度范围
		DEGREE_TYPE m_max_lat;
		DEGREE_TYPE m_min_lat;
		//经纬度转换成坐标系单位
		DEGREE_TYPE m_unit_lon;
		DEGREE_TYPE m_unit_lat;
		//
		

	};
}

#endif