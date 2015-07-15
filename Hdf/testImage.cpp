#include "stdafx.h"
#ifdef HDF_DEBUG
#include "vld.h"
#endif
#include <cassert>
#include "HDFTools/FuncProject.h"
#include "HDFTools/FuncAnalysis.h"
#include "HDFTools/HDF5Helper.h"
#include "Project/Projector.h"
#include "Project/Coordinate.h"
#include "Project/DataSet.h"
#include "Project/Enhance.h"
#include "hdf5func.h"
#include <map>
using namespace HDFTools;

int _tmain(int argc, _TCHAR* argv[])
{
	const int n=35;
   unsigned short pData[n]=
   {
	0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,
	0,0,1,2,1,0,0,
	0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,
   };
	unsigned short *temp=new unsigned short[n];
	Enhance::Median_Filter2(pData,temp,7,5);
	memcpy(pData,temp,n*sizeof(unsigned short));

	return 0;
}