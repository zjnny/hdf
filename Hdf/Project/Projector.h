/*
*投影过程实现
*/
#ifndef Projector_h_
#define Projector_h_
#include "DataSet.h"
class Coordinate;
class Projector
{
public:
	typedef unsigned short ushort;
	Projector(Coordinate *lon,Coordinate *lat);
	~Projector();
	//确保数据集与经纬度HW一致
	void Project(void *pData,ushort bytes);
	//投影2维数据
	void Project2D(void *pData,ushort bytes);
	void Project(DataSet* pData);
	void GenRaw(const char*file);
	void Output(const char* file);
	Coordinate* GetLatitude(){return m_pLatitude;}
	Coordinate* GetLongitude(){return m_pLongitude;}
	DataSet *GetProjectedDS(){return &m_pProjected;}
protected:
	void Init(DataSet* pData);
	void Destroy();
private:
	ushort m_width;
	ushort m_height;
	Coordinate *m_pLongitude;
	Coordinate *m_pLatitude;
	ushort *m_pData;
	//被投影数据集指针
	DataSet* m_pSrcData;
	//投影后数据集内容
	DataSet m_pProjected;
};
#endif