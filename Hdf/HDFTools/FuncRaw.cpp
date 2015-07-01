#include "FuncRaw.h"
#include "stdio.h"
#include <iostream>
using namespace HDFTools;
FuncRaw::FuncRaw():m_pData(NULL),m_width(0),m_height(0),m_bytes(1)
{
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
int FuncRaw::Execute()
{
	return Data2Raw();
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
	fwrite(m_pData, m_width*m_bytes,m_height, fp);
	fclose(fp);
	return 0;
}