/*
*ͶӰ����ʵ��
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
	//ȷ�����ݼ��뾭γ��HWһ��
	void Project(void *pData,ushort bytes);
	//ͶӰ2ά����
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
	//��ͶӰ���ݼ�ָ��
	DataSet* m_pSrcData;
	//ͶӰ�����ݼ�����
	DataSet m_pProjected;
};
#endif