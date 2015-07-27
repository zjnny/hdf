/*
快速视图生成从dataset中提取3个通道数据转化成rgb图像
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
//HDF5转化成raw，geotif
//-o out.tif -i xxx.hdf -d ds1 ds2 //-b ds1<2,1,0> 或者ds1<0> ds2<1> ds3<2>
int main(int argc, char* argv[])
{
	try
	{
		//分析参数
		Analysis ana;
		ana.SetParam(argc,argv);
		ana.Execute();
		//处理获取-d指定的数据集
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
#ifdef _DEBUG
	system("pause");
#endif
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