#include "Projector.h"
#include "Coordinate.h"
#include "Enhance.h"
#include <iostream>
#include <string>
#include <cassert>
#include <locale>
#include "../HDFTools/ToolFactory.h"
#include "../HDFTools/FuncObject.h"
#include "DataSet.h"
#include <iomanip>
using namespace HDFTools;
using namespace std;
Projector::Projector(Coordinate *lon,Coordinate *lat):
	m_width(0),
	m_height(0),
	m_pData(NULL),
	m_pLongitude(lon),
	m_pLatitude(lat)
{

}
Projector::~Projector()
{
	Destroy();
}
void Projector::Destroy()
{
	m_pData=NULL;
	m_pLongitude=NULL;
	m_pLatitude=NULL;
	//被投影数据集指针
	m_pSrcData=NULL;
	m_pProjected.Destroy();

}
void Projector::Init(DataSet* pData)
{
	//投影结果数据集大小
	if(m_pLatitude==NULL||m_pLongitude==NULL)
		return;
	m_width=m_pLongitude->GetDestLength();
	m_height=m_pLatitude->GetDestLength();
	int dims=pData->GetDimensions();
	ushort dimval[3]={1,1,1};
	if(dims==2)
	{
		dimval[0]=m_height;
		dimval[1]=m_width;
	}
	if(dims==3)
	{
		dimval[0]=pData->GetZ();
		dimval[1]=m_height;
		dimval[2]=m_width;
	}
	m_pProjected.SetDataSetName(pData->GetDataSetName());
	m_pProjected.SetDataSetGroupPath(pData->GetDataSetGroupPath());
	m_pProjected.SetDim(dims,dimval);
	m_pProjected.SetAttr(*(pData->GetAttr()));
	m_pProjected.SetFileAttr(*(pData->GetFileAttr()));
	vector<AttrItem> *pAttr=m_pProjected.GetFileAttr();
	for(int i=0;i<pAttr->size();++i)
	{
		if((*pAttr)[i].strName=="Orbit Point Latitude"&&(*pAttr)[i].iDataType==FLOATTYPE&&(*pAttr)[i].iByteLen==16)
		{
			float *dat=(float*)((*pAttr)[i].pData);
			dat[0]=m_pLatitude->m_min_degree;
			dat[1]=m_pLatitude->m_max_degree;
			dat[2]=m_pLatitude->m_min_degree;
			dat[3]=m_pLatitude->m_max_degree;
		}
		else if((*pAttr)[i].strName=="Orbit Point Longitude"&&(*pAttr)[i].iDataType==FLOATTYPE&&(*pAttr)[i].iByteLen==16)
		{
			float *dat=(float*)((*pAttr)[i].pData);
			dat[0]=m_pLongitude->m_min_degree;
			dat[1]=m_pLongitude->m_max_degree;
			dat[2]=m_pLongitude->m_min_degree;
			dat[3]=m_pLongitude->m_max_degree;
		}
	}
}
void Projector::Project(DataSet* pData)
{
	Init(pData);
	int dims=pData->GetDimensions();
	int zcoord=pData->GetZ();
	m_pProjected.SetDataSetName(pData->GetDataSetName());
	//经纬度宽高应一致
	ushort srcw=m_pLongitude->GetWidth();
	ushort srch=m_pLongitude->GetHeight();
	cout<<"投影图像宽度="<<m_width<<" 高度="<<m_height<<endl;

	//投影源数据集
	assert(pData->GetBytes()==sizeof(ushort));
	

	
	int len=m_width*m_height*zcoord;
	int srclen=srcw*srch;
	m_pData=new ushort[len];
	if(m_pData==NULL)
		throw exception("Out of memory");
	m_pProjected.SetData(m_pData);
	//填充值

	::memset((void*)m_pData,0,len*sizeof(ushort));
	ushort *tempBuf=m_pData;
	ushort *pSrc=static_cast<ushort*>(pData->GetData());
	double start ,end ,cost;
	start=clock();
	for(int z=0;z<zcoord;++z)
	{
		//经纬度坐标数据集
		ushort *pLon=(ushort*)m_pLongitude->GetCoord();
		ushort *pLat=(ushort*)m_pLatitude->GetCoord();
		//m_width m_height!=srcw srch
		pSrc=static_cast<ushort*>(pData->GetData())+srch*srcw*z;
		tempBuf=m_pData+z*m_width*m_height;//short +sizeof(unshort)
		//在投影前原始图上处理//直方图均衡后会明显改变原始数据//
		//Enhance::Histogram_Equalization(pSrc,srcw,srch);
		for(int i=0;i<srclen;++i)
		{
			tempBuf[*pLon+(*pLat)*m_width]=*pSrc;
			++pLon;
			++pLat;
			++pSrc;
		}
		//处理每一帧
		Enhance::ImageEnhance(tempBuf,m_width,m_height);
		end=clock();
		cost=end-start;
		cout<<"图像投影+处理耗时："<<setprecision(5)<<cost/CLOCKS_PER_SEC<<endl;
	}
}
void Projector::Project(void *pData,ushort bytes)
{
	//Init();
	////经纬度宽高应一致
	//ushort srcw=m_pLongitude->GetWidth();
	//ushort srch=m_pLongitude->GetHeight();
	////经纬度坐标数据集
	//ushort *pLon=(ushort*)m_pLongitude->GetCoord();
	//ushort *pLat=(ushort*)m_pLatitude->GetCoord();
	////投影源数据集
	//assert(bytes==sizeof(ushort));
	//ushort *pSrc=static_cast<ushort*>(pData);
	//int len=m_width*m_height;
	//int srclen=srcw*srch;
	//m_pData=new ushort[len];
	//if(m_pData==NULL)
	//	throw exception("Out of memory");
	////填充值
	//memset(m_pData,0,len*sizeof(ushort));
	//for(int i=0;i<srclen;++i)
	//{
	//	m_pData[*pLon+(*pLat)*m_width]=*pSrc;
	//	++pLon;
	//	++pLat;
	//	++pSrc;
	//}
	////Enhance::nearest_fill_unshort(m_pData,m_width,m_height,0,4,3);

	////ushort *temp=new ushort[len];
	////Enhance::Median_Filter2(m_pData,temp,m_width,m_height);
	////memcpy(m_pData,temp,len*sizeof(ushort));

	////delete []temp;

}
void Projector::GenRaw(const char*file)
{
	FILE* fp = fopen(file, "wb");
	if(fp==NULL)
	{
		std::cout<<"Can not open file at "<<__LINE__<<std::endl;
		return;
	}
	fwrite(m_pData, m_width*sizeof(ushort),m_height, fp);
	fclose(fp);
}
void Projector::Output(const char* file)
{
	string outfile=file;
	string ext=outfile.substr(outfile.rfind('.')+1,outfile.length());
	ext=strlwr((char*)ext.c_str());
	//大小写转换
	ToolFactory &fac=ToolFactory::GetInstance();
	FuncObject *fc=NULL;
	HDFTools::TOOL_COMMAND cmd=TOOL_NONE;
	if(ext=="raw")
	{
		cmd=TOOL_GEN_RAW;
	}
	else if(ext=="hdf"||ext=="h5"||ext=="hdf5")
	{
		cmd=TOOL_GEN_HDF;
	}
	else if(ext=="tif")
	{
		cmd=TOOL_GEN_TIF;
	}
	else
	{
		cmd=TOOL_GEN_HDF;
	}

	fc=fac.Concrete(cmd);
	fc->SetParam(&m_pProjected,(void*)&outfile);
	if(cmd==TOOL_GEN_HDF)
		fc->SetParam(this,(void*)&outfile);
	if(fc)
	{
		fc->Execute();
		delete fc;
		fc=NULL;
	}
	else
		cout<<"Can not output to file"<<endl;
}

