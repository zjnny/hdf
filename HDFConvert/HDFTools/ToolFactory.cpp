#include "ToolFactory.h"
#include "FuncObject.h"
#include "FuncRaw.h"
#include "FuncTif.h"
#include <iostream>
using namespace HDFTools;
FuncObject* ToolFactory::Concrete(TOOL_COMMAND cmd)
{
	FuncObject *func=NULL;
	switch (cmd)
	{
	case TOOL_NONE:
		func=new FuncObject();
		break;
	case TOOL_GEN_HDF:
		//func=new FuncHDF();
		break;
	case TOOL_GEN_RAW:
		func=new FuncRaw();
		break;
	case TOOL_GEN_TIF:
		break;
	case TOOL_GEN_GEOTIFF:
		func=new FuncTif();
		break;
	default:
		func=new FuncObject();
		break;
	}
	if(func==NULL)
		func=new FuncObject;
	return func;
}