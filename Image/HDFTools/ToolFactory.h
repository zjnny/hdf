/************************************************************************/
/*���ɹ�������Ҫ�ṩData����ת��Ϊ�ļ��Ĺ���
	ת����HDF��tif��geotif��raw��
*/
/************************************************************************/
#ifndef ToolFactory_h_
#define ToolFactory_h_

namespace HDFTools
{
	enum TOOL_COMMAND{
		TOOL_NONE=0,
		TOOL_GEN_HDF,
		TOOL_GEN_RAW,//����raw
		TOOL_GEN_TIF,//����tiff
		TOOL_GEN_GEOTIFF,//����geotiff
		TOOL_GEN_JPEG//����jpg
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