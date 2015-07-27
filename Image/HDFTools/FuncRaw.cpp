#include "FuncRaw.h"
#include "stdio.h"
#include <iostream>
#include <string>
#include "DataSet.h"
#include "Enhance.h"
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
	return Data2Raw();
}

int FuncRaw::Data2Raw()
{
	//以下为测试代码，提取像素转成8位输出
	if(m_pData==NULL)
		return -1;
	unsigned char*dest=new unsigned char[m_height*m_width];
	unsigned short *data=(unsigned short*)m_pData;
	char index[10];

	for(int i=0;i<m_zcoord;++i)
	{
		Enhance::Cast12To8(data+m_width*m_height*i,dest,m_width,m_height);
		sprintf_s(index,"%d",i);
		FILE* fp = fopen((m_outfile+index).c_str(), "wb");
		if(fp==NULL)
		{
			std::cout<<"Can not open file at "<<__LINE__<<std::endl;
			return -1;
		}
		fwrite(dest, m_width,m_height, fp);
		fclose(fp);
	}
	delete []dest;
	return 0;
}