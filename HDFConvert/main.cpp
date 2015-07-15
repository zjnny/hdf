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
//HDF5转化成raw，geotif
//-o out.tif -i xxx.hdf -d ds1 ds2
int main(int argc, char* argv[])
{
	try
	{
		//分析参数
		FuncAnalysis ana;
		ana.SetParam(argc,argv);
		ana.Execute();
		//处理
		vector<string> vc;
		ana.GetConvertedDataSet(vc);
		//依次处理每一个数据集
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
			//格式转化输出
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
	//获取数据集数据
	DataSet ds;
	ds.InitFromFile(filepath.c_str(),dspath);
	//暂时认为全部是ushort型
	if(ds.GetData()!=NULL)
		ExportFile(&ds,outname);

}
void ExportFile(DataSet* pDs,string &outname)
{
	string outfile=outname;
	string ext=outfile.substr(outfile.rfind('.')+1,outfile.length());
	ext=strlwr((char*)ext.c_str());
	//大小写转换
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