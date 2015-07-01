#include "ToolFactory.h"
#include "FuncObject.h"
#include <iostream>
using namespace HDFTools;
FuncObject* ToolFactory::Concrete(TOOL_COMMAND cmd)
{
	FuncObject *func=NULL;
	switch (cmd)
	{
	case HDFTools::TOOL_NONE:
		break;
	case HDFTools::TOOL_GEN_RAW:
		break;
	case HDFTools::TOOL_GEN_TIF:
		break;
	case HDFTools::TOOL_GEN_GEOTIFF:
		break;
	default:
		break;
	}
	return func;
}