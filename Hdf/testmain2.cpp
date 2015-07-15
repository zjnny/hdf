#include "stdafx.h"
#ifdef HDF_DEBUG
#include "vld.h"
#endif
#include <cassert>
#include "HDFTools/FuncProject.h"
#include "Project/FuncAnalysis.h"
#include "HDFTools/HDF5Helper.h"
#include "Project/Projector.h"
#include "Project/Coordinate.h"
#include "Project/DataSet.h"
#include "Project/Enhance.h"
#include "hdf5func.h"
#include <map>
#include <iomanip>
using namespace HDFTools;
//FY3C_MERSI_GBAL_L1_20150526_2340_GEO1K_MS.HDF
//FY3C_MERSI_GBAL_L1_20150526_2340_GEOQK_MS.HDF
//������ʾ��
//-o outfile1 -i file1 file2 file3 -d dset1 dset2 dset3 -j Longitude -w Latitude
//����γ�ȼ�����ΪLongitude �� Latitudeʱ���Բ���ָ��-j��-w
//-o outfile1 -i file1 file2 file3 -d dset1 dset2 dset3
//���ӣ�   exe -i F:\\temp\\in.hdf -o F:\\temp\\out.hdf -f  F:\\temp\\1.hdf F:\\temp\\2.hdf -d EV_1KM_RefSB  EV_1KM_RefSB2

int _tmain(int argc, _TCHAR* argv[])
{
	double start ,end ,cost;
	start=clock();
	if(argc==1)
	{
		cout<<"������ʾ��:"<<endl;
		cout<<"-o outfile1 -i file1 file2 file3 -d dset1 dset2 dset3 -j Longitude -w Latitude"<<endl;
		cout<<"����γ�ȼ�����ΪLongitude �� Latitudeʱ���Բ���ָ��-j��-w:"<<endl;
		cout<<"-o outfile1 -i file1 file2 file3 -d dset1 dset2 dset3"<<endl;
		return -1;
	}
	for(int i=0;i<argc;++i)
	{
		cout<<argv[i]<<endl;
	}
	int map_size=1000;//Ĭ��1000m����������ͼ�����
	string dsetName_lon="Longitude";//�������ݼ�����
	string dsetName_lat="Latitude";//γ�����ݼ�����
	string tempstr;
	USHORT filled_value=0;
try
{
	
#pragma region Region_Analysis
	FuncAnalysis ana;
	ana.SetParam(argc,argv);
	ana.Execute();
	tempstr=ana.GetLongitudeDSetName();
	if(!tempstr.empty())
		dsetName_lon=tempstr;
	tempstr=ana.GetLatitudeDSetName();
	if(!tempstr.empty())
		dsetName_lat=tempstr;
	map_size=ana.GetMapSize();
#pragma endregion
#pragma region Region_ReadData
	//�ȶ�ȡ��γ����Ϣ
	string lonfile=ana.GetDataSetFile(dsetName_lon.c_str());//�����ļ�·����
	string latfile=ana.GetDataSetFile(dsetName_lat.c_str());
	string longpath=ana.GetDataSetGroupPath(dsetName_lon.c_str());//����hdf�ļ��о�����·����/group/dataset
	string latgpath=ana.GetDataSetGroupPath(dsetName_lat.c_str());
	DataSet data_lon;
	DataSet data_lat;
	data_lon.InitFromFile(lonfile.c_str(),
		longpath);
	data_lat.InitFromFile(latfile.c_str(),
		latgpath);
	Coordinate lat;
	Coordinate lon;
	//��ʱ��Ϊ�������ݼ�����float��
	lon.Init(data_lon.GetX(),data_lon.GetY(),data_lon.GetData(),map_size,sizeof(float));
	lon.Convert();
	//������õ��ڴ�
	data_lon.Destroy();

	lat.Init(data_lat.GetX(),data_lat.GetY(),data_lat.GetData(),map_size,sizeof(float));
	lat.Convert();
	//������õ��ڴ�
	data_lat.Destroy();

	
	
	//
	end=clock();
	cost=end-start;
	cout<<"��ʱ��"<<setprecision(5)<<cost/CLOCKS_PER_SEC<<endl;
	//////////////////////////////////////////////////////////////////////////
	//ͶӰÿһ��ָ�������ݼ�
	vector<string> dslist;
	ana.GetProjectedDataSet(dslist);
	for(int i=0;i<dslist.size();++i)
	{
		string dfilename=ana.GetDataSetFile(dslist[i].c_str());
		string gpathname=ana.GetDataSetGroupPath(dslist[i].c_str());
		DataSet data;
		data.InitFromFile(dfilename.c_str(),
			gpathname);
		//ȷ�����һ��
		assert(lon.GetWidth()==data.GetX());
		assert(lon.GetHeight()==data.GetY());
#pragma region Region_project
		Projector pro(&lon,&lat);

		end=clock();
		cost=end-start;
		cout<<"��ʱ��"<<setprecision(5)<<cost/CLOCKS_PER_SEC<<endl;

		//pro.Project(data.GetData(),2);
		pro.Project(&data);
		end=clock();
		cost=end-start;
		cout<<"��ʱ��"<<setprecision(5)<<cost/CLOCKS_PER_SEC<<endl;
		//��������ڴ�
		data.Destroy();
#pragma region OutPut
		string outfname=ana.GetOutputFile();
		if(outfname.substr(outfname.rfind('.')+1,outfname.length())=="raw")
		{
			outfname=outfname.substr(0,outfname.length()-4);
			outfname+=dslist[i]+".raw";
		}
		
		//pro.Output(ana.GetOutputFile().c_str());
		pro.Output(outfname.c_str());
#pragma endregion
		end=clock();
		cost=end-start;
		cout<<"��ʱ��"<<setprecision(5)<<cost/CLOCKS_PER_SEC<<endl;
		//string ofile=ana.GetOutputFile();
		//ofile=ofile.substr(0,ofile.rfind('\\')+1);
		//ofile+="prd%d.raw";
		//char buf[MAX_PATH];
		//sprintf_s(buf,MAX_PATH,ofile.c_str(),i);
		//pro.GenRaw(buf);
#pragma endregion
	}
	


	////�Ȼ�ȡ���ȡ�γ�����ݼ�
	////HDF5Helper::GetDataSetAttr()
	//string file_lat=ana.GetDataSetFile("Latitude");
	//string file_lon=ana.GetDataSetFile("Longitude");
	//string file_data=ana.GetDataSetFile("EV_250_Aggr.1KM_Emissive");
	//if( file_lat.empty()||file_lon.empty()||file_data.empty())
	//	return -1;
	//void *pMyLonData=NULL;//����
	//void *pMyLatData=NULL;//γ��
	//void *pMyData=NULL;
	//HDF5Helper::GetDataSet(file_lon.c_str(),"Longitude",pMyLonData);
	//HDF5Helper::GetDataSet(file_lat.c_str(),"Latitude",pMyLatData);
	//HDF5Helper::GetDataSet(file_data.c_str(),"EV_250_Aggr.1KM_Emissive",pMyData);


	////

#pragma endregion
}
catch (std::exception * e)
{
	cout<<"Error:"<<e->what()<<endl;
	return -1;
}
catch (...)
{
	cout<<"Some error hanppend!"<<endl;
	return -1;
}

	end=clock();
	cost=end-start;
	cout<<"���պ�ʱ��"<<setprecision(5)<<cost/CLOCKS_PER_SEC<<endl;
	system("pause");
	return 0;
}