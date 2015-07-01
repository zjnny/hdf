#ifndef ToolFactory_h_
#define ToolFactory_h_

namespace HDFTools
{
	enum TOOL_COMMAND{
		TOOL_NONE=0,
		TOOL_GEN_RAW,//����raw
		TOOL_GEN_TIF,//����tiff
		TOOL_GEN_GEOTIFF//����geotiff
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