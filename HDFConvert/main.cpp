#include <iostream>
using namespace std;
#include "xtiffio.h"
#include "geotiffio.h"
#include "hdf5.h"
#include "FuncAnalysis.h"
#include "DataSet.h"
#include "HDFTools/ToolFactory.h"
#include "HDFTools/FuncObject.h"
#include <string>
#include <iostream>
using namespace std;
using namespace HDFTools;
typedef unsigned short ushort;
void ConvertDataset(string &filepath,string &dspath,string &outname);
void ExportFile(DataSet* pDs,string &outname);
//HDF5ת����raw��geotif
//-o out.tif -i xxx.hdf -d ds1 ds2
int main(int argc, char* argv[])
{
	try
	{
		//��������
		FuncAnalysis ana;
		ana.SetParam(argc,argv);
		ana.Execute();
		//����
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
	system("pause");
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
	if(ext=="raw")
		fc=fac.Concrete(TOOL_GEN_RAW);
	else if(ext=="tif")
		fc=fac.Concrete(TOOL_GEN_GEOTIFF);
	else
		fc=fac.Concrete(TOOL_GEN_GEOTIFF);
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