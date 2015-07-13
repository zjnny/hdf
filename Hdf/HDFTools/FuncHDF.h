#ifndef FuncHDF_h_
#define FuncHDF_h_
#include "FuncObject.h"
#include <string>
class DataSet;
namespace HDFTools
{
	
	class FuncHDF:public FuncObject
	{
	public:
		FuncHDF();
		~FuncHDF();
		int Execute();
	public:
		virtual void SetParam(void * lparam,void *rparam);
	protected:
		void Data2HDF();
	private:
		std::string m_outfile;//输出文件全路径名
		DataSet *m_pDataSet;
	};
}

#endif