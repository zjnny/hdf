#ifndef ToolFactory_h_
#define ToolFactory_h_

namespace HDFTools
{
	enum TOOL_COMMAND{
		TOOL_NONE=0,
		TOOL_GEN_RAW,//生成raw
		TOOL_GEN_TIF,//生成tiff
		TOOL_GEN_GEOTIFF//生成geotiff
	};
	class FuncObject;
	class ToolFactory
	{
	public:
		ToolFactory & GetInstance()
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