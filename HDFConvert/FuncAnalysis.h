/*
*�����з�����ֽ⣬Ϊ���������ṩ����
//������ʾ��
//-o outfile1 -i file1 file2 file3 -d dset1 dset2 dset3 -j Longitude -w Latitude
//����γ�ȼ�����ΪLongitude �� Latitudeʱ���Բ���ָ��-j��-w
//-o outfile1 -i file1 file2 file3 -d dset1 dset2 dset3
//���ӣ�   exe -i F:\\temp\\in.hdf -o F:\\temp\\out.hdf -f  F:\\temp\\1.hdf F:\\temp\\2.hdf -d EV_1KM_RefSB  EV_1KM_RefSB2
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
	typedef std::map<std::string,std::vector<std::string> > CMD_Map;//����commandline�Ľṹ<-x>-<param>
	typedef std::map<std::string,std::string> DSet_Map;//�����ļ����ݼ���Ӧ��ϵ�Ľṹ<dataset>-<filename>
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