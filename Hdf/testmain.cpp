#include "stdafx.h"
#ifdef HDF_DEBUG
#include "vld.h"
#endif
#include <cassert>
#include "HDFTools/FuncProject.h"
#include "HDFTools/FuncAnalysis.h"
#include "HDFTools/HDF5Helper.h"
#include "Project/Projector.h"
#include "Project/Coordinate.h"
#include "hdf5func.h"
#include <map>
using namespace HDFTools;

void * pData=NULL;
//经度信息
void *pLonData=NULL;
//纬度信息
void *pLatData=NULL;
void *pDataSet=NULL;

int _tmain(int argc, _TCHAR* argv[])
{
	////HDFTools::HDF5Helper::IsDataSetExist("F:\\temp\\HDF\\FY3C_MERSI_GBAL_L1_20150526_2340_GEO1K_MS.HDF","DEM");

	//hid_t hFile =H5Fopen("F:\\temp\\HDF\\FY3C_MERSI_GBAL_L1_20150526_2335_0250M_MS.HDF",H5F_ACC_RDONLY,H5P_DEFAULT);
	////hid_t hDSet=H5Dopen(hFile,"/Data/EV_250_RefSB_b1");
	//hid_t hgrp=H5Gopen(hFile,"Data");

	//hid_t hDSet=H5Dopen(hFile,"/Data/EV_250_RefSB_b1");
	//hid_t	memspace;
	//hid_t hDsSpace= H5Dget_space(hDSet);
	//int width=0,height=0;
	//hid_t	RANK;	
	//hsize_t	hDims[3]={0,0,0};
	//hsize_t	dimsw[3] = { 0 };	
	//RANK=H5Sget_simple_extent_ndims(hDsSpace);
	//if(H5Sget_simple_extent_dims(hDsSpace, hDims, dimsw)<0)
	//	return -1;
	//width=hDims[1];
	//height=hDims[0];
	////herr_t hStatus = H5Sselect_hyperslab (hDsSpace, H5S_SELECT_SET, NULL, NULL, hDims, NULL);  
	//memspace = H5Screate_simple (RANK, hDims, NULL); 


	//pDataSet=new unsigned short[width*height];
	//memset(pDataSet,0,width*height*sizeof(unsigned short));

	//H5Dread(hDSet,H5T_NATIVE_USHORT,memspace,hDsSpace,H5P_DEFAULT,pDataSet);
	char* file="F:\\temp\\HDF\\FY3C_MERSI_GBAL_L1_20150526_2335_0250M_MS.HDF";
	string dset="/Data/EV_250_RefSB_b1";
	int dimensions=0;
	hsize_t dim[3]={0};
	hid_t type=0;
	size_t bytes=0;
	int res=HDF5Helper::GetDataSetAttr(file,dset,dimensions,dim,type,bytes);
	if(res==0)
	{
		//H5T_NATIVE_USHORT
		//	H5T_NATIVE_FLOAT 
		//	H5T_NATIVE_UCHAR
		//	H5T_NATIVE_CHAR
		unsigned short *pdset;
		pdset=new unsigned short[dim[0]*dim[1]];
		HDF5Helper::GetDataSet(file,dset,pdset,H5T_NATIVE_USHORT);
	}
#pragma region REGIONFIRST
	//HDFTools::FuncAnalysis ana;
	//ana.SetParam(argc,argv);
	//ana.Execute();
#pragma endregion
#pragma region MyRegion
	//HDF5Func m_hdf;
	//DataTypeEnum datatype=UNCHARTYPE;
	//int dimensions=0,datatype2=0,bytes=0;
	//int width=0,height=0;
	//hsize_t dimValue[3]={0};
	//m_hdf.GetHdfSdsParam("F:\\temp\\HDF\\FY3C_MERSI_GBAL_L1_20150526_2340_GEOQK_MS.HDF",
	//	"Longitude",&dimensions,dimValue,datatype2,bytes);
	//datatype=DataTypeEnum(datatype2);
	//width=dimValue[1];
	//height=dimValue[0];
	//if(datatype==FLOATTYPE)
	//{
	//	assert(sizeof(float)==bytes);
	//	pLatData=new char[width*height*sizeof(float)];
	//	pLonData=new char[width*height*sizeof(float)];
	//	pDataSet=new unsigned short[width*height];
	//	memset(pLatData,0,width*height*sizeof(float));
	//	memset(pLonData,0,width*height*sizeof(float));
	//	memset(pDataSet,0,width*height*sizeof(unsigned short));
	//}
	//m_hdf.ReadHdf5FileData("F:\\temp\\HDF\\FY3C_MERSI_GBAL_L1_20150526_2340_GEOQK_MS.HDF",
	//	"Longitude",pLonData);
	//m_hdf.ReadHdf5FileData("F:\\temp\\HDF\\FY3C_MERSI_GBAL_L1_20150526_2340_GEOQK_MS.HDF",
	//	"Latitude",pLatData);


	//hid_t hFile =H5Fopen("F:\\temp\\HDF\\FY3C_MERSI_GBAL_L1_20150526_2335_0250M_MS.HDF",H5F_ACC_RDONLY,H5P_DEFAULT);
	////hid_t hDSet=H5Dopen(hFile,"/Data/EV_250_RefSB_b1");
	//hid_t hgrp=H5Gopen(hFile,"Data");

	//hid_t hDSet=H5Dopen(hgrp,"EV_250_RefSB_b1");


	//hid_t	hType;
	//hsize_t	hDims[3]={1,1,1};
	//hsize_t	dimsw[3] = { 0 };		// 各维数的大小
	//hid_t	RANK;	
	//hid_t	memspace;
	//hid_t hDsSpace= H5Dget_space(hDSet);
	//hType = H5Dget_type(hDSet);	
	//H5T_class_t hTypeClass = H5Tget_class(hType);
	//size_t hSize = H5Tget_size(hType);
	//int iDataType = UNSHORTTYPE;/*4字节浮点型*/
	//int iNBytes = 2;

	//RANK=H5Sget_simple_extent_ndims(hDsSpace);	//得到数据集维数
	//if(H5Sget_simple_extent_dims(hDsSpace, hDims, dimsw)<0)
	//	return -1;


	//herr_t hStatus = H5Sselect_hyperslab (hDsSpace, H5S_SELECT_SET, NULL, NULL, hDims, NULL);  
	//memspace = H5Screate_simple (RANK, hDims, NULL); 
	//H5Dread(hDSet,H5T_NATIVE_USHORT,memspace,hDsSpace,H5P_DEFAULT,pDataSet);
	//H5Dclose(hDSet);
	//H5Gclose(hgrp);
	//H5Fclose(hFile);

#pragma endregion

#pragma region MyRegion2
	////先获取经度、纬度数据集
	////HDF5Helper::GetDataSetAttr()
	//string file_lat=ana.GetDataSetFile("Latitude");
	//string file_lon=ana.GetDataSetFile("Longitude");
	//string file_data=ana.GetDataSetFile("EV_250_Aggr.1KM_Emissive");
	//if( file_lat.empty()||file_lon.empty()||file_data.empty())
	//	return -1;
	//void *pMyLonData=NULL;//经度
	//void *pMyLatData=NULL;//纬度
	//void *pMyData=NULL;
	//HDF5Helper::GetDataSet(file_lon.c_str(),"Longitude",pMyLonData);
	//HDF5Helper::GetDataSet(file_lat.c_str(),"Latitude",pMyLatData);
	//HDF5Helper::GetDataSet(file_data.c_str(),"EV_250_Aggr.1KM_Emissive",pMyData);
	//Coordinate lat;
	//Coordinate lon;
	//lon.Init(width,height,pLonData,250,4);
	//lon.m_max_degree=170.08118f;
	//lon.m_min_degree=121.095f;
	//lon.Convert();
	//lat.Init(width,height,pLatData,250,4);
	//lat.m_max_degree=-39.669846f;
	//lat.m_min_degree=-62.443058f;
	//lat.Convert();
	//Projector pro(&lon,&lat);
	//pro.Project(pDataSet,2);
	//pro.GenRaw("F:\\temp\\HDF\\pro.raw");
	////
#pragma endregion

	

	return 0;
}