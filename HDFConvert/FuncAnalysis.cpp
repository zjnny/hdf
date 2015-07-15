#include "FuncAnalysis.h"
#include "HDFTools/HDF5Helper.h"
#include <iostream>
using namespace std;
FuncAnalysis::FuncAnalysis()
{
}
FuncAnalysis::~FuncAnalysis()
{
}
void  FuncAnalysis::SetParam(int argc,char **argv)
{
	m_argc=argc;
	m_argv=argv;
}
void FuncAnalysis::SpiltCmd()
{
	vector<string> clist;
	string str,temp;
	for(int i=1;i<m_argc;++i)
	{
		temp=m_argv[i];
		if(temp.find('-')!=string::npos)
		{
			if(clist.size()!=0)
			{
				if(str.empty())
				{
					cout<<"command line error!"<<endl;
					throw std::exception("command line error");
				}
				m_cmdmap[str]=clist;
			}
			clist.clear();
			str=m_argv[i];
		}
		else
		{
			clist.push_back(m_argv[i]);
		}
	}
	m_cmdmap[str]=clist;
}
int FuncAnalysis::Execute()
{
	return Analysis();
}
int FuncAnalysis::Analysis()
{
	SpiltCmd();
	//经纬度数据集的处理

	AnalysisDataSet();
	return 0;
}
int FuncAnalysis::TravelSrcFile(std::string &dset)
{
	std::vector<std::string > flist;
	flist=m_cmdmap["-i"];
	bool bfind=false;
	for(int i=0;i<flist.size();++i)
	{
		if(HDFTools::HDF5Helper::IsDataSetExist(flist[i].c_str(),dset.c_str()))
		{
			m_dsetmap[dset]=flist[i];
			m_groupmap[dset]=HDFTools::HDF5Helper::grouppath;
			cout<<"Find dataset:"<<dset<<" in file:"<<flist[i]<<endl;
			bfind=true;
			return 0;
		}
	}
	if(!bfind)
		cout<<"Can not find dset:"<<dset<<" in all files!"<<endl;
	return -1;
}
void FuncAnalysis::AnalysisDataSet()
{
	vector<string> dataset;
	dataset=m_cmdmap["-d"];
	if(dataset.size()==0)
	{
		cout<<"can not find dataset"<<endl;
		throw std::exception("for all dataset unimplemented");
	}

	for(int i=0;i<dataset.size();++i)
	{
		//遍历文件查找数据集
		TravelSrcFile(dataset[i]);
	}
}
std::string FuncAnalysis::GetDataSetFile(const char * dset)
{
	std::string filename=m_dsetmap[dset];
	if(filename.empty())
		throw exception("the dataset can not find");
	return filename;
}
void FuncAnalysis::GetDataSetGroupPath(const char * dset,std::vector<std::string> &vt)
{
	vt=m_groupmap[dset];
	if(vt.size()==0)
		throw exception("the dataset can not find");
}
std::string FuncAnalysis::GetDataSetGroupPath(const char * dset)
{
	vector<string> vt=m_groupmap[dset];
	if(vt.size()==0)
		throw exception("the group path can not find");
	string str;
	for(int i=0;i<vt.size();++i)
		str+="/"+vt[i];
	return str;
}
void FuncAnalysis::GetConvertedDataSet(std::vector<std::string> &vt)
{
	//要被转换的数据集
	vector<string> dataset;
	dataset=m_cmdmap["-d"];

	for(int i=0;i<dataset.size();++i)
	{
		/*if(dataset[i]=="Latitude"||dataset[i]=="Longitude")
		continue;
		else*/
			vt.push_back(dataset[i]);
	}
}
std::string FuncAnalysis::GetOutputFile()
{
	vector<string> strlist;
	strlist=m_cmdmap["-o"];
	if(strlist.size()==0)
	{
		return GetDefaultOutputFile();
	}
	else
	{
		return strlist[0];
	}
}
std::string FuncAnalysis::GetDefaultOutputFile()
{
	vector<string> strlist;
	strlist=m_cmdmap["-i"];

	string temp=strlist[0];
	string::size_type pos=temp.rfind('.');
	temp= temp.substr(0,pos);//.exe
	temp+="default.HDF";
	return temp;
}

