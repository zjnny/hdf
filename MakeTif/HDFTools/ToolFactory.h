/************************************************************************/
/*生成工厂，主要提供Data数据转换为文件的功能
	转换成HDF，tif，geotif，raw等
*/
/************************************************************************/
#ifndef ToolFactory_h_
#define ToolFactory_h_

namespace HDFTools
{
	enum TOOL_COMMAND{
		TOOL_NONE=0,
		TOOL_GEN_HDF,
		TOOL_GEN_RAW,//生成raw
		TOOL_GEN_TIF,//生成tiff
		TOOL_GEN_GEOTIFF,//生成geotiff
		TOOL_GEN_JPEG//生成jpg
	};
	class FuncObject;
	class ToolFactory
	{
	public:
		static ToolFactory & GetInstance()
		{
			static ToolFactory fac;
			return fac;
		}
		FuncObject* Concrete(TOOL_COMMAND cmd=TOOL_NONE);
	protected:
		ToolFactory(){}

	};
}

#endif