#include "Coordinate.h"
#include <iostream>
#include <exception>
#include <cassert>
using namespace std;

Coordinate::Coordinate():
	m_width(0),
	m_height(0),
	m_pData(NULL),
	m_pSrcData(NULL),
	m_pixelUnit(0.0025f),
	m_bytes(4)
{

}
Coordinate::~Coordinate()
{
	if(m_pData!=NULL)
	{
		delete []m_pData;
		m_pData=NULL;
	}
}
void Coordinate::Init(int width,int height,void *pDada,int metre,unsigned char bytes)
{
	m_width=width;
	m_height=height;
	m_pixelUnit=metre/100000.0f;
	m_pSrcData=pDada;
	FindMaxAndMin();
	//Convert();
	//���������С����
	//ת��Ϊ����ϵ����ȷ��ʹ�õ��ֽ�����
	//m_pData=pDada;
	//m_bytes=bytes;
	
}
int Coordinate::GetDestLength()
{
	int len=int((m_max_degree-m_min_degree)/m_pixelUnit/*+0.5f*/)+1;
	if(len<1)
		throw exception("���㳤�ȴ���");
	if(len>40000)
		throw exception("���������п��ܴ����쳣ֵ��������������ڴ�");
	return len;
}
void Coordinate::FindMaxAndMin()
{
	m_max_degree=0.0f;
	m_min_degree=0.0f;
	float *pBuf=static_cast<float*>(m_pSrcData);
	float tpMax=-10000.0,tpMin=10000.0;
	for(int i=0;i<m_width*m_height;++i)
	{
		tpMax=max(*pBuf,tpMax);
		tpMin=min(*pBuf,tpMin);
		++pBuf;
	}
	m_max_degree=tpMax;
	m_min_degree=tpMin;
}
void Coordinate::Convert()
{
	//ת��Ϊ����ϵ����ȷ��ʹ�õ��ֽ�����
	//��ʱ��Ϊ��γ������ȫ��float��
	float *pBuf=static_cast<float*>(m_pSrcData);
	int len=m_width*m_height;
	m_pData=new ushort[len];
	assert(m_pData!=NULL);
	unsigned short *pTemp=static_cast<ushort*>(m_pData);
	for(int i=0;i<len;++i)
	{
		*pTemp++=ushort((*pBuf++-m_min_degree)/m_pixelUnit+0.5f);
	}
	//ת����m_pSrcData��Դû�����壬����ʹ�ø�ָ��
	m_pSrcData=NULL;
}
unsigned short Coordinate::GetWidth()
{
	return m_width;
}
unsigned short Coordinate::GetHeight()
{
	return m_height;
}
void*Coordinate::GetCoord()
{
	return m_pData;
}