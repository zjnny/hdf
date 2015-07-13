#include "FuncHDF.h"
#include "stdio.h"
#include "hdf5.h"
#include "../Project/DataSet.h"
#include "HDF5Helper.h"
#include <iostream>
#include <string>
using namespace std;
using namespace HDFTools;
FuncHDF::FuncHDF()
{
	m_pDataSet=NULL;
}
FuncHDF::~FuncHDF()
{
}

void FuncHDF::SetParam(void * lparam,void *rparam)
{
	m_pDataSet=(DataSet*)lparam;
	m_outfile=*(string*)rparam;
}
int FuncHDF::Execute()
{
	Data2HDF();
	return 0;
}
void FuncHDF::Data2HDF()
{
	int dimens=m_pDataSet->GetDimensions();
	int x=m_pDataSet->GetX();
	int y=m_pDataSet->GetY();
	int z=m_pDataSet->GetZ();

	hsize_t hval[3]={1,1,1};
	if(dimens==2)
	{
		hval[0]=m_pDataSet->GetY();
		hval[1]=m_pDataSet->GetX();
	}
	else if(dimens==3)
	{
		hval[0]=m_pDataSet->GetZ();
		hval[1]=m_pDataSet->GetY();
		hval[2]=m_pDataSet->GetX();
	}

	HDF5Helper::SaveDataSet(m_outfile.c_str(),m_pDataSet->GetDataSetName(),m_pDataSet->GetData(),dimens,hval);
	HDF5Helper::SetAttrInfo(m_outfile.c_str(),m_pDataSet->GetDataSetName().c_str(),*(m_pDataSet->GetAttr()));
}
