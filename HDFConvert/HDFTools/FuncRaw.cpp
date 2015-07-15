#include "FuncRaw.h"
#include "stdio.h"
#include <iostream>
#include <string>
#include "../DataSet.h"
using namespace HDFTools;
FuncRaw::FuncRaw():m_pData(NULL),m_width(1),m_height(1),m_bytes(2)
{
	m_zcoord=1;
}
FuncRaw::~FuncRaw()
{
}
void  FuncRaw::SetParam(const char * file,void *pData,int width,int height,int bytes)
{
	m_outfile=file;
	m_pData=pData;
	m_width=width;
	m_height=height;
	m_bytes=bytes;
}
void FuncRaw::SetParam(void * lparam,void *rparam)
{
	m_pDataSet=(DataSet*)lparam;
	m_outfile=*(std::string*)rparam;
	m_pData=m_pDataSet->GetData();
	m_bytes=m_pDataSet->GetBytes();
	m_width=m_pDataSet->GetX();
	m_height=m_pDataSet->GetY();
	m_zcoord=m_pDataSet->GetZ();
}
int FuncRaw::Execute()
{
	return Data2Raw2();
}
int FuncRaw::Data2Raw2()
{
	if(m_pData==NULL)
		return -1;
	FILE* fp = fopen(m_outfile.c_str(), "wb");
	if(fp==NULL)
	{
		std::cout<<"Can not open file at "<<__LINE__<<std::endl;
		return -1;
	}
	fwrite(m_pData, m_width*m_bytes,m_height*m_zcoord, fp);
	fclose(fp);
	return 0;
	return 0;
}
int FuncRaw::Data2Raw()
{
	if(m_pData==NULL)
		return -1;
	FILE* fp = fopen(m_outfile.c_str(), "wb");
	if(fp==NULL)
	{
		std::cout<<"Can not open file at "<<__LINE__<<std::endl;
		return -1;
	}
	fwrite(m_pData, m_width*m_bytes,m_height*m_zcoord, fp);
	fclose(fp);
	return 0;
}