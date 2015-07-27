#include "FuncHDF.h"
#include "stdio.h"
#include "hdf5.h"
#include "../Project/DataSet.h"
#include "../Project/Projector.h"
#include "../Project/Coordinate.h"
#include "HDF5Helper.h"
#include <iostream>
#include <string>
#include <ctime>
#include <cassert>
using namespace std;
using namespace HDFTools;
static int FindAttrStr(string &name,char** str,int len);
//需要修改属性
static char * attrstr[]={
	"Data Creating Date",
	"Data Creating Time",
	"Data Pixels",
	"Data Lines",
	"Dataset Name",
	"File Name",
	"Left-Bottom X",
	"Left-Bottom Y",
	"Left-Top X",
	"Left-Top Y",
	"Right-Bottom X",
	"Right-Bottom Y",
	"Right-Top X",
	"Right-Top Y",
	"Time Of Data Composed"
};
enum HDFAttrName {
	Data_Creating_Date=0,
	Data_Creating_Time,
	Data_Pixels,
	Data_Lines,
	Dataset_Name,
	File_Name,
	Left_Bottom_X,
	Left_Bottom_Y,
	Left_Top_X,
	Left_Top_Y,
	Right_Bottom_X,
	Right_Bottom_Y,
	Right_Top_X,
	Right_Top_Y,
	Time_Of_Data_Composed
};
FuncHDF::FuncHDF()
{
	m_pDataSet=NULL;
	m_pLongitude=NULL;
	m_pLatitude=NULL;
}
FuncHDF::~FuncHDF()
{
}

