#include "DataSet.h"
#include <iostream>
#include <cassert>
#include "HDF5Helper.h"
using namespace std;
using namespace HDFTools;
DataSet::DataSet()
{
	m_Dimensions=2;//是几维的数据结构
	m_DimVal[0]=1;//每维的长度
	m_DimVal[1]=1;
	m_DimVal[2]=1;
	m_pData=NULL;//数据内容
	m_Bytes=2;//每单位数据占用的字节数
	m_Type=H5T_NATIVE_USHORT;
}
DataSet::~DataSet()
{
	Destroy();
}
void DataSet::Destroy()
{
	if(m_pData!=NULL)
	{
		delete [] m_pData;
		m_pData=NULL;
	}
	m_Dimensions=2;//是几维的数据结构
	m_DimVal[0]=1;//每维的长度
	m_DimVal[1]=1;
	m_DimVal[2]=1;//
	m_Bytes=2;//每单位数据占用的字节数
}
bool DataSet::IsPlanar()
{
	return m_DimVal[2]==1;
}
void DataSet::Init(void* pData,uchar Dimensions,ushort *DimVal,uchar bytes)
{
	Destroy();
	m_pData=pData;
	m_Dimensions=Dimensions;
	if(m_Dimensions>3)
		throw(exception("unimplement"));
	for(int i=0;i<m_Dimensions;++i)
	{
		m_DimVal[i]=DimVal[i];
	}
}
void DataSet::SetAttr(std::vector<HDFTools::AttrItem> &vc)
{
	m_attr=vc;
}
std::vector<HDFTools::AttrItem>* DataSet::GetAttr()
{
	return &m_attr;
}
void DataSet::GetAttrItem(HDFTools::AttrItem &item)
{
	for(int i=0;i<m_attr.size();++i)
	{
		if(m_attr[i].strName==item.strName)
		{
			item=m_attr[i];
			return;
		}
	}
}
void DataSet::InitFromFile(const char* file,std::string grouppath)
{
	Destroy();
	if(grouppath.rfind('/')==string::npos)
	{
		throw exception("must use absulote path like \"/Data/DataSet\"");
	}
	m_GroupPath=grouppath;
	m_Name=grouppath.substr(grouppath.rfind('/')+1,grouppath.length());
	HDF5Helper::GetDataSetAttr(file,grouppath,m_Dimensions,m_DimVal,m_Type,m_Bytes);
	if(m_Dimensions<1)
		return ;
	if(m_DimVal[2]==0)
		m_DimVal[2]=1;
	HDF5Helper::GetDSAttrInfo(file,grouppath.c_str(),m_attr);
	if(m_Type==UNSHORTTYPE)//m_Type  USHORT
	{
		assert(m_Bytes==2);

		m_pData=new unsigned short[(m_DimVal[0]*m_DimVal[1]*m_DimVal[2])];
		memset(m_pData,0,sizeof(unsigned short)*m_DimVal[0]*m_DimVal[1]*m_DimVal[2]);
		HDF5Helper::GetDataSet(file,grouppath,m_pData,H5T_NATIVE_USHORT);
		
	}
	else if(m_Type==FLOATTYPE)//m_Type  FLOAT
	{
		assert(m_Bytes==4);
		m_pData=new float[m_DimVal[0]*m_DimVal[1]];
		memset(m_pData,0,sizeof(float)*m_DimVal[0]*m_DimVal[1]);
		HDF5Helper::GetDataSet(file,grouppath,m_pData,H5T_NATIVE_FLOAT);
		
	}
	else
	{
		cout<<"datatype Unimplement"<<endl;
	}
	
}
void DataSet::SetDim(uchar Dimensions,ushort *DimVal)
{
	m_Dimensions=Dimensions;
	for(int i=0;i<Dimensions;++i)
	{
		m_DimVal[i]=DimVal[i];
		if(m_DimVal[i]==0)
			m_DimVal[i]=1;
	}
	
}
void DataSet::SetData(void *pData)
{
	m_pData=pData;
}