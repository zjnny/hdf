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
		std::string m_outfile;//����ļ�ȫ·����
		DataSet *m_pDataSet;
	};
}

#endif