void FuncHDF::SetParam(void * lparam,void *rparam)
{
	Projector *pro=(Projector *)lparam;
	m_pDataSet=pro->GetProjectedDS();
	m_pLatitude=pro->GetLatitude();
	m_pLongitude=pro->GetLongitude();
	m_outfile=*(string*)rparam;
}
int FuncHDF::Execute()
{
	Data2HDF();
	return 0;
}
int FindAttrStr(string &name,char** str,int len)
{
	for(int i=0;i<len;++i)
	{
		if(name==str[i])
			return i;
	}
	return -1;
}
void FuncHDF::Data2HDF()
{
	int dimens=m_pDataSet->GetDimensions();
	int x=m_pDataSet->GetX();
	int y=m_pDataSet->GetY();
	int z=m_pDataSet->GetZ();

	hsize_t hval[3]={1,1,1};
	if(dimens==2)
	{
		hval[0]=m_pDataSet->GetY();
		hval[1]=m_pDataSet->GetX();
	}
	else if(dimens==3)
	{
		hval[0]=m_pDataSet->GetZ();
		hval[1]=m_pDataSet->GetY();
		hval[2]=m_pDataSet->GetX();
	}
	//保存数据集数据
	HDF5Helper::SaveDataSet(m_outfile.c_str(),m_pDataSet->GetDataSetName(),m_pDataSet->GetData(),dimens,hval);
	//检查文件属性是否存在
	vector<AttrItem> vc;
	vector<AttrItem> tpvc;
	vector<AttrItem> *pAttrlist=NULL;
	string dayornight;
	HDF5Helper::GetFileAttrInfo(m_outfile.c_str(),tpvc);
	if(tpvc.size()==0)
	{
		//获取模版文件属性
		HDF5Helper::GetFileAttrInfo("FileAttrTemplate.hdf",vc);
		//FileAttrTemplate.hdf文件不存在
		assert(vc.size()!=0);
		pAttrlist=&vc;
		//如果模版文件不存在则使用原始hdf的头文件属性
		if(vc.size()==0)
			pAttrlist=m_pDataSet->GetFileAttr();
		for(int i=0;i<m_pDataSet->GetFileAttr()->size();++i)
			if((*m_pDataSet->GetFileAttr())[i].strName=="Day Or Night Flag")
			{
				dayornight=(char*)((*m_pDataSet->GetFileAttr())[i].pData);
			}
	}
	
	//修改文件默认属性值
	if(pAttrlist!=NULL&&(*pAttrlist).size()!=0)
	{
		for(int i=0;i<(*pAttrlist).size();++i)
		{
			int pos=FindAttrStr((*pAttrlist)[i].strName,attrstr,sizeof(attrstr)/4);
			if(pos>=0)
			{
				AttrItem*pItem=&((*pAttrlist)[i]);
				switch (pos)
				{
				case	Data_Creating_Date:
					{
						char ch[20];
						time_t tm1;
						time(&tm1);
						tm *t=localtime(&tm1);
						sprintf_s(ch,"%d-%02d-%02d",1900+t->tm_year,t->tm_mon,t->tm_mday);
						int len=strlen(ch)+1;
						if(pItem->pData!=NULL)
							delete[]pItem->pData;
						char* buf=new char[len];
						buf[len-1]='\0';
						memcpy(buf,ch,len);
						pItem->pData=buf;
						pItem->iByteLen=len;
						strcpy((char*)pItem->pData,ch);
					}
					break;
				case	Data_Creating_Time:
					{
						char ch[20];
						time_t tm1;
						time(&tm1);
						tm *t=localtime(&tm1);
						sprintf_s(ch,"%02d-%02d-%02d",t->tm_hour,t->tm_min,t->tm_sec);
						int len=strlen(ch)+1;
						if(pItem->pData!=NULL)
							delete[]pItem->pData;
						char* buf=new char[len];
						buf[len-1]='\0';
						memcpy(buf,ch,len);
						pItem->pData=buf;
						pItem->iByteLen=len;
						strcpy((char*)pItem->pData,ch);
					}
					break;
				case	Data_Pixels:
					{
						int *dat=(int*)pItem->pData;
						*dat=m_pDataSet->GetX();
					}
					
					break;
				case	Data_Lines:
					{
						int *dat=(int*)pItem->pData;
						*dat=m_pDataSet->GetY();
					}
					break;
				case	Dataset_Name:
					{
						char ch[50];
						sprintf_s(ch,"%s",m_pDataSet->GetDataSetName().c_str());
						int len=strlen(ch)+1;
						if(pItem->pData!=NULL)
							delete[]pItem->pData;
						char* buf=new char[len];
						buf[len-1]='\0';
						memcpy(buf,ch,len);
						pItem->pData=buf;
						pItem->iByteLen=len;
					}
					break;
				case	File_Name:
					{
						string fname=m_outfile;
						int pos=fname.rfind('/');
						if(pos<0)
							pos=fname.rfind('\\');
						if(pos<0)
							break;
						fname=fname.substr(pos+1,fname.length()-pos-1);

						int len=fname.length()+1;
						if(pItem->pData!=NULL)
							delete[]pItem->pData;
						char* buf=new char[len];
						buf[len-1]='\0';
						memcpy(buf,fname.c_str(),len);
						pItem->pData=buf;
						pItem->iByteLen=len;
					}
					
					break;
				case	Left_Bottom_X:
					{
						float *dat=(float*)pItem->pData;
						*dat=m_pLongitude->m_min_degree;
					}

					//*dat=;
					break;
				case	Left_Bottom_Y:
					{
						float *dat=(float*)pItem->pData;
						*dat=m_pLatitude->m_min_degree;
					}

					break;
				case	Left_Top_X:
					{
						float *dat=(float*)pItem->pData;
						*dat=m_pLongitude->m_min_degree;
					}

					break;
				case	Left_Top_Y:
					{
						float *dat=(float*)pItem->pData;
						*dat=m_pLatitude->m_max_degree;
					}

					break;
				case	Right_Bottom_X:
					{
						float *dat=(float*)pItem->pData;
						*dat=m_pLongitude->m_max_degree;
					}

					break;
				case	Right_Bottom_Y:
					{
						float *dat=(float*)pItem->pData;
						*dat=m_pLatitude->m_min_degree;
					}

					break;
				case	Right_Top_X:
					{
						float *dat=(float*)pItem->pData;
						*dat=m_pLongitude->m_max_degree;
					}
					break;
				case	Right_Top_Y:
					{
						float *dat=(float*)pItem->pData;
						*dat=m_pLatitude->m_max_degree;
					}
					break;
				case Time_Of_Data_Composed:
					{
						//Time Of Data Composed
						char ch[20];
						sprintf_s(ch,"%s",dayornight.c_str());
						int len=strlen(ch)+1;
						if(pItem->pData!=NULL)
							delete[]pItem->pData;
						char* buf=new char[len];
						buf[len-1]='\0';
						memcpy(buf,ch,len);
						pItem->pData=buf;
						pItem->iByteLen=len;
					}
					break;
				default:
					break;
				}
			}
		}
		//保存文件属性
		HDF5Helper::SetFileAttrInfo(m_outfile.c_str(),*pAttrlist);
	}
	HDF5Helper::SetAttrInfo(m_outfile.c_str(),m_pDataSet->GetDataSetName().c_str(),*(m_pDataSet->GetAttr()));
}
