/*
*命令行分析与分解，为其他操作提供参数
//命令行示例
//-o outfile1 -i file1 file2 file3 -d dset1 dset2 dset3 -j Longitude -w Latitude
//当经纬度集名称为Longitude 和 Latitude时可以不必指定-j和-w
//-o outfile1 -i file1 file2 file3 -d dset1 dset2 dset3
//例子：   exe -i F:\\temp\\in.hdf -o F:\\temp\\out.hdf -f  F:\\temp\\1.hdf F:\\temp\\2.hdf -d EV_1KM_RefSB  EV_1KM_RefSB2
*/
#ifndef FuncAnalysis_h_
#define FuncAnalysis_h_
#include <string>
#include <vector>
#include <map>

	struct DataSetAttr
	{
		int dimensions;
		int dimvalue[3];
		unsigned char bytes;
	};
	typedef std::map<std::string,std::vector<std::string> > CMD_Map;//分析commandline的结构<-x>-<param>
	typedef std::map<std::string,std::string> DSet_Map;//分析文件数据集对应关系的结构<dataset>-<filename>
	class FuncAnalysis
	{
	public:
		FuncAnalysis();
		~FuncAnalysis();
		int Execute();
	protected:
		int Analysis();
		int TravelSrcFile(std::string &dset);
		void AnalysisDataSet();
		void SpiltCmd();
		std::string GetDefaultOutputFile();
	public:
		void SetParam(int argc,char **argv);
		std::string GetDataSetFile(const char * dset);
		void GetDataSetGroupPath(const char * dset,std::vector<std::string> &vt);
		std::string GetDataSetGroupPath(const char * dset);
		void GetConvertedDataSet(std::vector<std::string> &vt);
		std::string GetOutputFile();

	private:
		CMD_Map m_cmdmap;
		DSet_Map m_dsetmap;
		CMD_Map m_groupmap;
		int m_argc;
		char** m_argv;
	};

#endif