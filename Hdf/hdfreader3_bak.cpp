// hdfreader3.cpp : 定义控制台应用程序的入口点。
//
#include "stdafx.h"
#include "hdf5func.h"
#include "atlimage.h"
#include <cassert>
#include "HDFTools/FuncProject.h"
#include "HDFTools/FuncAnalysis.h"
#include "HDFTools/HDF5Helper.h"
#include <map>
void release();
void * pData=NULL;
//经度信息
void *pLonData=NULL;
//纬度信息
void *pLatData=NULL;
void *pDataSet=NULL;
//FY3C_MERSI_GBAL_L1_20150526_2340_GEO1K_MS.HDF
//FY3C_MERSI_GBAL_L1_20150526_2340_GEOQK_MS.HDF
//-i infile -o outfile -f file1 file2 file3 -d dset1 dset2 dset3
//暂时所有参数必须都要,而且顺序要和下面一致，否则要重写分析command的代码
//例子：   exe -i F:\\temp\\in.hdf -o F:\\temp\\out.hdf -f  F:\\temp\\1.hdf F:\\temp\\2.hdf -d EV_1KM_RefSB  EV_1KM_RefSB2
int _tmain(int argc, _TCHAR* argv[])
{
	//HDFTools::HDF5Helper::IsDataSetExist("F:\\temp\\HDF\\FY3C_MERSI_GBAL_L1_20150526_2340_GEO1K_MS.HDF","DEM");


	HDFTools::FuncAnalysis ana;
	ana.SetParam(argc,argv);
	ana.Execute();

	HDF5Func m_hdf;
	DataTypeEnum datatype=UNCHARTYPE;
	int dimensions=0,datatype2=0,bytes=0;
	int width=0,height=0;
	hsize_t dimValue[3]={0};
	m_hdf.GetHdfSdsParam("F:\\temp\\HDF\\FY3C_MERSI_GBAL_L1_20150526_2340_GEOQK_MS.HDF",
		"Latitude",&dimensions,dimValue,datatype2,bytes);
	datatype=DataTypeEnum(datatype2);
	width=dimValue[1];
	height=dimValue[0];
	if(datatype==FLOATTYPE)
	{
		assert(sizeof(float)==bytes);
		pLatData=new char[width*height*sizeof(float)];
		pLonData=new char[width*height*sizeof(float)];
		pDataSet=new unsigned short[width*height];
		memset(pLatData,0,width*height*sizeof(float));
		memset(pLonData,0,width*height*sizeof(float));
		memset(pDataSet,0,width*height*sizeof(unsigned short));
	}
	m_hdf.ReadHdf5FileData("F:\\temp\\HDF\\FY3C_MERSI_GBAL_L1_20150526_2340_GEOQK_MS.HDF",
		"Longitude",pLonData);
	m_hdf.ReadHdf5FileData("F:\\temp\\HDF\\FY3C_MERSI_GBAL_L1_20150526_2340_GEOQK_MS.HDF",
		"Latitude",pLatData);
	
	
	hid_t hFile =H5Fopen("F:\\temp\\HDF\\FY3C_MERSI_GBAL_L1_20150526_2335_0250M_MS.HDF",H5F_ACC_RDONLY,H5P_DEFAULT);
	//hid_t hDSet=H5Dopen(hFile,"/Data/EV_250_RefSB_b1");
	hid_t hgrp=H5Gopen(hFile,"Data");

	hid_t hDSet=H5Dopen(hgrp,"EV_250_RefSB_b1");


	hid_t	hType;
	hsize_t	hDims[3]={1,1,1};
	hsize_t	dimsw[3] = { 0 };		// 各维数的大小
	hid_t	RANK;	
	hid_t	memspace;
	hid_t hDsSpace= H5Dget_space(hDSet);
		hType = H5Dget_type(hDSet);	
		H5T_class_t hTypeClass = H5Tget_class(hType);
		size_t hSize = H5Tget_size(hType);
		int iDataType = UNSHORTTYPE;/*4字节浮点型*/
		int iNBytes = 2;
		
	RANK=H5Sget_simple_extent_ndims(hDsSpace);	//得到数据集维数
	if(H5Sget_simple_extent_dims(hDsSpace, hDims, dimsw)<0)
		return -1;


	herr_t hStatus = H5Sselect_hyperslab (hDsSpace, H5S_SELECT_SET, NULL, NULL, hDims, NULL);  
	memspace = H5Screate_simple (RANK, hDims, NULL); 
	H5Dread(hDSet,H5T_NATIVE_USHORT,memspace,hDsSpace,H5P_DEFAULT,pDataSet);
	H5Dclose(hDSet);
	H5Gclose(hgrp);
	H5Fclose(hFile);

	HDFTools::FuncProject pro;
	pro.SetParam(pDataSet,(HDFTools::DEGREE_TYPE*)pLonData,(HDFTools::DEGREE_TYPE*)pLatData,width,height,2);
	pro.Execute();
//
//	HDF5Func m_hdf;
//	//hsize_t a2;
//	//int a1,a3,a4;
//	hid_t hInFile = H5Fopen("F:\\temp\\HDF\\FY3C_MERSI_GBAL_L1_20150526_2335_0250M_MS.HDF",H5F_ACC_RDONLY,H5P_DEFAULT);
//// 	hid_t hInFile = H5Fopen("F:\\temp\\FY2G_CLC_MLT_NOM_20150603_0200.hdf",H5F_ACC_RDONLY,H5P_DEFAULT);
//// 		//attrOne attr;
//// 		//attr.strName = "Data Lines";
//	vector<string> vt;
//	m_hdf.getAllDsNames(hInFile,vt);
//	DataTypeEnum datatype=UNCHARTYPE;
//	int dimensions=0,datatype2=0,bytes=0;
//	int width=0,height=0;
//	hsize_t dimValue[3]={0};
//	H5Fclose(hInFile);
//	m_hdf.GetHdfSdsParam("F:\\temp\\HDF\\FY3C_MERSI_GBAL_L1_20150526_2340_GEOQK_MS.HDF",
//		"Latitude",&dimensions,dimValue,datatype2,bytes);
//	
//	if(dimensions==2)
//	{
//		datatype=DataTypeEnum(datatype2);
//		width=dimValue[0];
//		height=dimValue[1];
//		if(datatype==CHARTYPE)
//		{
//			assert(sizeof(char)==bytes);
//			pData=new char[width*height*sizeof(char)];
//			memset(pData,0,width*height*sizeof(char));
//		}
//		else if(datatype==SHORTTYPE)
//		{
//
//		}
//		else if(datatype==FLOATTYPE)
//		{
//			assert(sizeof(float)==bytes);
//			pData=new char[width*height*sizeof(float)];
//			memset(pData,0,width*height*sizeof(float));
//		}
//	}
//	else
//	{
//		cout<<"dimensions==3 unimplement"<<endl;
//	}
//	
//	//从HDF读取源数据
//	m_hdf.ReadHdf5FileData("F:\\temp\\HDF\\FY3C_MERSI_GBAL_L1_20150526_2340_GEOQK_MS.HDF","Latitude",pData);	
//	
//	FILE* fp = fopen("F:\\temp\\test8.raw", "wb");
//	fwrite(pData, width,height, fp);
//	fclose(fp);
//
//
//
//	CImage m_img;
//	m_img.Create(width,height,8);
//	
//	HDC dc = m_img.GetDC();
//	for(int x=0;x<width;x++)
//		for(int y=0;y<height;y++)
//		{
//			unsigned char *data=(unsigned char*)pData;
//			unsigned char px=data[x*width+y];
//			if(px==255)
//				SetPixel(dc,x,y,RGB(px,0,0));
//			else if(px>125&&px<255)
//				SetPixel(dc,x,y,RGB(0,px,0));
//			else
//				SetPixel(dc,x,y,RGB(0,0,px));
//		}
//	
//	//StretchDIBits(dc,0,0,width,height,0,0,width,height,pData,m_img);
//	//memcpy(pbuf,pData,width*height*sizeof(char));
//	//::Rectangle(dc,0,0,999,999);
//	//double t,x,y;
//	//BYTE * buff = (BYTE *) m_img.GetBits();
//	m_img.Save("F:\\temp\\1.bmp");
//	m_img.ReleaseDC();
//	release();
	return 0;

}
void release()
{
	delete []pData;
}

