#ifndef FuncGeoTif_h_
#define FuncGeoTif_h_
#include "FuncObject.h"
#include <string>
class DataSet;
namespace HDFTools
{
	
	class FuncTif:public FuncObject
	{
	public:
		FuncTif();
		~FuncTif();
		int Execute();
	public:
		virtual void SetParam(void * lparam,void *rparam);
	protected:
		int DataToTiff();
	private:
		std::string m_outfile;//输出文件全路径名
		DataSet *m_pDataSet;
	};
}

#endif