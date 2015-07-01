#include "FuncProject.h"
#include <iostream>
#include <exception>
#include "FuncRaw.h"
using std::endl;
using std::cout;
using namespace HDFTools;
 FuncProject::FuncProject():
	 m_pDataSet(NULL),
	 m_pDesSet(NULL),
	 m_pLatitude(NULL),
	 m_pLongitude(NULL),
	 m_pVecBuf(NULL),
	 m_srcheight(0),m_srcwidth(0),m_desheight(0),m_deswidth(0)
 {

 }
 FuncProject::~FuncProject()
 {
	 if(m_pVecBuf!=NULL)
	 {
		 delete []m_pVecBuf;
		 m_pVecBuf=NULL;
	 }
	 if(m_pDesSet!=NULL)
	 {
		 delete[]m_pDesSet;
		 m_pDesSet=NULL;
	 }
 }
int FuncProject::Execute()
{
	try
	{
		Project();
		FuncRaw opt;
		opt.SetParam("F:\\temp\\project.raw",m_pDesSet,m_deswidth,m_desheight,sizeof(unsigned short));
		opt.Execute();	
	}
	catch (std::exception* e)
	{
		cout<<e->what()<<endl;
	}
	catch(...)
	{
		cout<<"Some error!"<<endl;
	}

	return 0;
}
void FuncProject::SetParam(void *pDataSet,DEGREE_TYPE*pLongitude,DEGREE_TYPE *pLatitude,int srcwidth,int srcheight,int bytes)
{
	m_pDataSet=pDataSet;
	m_pLongitude=pLongitude;
	m_pLatitude=pLatitude;
	m_deswidth=m_srcwidth=srcwidth;//最终生成的数据集与原数据集一样大小
	m_desheight=m_srcheight=srcheight;
}
void  FuncProject::Convert()
{
	unsigned short *pDesbuf=new unsigned short[m_deswidth*m_desheight];
	m_pDesSet=pDesbuf;
	unsigned short *pSrcbuf=(unsigned short *)m_pDataSet;
	Vertor2 *pVec=m_pVecBuf;
	memset(m_pDesSet,0,m_deswidth*m_desheight*sizeof(unsigned short));
	for(int i=0;i<m_srcheight*m_srcwidth;++i)
	{
		pDesbuf=(unsigned short*)m_pDesSet;
		int pos=(*pVec).x+(*pVec).y*m_deswidth;
		if(pos>m_srcheight*m_srcwidth)
		{
			cout<<"error"<<__LINE__<<endl;
			++pDesbuf;
			++pVec;
			++pSrcbuf;
			continue;
		}
		pDesbuf[pos]=*pSrcbuf++;
		++pDesbuf;
		++pVec;
	}
}
int FuncProject::Project()
{
	if(m_pDataSet==NULL||m_pLatitude==NULL||m_pLongitude==NULL)
		return -1;
	if(m_srcheight*m_srcwidth<=0||m_deswidth*m_desheight<=0)
		return -1;
	//建立经纬度坐标buff,存储区域buf，以及各项参数
	MakeVector();

	Convert();
	return 0;
}
void FuncProject::FindMaxMin(DEGREE_TYPE& min_val,DEGREE_TYPE& max_val,DEGREE_TYPE* pBuff)
{

}
void FuncProject::FindMaxMin()
{
	DEGREE_TYPE min_lon=-180.0;
	DEGREE_TYPE max_lon=180.0;
	DEGREE_TYPE min_lat=-90.0;
	DEGREE_TYPE max_lat=90.0;
	//最大经度
	//FindMaxMin(m_min_lon,m_max_lon,m_pLongitude);
	//最大纬度
	//FindMaxMin(m_min_lat,m_max_lat,m_pLatitude);
	m_min_lon=121.095f;m_max_lon=170.08118f;
	m_min_lat=-62.443058f,m_max_lat=-39.669846f;
	if(m_max_lon>max_lon||m_min_lon<min_lon||m_max_lat>max_lat||m_min_lat<min_lat)
		throw std::exception("经纬度数据可能异常");
	m_unit_lon=(m_max_lon-m_min_lon)/(m_srcwidth-1);//没有-1则最终生成数组变成width+1
	m_unit_lat=(m_max_lat-m_min_lat)/(m_srcheight-1);
}
void  FuncProject::MakeVector()
{
	DEGREE_TYPE*temp_lon=m_pLongitude;
	DEGREE_TYPE*temp_lat=m_pLatitude;
	
	FindMaxMin();//确保最大最小数正确
	//MakeBuf
	m_pVecBuf=new Vertor2[m_srcwidth*m_srcheight];
	Vertor2 *pbuf=m_pVecBuf;
	for(int i=0;i<m_srcwidth*m_srcheight;++i)
	{
		(*pbuf).x=int(((*temp_lon++)-m_min_lon)/m_unit_lon+0.5f);//经度longitude
		(*pbuf).y=int(((*temp_lat++)-m_min_lat)/m_unit_lat+0.5f);//纬度latitude
		++pbuf;
	}
	
}