// hdfreader3.cpp : 定义控制台应用程序的入口点。
//

//#include "stdafx.h"
#include "hdf5func.h"
#include "hdfreader3.h"
#include "atlimage.h"
#include "stdio.h"
using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{

	//
	// argv[1] - the output HDF file path name, at the same directory;
	// argv[2] - the directory that each single HDF file resides;
	// argv[3] - the date info string for the current batch (648), each of which should
	//  share the same info
	//

	if ( 4 != argc )
	{
		printf("wrong arguments.\n");
		return -1;
	}

	HDF5Func m_hdf;
	int iDataType = 3; // short, 2 bytes	
	HDF5Func m_outhdf;
	m_outhdf.createOutFile(argv[1]);	
	hid_t hInf = H5Fopen(argv[1], H5F_ACC_RDWR, H5P_DEFAULT);

	//	int *pData = new inta [1000000];
	short *pt  = new short [36000000];
	short **pData= new short *[36];
	for(int i = 0; i < 36; i++)
	{
		pData[i] = new short [1000000];
	}

	int iMemDimSize[2] = {1000,36000};
	int iFileDimSize[2] = {18000,36000};
	int pMemStart[2] = {0,0};
	int pMemCount[2] = {1000,36000};
	int pFileStart[2] = {0,0};
	int pFileCount[2] = {1000,36000};
	CString fname = CString(argv[2]) + CString("\\FY3A_VIRRX_");
	CString bname = CString("_L2_OLR_MLT_GLL_") + CString(argv[3]) + CString("_AOAD_1000M_MS.HDF");
	CString strname;
	bool bCreate = true;
	int k = 0;

	for(int j= '9'; j>='0'; j--)
	{
		k = 0;
		pFileStart[0] = 1000*('8'-j);
		if(j=='9')
			pFileStart[0]= 9000;
		for(int i = 'Z';i>='I';i--)
		{
			char t1 = j;
			char t2 = i;

			strname = fname + t1 +"0"+t2+"0"+bname;
			m_hdf.ReadHdf5FileData(strname,"OLR",pData[k]);
			k++;
		}
		for(int i = '0';i<='9';i++)
		{
			char t1 = j;
			char t2 = i;

			strname = fname + t1 +"0"+t2+"0"+bname;
			m_hdf.ReadHdf5FileData(strname,"OLR",pData[k]);
			k++;
		}
		for(int i = 'A';i<='H';i++)
		{
			char t1 = j;
			char t2 = i;

			strname = fname + t1 +"0"+t2+"0"+bname;
			m_hdf.ReadHdf5FileData(strname,"OLR",pData[k]);
			k++;
		}
		for(int m = 0;m<36000000;m++)
		{
			int c0 = m/36000;
			int c1 = m%36000;
			int c2 = c1%1000;
			int c3 = c1/1000;

			pt[m] = pData[c3][c0*1000+c2];
		}
		m_outhdf.WriteHdf5FileData2D(hInf,"OLR",pt,iDataType,iMemDimSize,iFileDimSize,bCreate,false,pMemStart,pMemCount,pFileStart,pFileCount);

		bCreate = false;
		char b = j;
		cout<<b<<endl;
	}

	for(int j= 'A';j<='H';j++)
	{
		k = 0;
		pFileStart[0] = 10000+1000*(j-'A');

		for(int i = 'Z';i>='I';i--)
		{
			char t1 = j;
			char t2 = i;

			strname = fname + t1 +"0"+t2+"0"+bname;
			m_hdf.ReadHdf5FileData(strname,"OLR",pData[k]);
			k++;
		}
		for(int i = '0';i<='9';i++)
		{
			char t1 = j;
			char t2 = i;

			strname = fname + t1 +"0"+t2+"0"+bname;
			m_hdf.ReadHdf5FileData(strname,"OLR",pData[k]);
			k++;
		}
		for(int i = 'A';i<='H';i++)
		{
			char t1 = j;
			char t2 = i;

			strname = fname + t1 +"0"+t2+"0"+bname;
			m_hdf.ReadHdf5FileData(strname,"OLR",pData[k]);
			k++;
		}
		for(int m = 0;m<36000000;m++)
		{
			int c0 = m/36000;
			int c1 = m%36000;
			int c2 = c1%1000;
			int c3 = c1/1000;

			pt[m] = pData[c3][c0*1000+c2];
		}
		m_outhdf.WriteHdf5FileData2D(hInf,"OLR",pt,iDataType,iMemDimSize,iFileDimSize,bCreate,false,pMemStart,pMemCount,pFileStart,pFileCount);

		bCreate = false;
		char b = j;
		cout<<b<<endl;
	}



	//	for(int i ='Z';i>='I';i--)
	//	{
	//		pFileStart[1]= 1000*('Z'-i);
	////		iFileDimSize[1]= 1000*('Z'-i+1);
	//		int k= 0;
	//		for(int j='8';j>='0';j--)
	//		{
	//			//pFileStart[0] = 1000*('8'-j);
	//			//pFileStart[1] = 1000*('Z'-i);
	//			char t1 = j;
	//			char t2 = i;
	//
	//			strname = fname + t1 +"0"+t2+"0"+bname;
	//			m_hdf.ReadHdf5FileData(strname,"OLR",pData[k]);
	//			k++;
	//			//cout<<"拼接"<<strname<<"...";
	//
	//			//m_outhdf.WriteHdf5FileData2D(hInf,"OLR",pData,iDataType,iMemDimSize,iFileDimSize,bCreate,false,pMemStart,pMemCount,pFileStart,pFileCount);
	//			//if(bCreate)
	//			//bCreate = false;
	//			//cout<<"	完成"<<endl;
	//		}
	//		//pFileStart[0] = 9000;
	//		//pFileStart[1] = 1000*('Z'-i);
	//		char t1 = '9';
	//		char t2 = i;
	//
	//		strname = fname + t1 +"0"+t2+"0"+bname;
	//		m_hdf.ReadHdf5FileData(strname,"OLR",pData[k]);
	//		k++;
	//		//cout<<"拼接"<<strname<<"...";
	//
	//		//m_outhdf.WriteHdf5FileData2D(hInf,"OLR",pData,iDataType,iMemDimSize,iFileDimSize,bCreate,false,pMemStart,pMemCount,pFileStart,pFileCount);
	//		//if(bCreate)
	//		//	bCreate = false;
	//		//cout<<"	完成"<<endl;
	//		for(int j='A';j<='H';j++)
	//		{
	//			//pFileStart[0] = 10000+1000*(j-'A');
	//			//pFileStart[1] = 1000*('Z'-i);
	//			char t1 = j;
	//			char t2 = i;
	//
	//			strname = fname + t1 +"0"+t2+"0"+bname;
	//			m_hdf.ReadHdf5FileData(strname,"OLR",pData[k]);
	//			k++;
	//			//cout<<"拼接"<<strname<<"...";
	//
	//			//m_outhdf.WriteHdf5FileData2D(hInf,"OLR",pData,iDataType,iMemDimSize,iFileDimSize,bCreate,false,pMemStart,pMemCount,pFileStart,pFileCount);
	//			//if(bCreate)
	//			//bCreate = false;
	//			//cout<<"	完成"<<endl;
	//		}
	//		for(int m = 0 ;m<18000000;m++)
	//		{
	//			int c1 = m/1000000;
	//			int c2 = m%1000000;
	//			pt[m] = pData[c1][c2];
	//		}
	//		//pFile
	//		m_outhdf.WriteHdf5FileData2D(hInf,"OLR",pt,iDataType,iMemDimSize,iFileDimSize,bCreate,false,pMemStart,pMemCount,pFileStart,pFileCount);
	//		bCreate = false;
	//	//	break;
	//		cout<<i<<endl;
	//
	//			
	//	}

	//	for(int i ='0';i<='9';i++)
	//	{
	//		pFileStart[1]= 18000+1000*(i-'0');
	//		int k=0;
	//		for(int j='8';j>='0';j--)
	//		{
	////			pFileStart[0] = 1000*('8'-j);
	////			pFileStart[1] = 18000+1000*(i-'0');
	//			char t1 = j;
	//			char t2 = i;
	//
	//			strname = fname + t1 +"0"+t2+"0"+bname;
	//			m_hdf.ReadHdf5FileData(strname,"OLR",pData[k]);
	//			k++;
	//			//cout<<"拼接"<<strname<<"...";
	//
	//			//m_outhdf.WriteHdf5FileData2D(hInf,"OLR",pData,iDataType,iMemDimSize,iFileDimSize,bCreate,false,pMemStart,pMemCount,pFileStart,pFileCount);
	//			//if(bCreate)
	//			//bCreate = false;
	//			//cout<<"	完成"<<endl;
	//		}
	//		//pFileStart[0] = 9000;
	//		//pFileStart[1] = 18000+1000*(i-'0');
	//		char t1 = '9';
	//		char t2 = i;
	//
	//		strname = fname + t1 +"0"+t2+"0"+bname;
	//		m_hdf.ReadHdf5FileData(strname,"OLR",pData[k]);
	//		k++;
	//		//cout<<"拼接"<<strname<<"...";
	//
	//		//m_outhdf.WriteHdf5FileData2D(hInf,"OLR",pData,iDataType,iMemDimSize,iFileDimSize,bCreate,false,pMemStart,pMemCount,pFileStart,pFileCount);
	//		//if(bCreate)
	//		//	bCreate = false;
	//		//cout<<"	完成"<<endl;
	//		for(int j='A';j<='H';j++)
	//		{
	//			//pFileStart[0] = 10000+1000*(j-'A');
	//			//pFileStart[1] = 18000+1000*(i-'0');
	//			char t1 = j;
	//			char t2 = i;
	//
	//			strname = fname + t1 +"0"+t2+"0"+bname;
	//			m_hdf.ReadHdf5FileData(strname,"OLR",pData[k]);
	//			k++;
	//			//cout<<"拼接"<<strname<<"...";
	//
	//			//m_outhdf.WriteHdf5FileData2D(hInf,"OLR",pData,iDataType,iMemDimSize,iFileDimSize,bCreate,false,pMemStart,pMemCount,pFileStart,pFileCount);
	//			//if(bCreate)
	//			//bCreate = false;
	//			//cout<<"	完成"<<endl;
	//		}
	//		for(int m = 0 ;m<18000000;m++)
	//		{
	//			int c1 = m/1000000;
	//			int c2 = m%1000000;
	//			pt[m] = pData[c1][c2];
	//		}
	//
	//		m_outhdf.WriteHdf5FileData2D(hInf,"OLR",pt,iDataType,iMemDimSize,iFileDimSize,bCreate,false,pMemStart,pMemCount,pFileStart,pFileCount);
	//		bCreate = false;
	//		cout<<i<<endl;
	//			
	//	}
	//
	//	for(int i ='A';i<='H';i++)
	//	{
	//		int k = 0;
	//		pFileStart[1] = 28000+1000*(i-'A');
	//		for(int j='8';j>='0';j--)
	//		{
	//			//pFileStart[0] = 1000*('8'-j);
	//			//pFileStart[1] = 28000+1000*(i-'A');
	//			char t1 = j;
	//			char t2 = i;
	//
	//			strname = fname + t1 +"0"+t2+"0"+bname;
	//			m_hdf.ReadHdf5FileData(strname,"OLR",pData[k]);
	//			k++;
	//			//cout<<"拼接"<<strname<<"...";
	//
	//			//m_outhdf.WriteHdf5FileData2D(hInf,"OLR",pData,iDataType,iMemDimSize,iFileDimSize,bCreate,false,pMemStart,pMemCount,pFileStart,pFileCount);
	//			//if(bCreate)
	//			//bCreate = false;
	//			//cout<<"	完成"<<endl;
	//		}
	//		//pFileStart[0] = 9000;
	//		//pFileStart[1] = 28000+1000*(i-'A');
	//		char t1 = '9';
	//		char t2 = i;
	//
	//		strname = fname + t1 +"0"+t2+"0"+bname;
	//		m_hdf.ReadHdf5FileData(strname,"OLR",pData[k]);
	//		k++;
	//		//cout<<"拼接"<<strname<<"...";
	//
	//		//m_outhdf.WriteHdf5FileData2D(hInf,"OLR",pData,iDataType,iMemDimSize,iFileDimSize,bCreate,false,pMemStart,pMemCount,pFileStart,pFileCount);
	//		//if(bCreate)
	//		//	bCreate = false;
	//		//cout<<"	完成"<<endl;
	//		for(int j='A';j<='H';j++)
	//		{
	//			//pFileStart[0] = 10000+1000*(j-'A');
	//			//pFileStart[1] = 28000+1000*(i-'A');
	//			char t1 = j;
	//			char t2 = i;
	//
	//			strname = fname + t1 +"0"+t2+"0"+bname;
	//			m_hdf.ReadHdf5FileData(strname,"OLR",pData[k]);
	//			k++;
	//			//cout<<"拼接"<<strname<<"...";
	//
	//			//m_outhdf.WriteHdf5FileData2D(hInf,"OLR",pData,iDataType,iMemDimSize,iFileDimSize,bCreate,false,pMemStart,pMemCount,pFileStart,pFileCount);
	//			//if(bCreate)
	//			//bCreate = false;
	//			//cout<<"	完成"<<endl;
	//		}
	//		for(int m = 0 ;m<18000000;m++)
	//		{
	//			int c1 = m/1000000;
	//			int c2 = m%1000000;
	//			pt[m] = pData[c1][c2];
	//		}
	//
	//		m_outhdf.WriteHdf5FileData2D(hInf,"OLR",pt,iDataType,iMemDimSize,iFileDimSize,bCreate,false,pMemStart,pMemCount,pFileStart,pFileCount);
	//		bCreate = false;
	//		cout<<i<<endl;
	//			
	//	}







	//m_outhdf.WriteHdf5FileData2D(hInf,"OLR",pData,iDataType,iMemDimSize,iFileDimSize,true,false,pMemStart,pMemCount,pFileStart,pFileCount);

	//pFileStart[1] = 1000;

	//m_hdf.ReadHdf5FileData("D:\\20110715\\FY3A_VIRRX_0010_L2_OLR_MLT_GLL_20110715_AOAD_1000M_MS.HDF","OLR",pData);
	//m_outhdf.WriteHdf5FileData2D(hInf,"OLR",pData,iDataType,iMemDimSize,iFileDimSize,false,false,pMemStart,pMemCount,pFileStart,pFileCount);
	delete pData;
	delete pt;

	return 0;
}
