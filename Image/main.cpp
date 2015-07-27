/*
������ͼ���ɴ�dataset����ȡ3��ͨ������ת����rgbͼ��
*/
#include <iostream>
using namespace std;

#include "hdf5.h"
#include "HDFTools/Analysis.h"
#include "HDFTools/DataSet.h"
#include "HDFTools/ToolFactory.h"
#include "HDFTools/FuncObject.h"
#include "HDFTools/Enhance.h"
#include <string>
#include <iostream>
using namespace std;
using namespace HDFTools;
typedef unsigned short ushort;
typedef unsigned char uchar;
void ConvertDataset(string &filepath,string &dspath,string &outname);
void ExportFile(DataSet* pDs,string &outname);
//HDF5ת����raw��geotif
//-o out.tif -i xxx.hdf -d ds1 ds2 //-b ds1<2,1,0> ����ds1<0> ds2<1> ds3<2>
int main(int argc, char* argv[])
{
	try
	{
		//��������
		Analysis ana;
		ana.SetParam(argc,argv);
		ana.Execute();
		//�����ȡ-dָ�������ݼ�
		vector<string> vc;
		ana.GetConvertedDataSet(vc);
		//���δ���ÿһ�����ݼ�
		for(int i=0;i<vc.size();++i)
		{
			cout<<"Convert DataSet:"<<vc[i]<<endl;
			string dsFile=ana.GetDataSetFile(vc[i].c_str());
			string dsGroup=ana.GetDataSetGroupPath(vc[i].c_str());
			if(dsFile.empty()||dsGroup.empty())
			{
				cout<<"Can not find dataset:"<<vc[i]<<endl;
				continue;
			}
			//��ʽת�����
			ConvertDataset(dsFile,dsGroup,ana.GetOutputFile());
		}
	}
	catch (std::exception* e)
	{
		cout<<e->what()<<endl;
	}
	catch (...)
	{
		cout<<"Unknow error!"<<endl;
	}
#ifdef _DEBUG
	system("pause");
#endif
	return 0;
}
void ConvertDataset(string &filepath,string &dspath,string &outname)
{
	//��ȡ���ݼ�����
	DataSet ds;
	ds.InitFromFile(filepath.c_str(),dspath);
	//��ʱ��Ϊȫ����ushort��
	if(ds.GetData()!=NULL)
		ExportFile(&ds,outname);

}
void ExportFile(DataSet* pDs,string &outname)
{
	string outfile=outname;
	string ext=outfile.substr(outfile.rfind('.')+1,outfile.length());
	ext=strlwr((char*)ext.c_str());
	//��Сдת��
	ToolFactory &fac=ToolFactory::GetInstance();
	FuncObject *fc=NULL;
	if(ext=="jpg")
		fc=fac.Concrete(TOOL_GEN_JPEG);
	else
		fc=fac.Concrete(TOOL_GEN_JPEG);
	if(fc)
	{
		fc->SetParam((void*)pDs,(void*)&outfile);
		fc->Execute();
		delete fc;
		fc=NULL;
	}
	else
		cout<<"Can not output to file"<<endl;

}