
#include "hdf5.h"
#include "H5Cpp.h"
#ifndef H5_NO_NAMESPACE
using namespace H5;
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
using namespace std;
#define MAXATTR 100
int main()
{
	char * filename="F:\\temp\\misr_am1_metadata.hdf";
	hid_t hInFile = H5Fopen(filename,H5F_ACC_RDONLY,H5P_DEFAULT);
	hsize_t hDsCount;

	//���������ļ��е����ݼ��ĸ���
	herr_t hStatus = H5Gget_num_objs(hInFile, &hDsCount);
	char pAttrTmp[100];
	int iDsCount=(int)hDsCount;	
	int i = 0;
	vector<string> vDsNs;
	for (i=0;i<iDsCount;++i)
	{
		//��ȡ�������ݼ�������
		ssize_t sSizet =  H5Gget_objname_by_idx(hInFile,i,pAttrTmp,100);
		string strDsTmp = pAttrTmp;
		vDsNs.push_back(strDsTmp);
	}

	H5Fclose(hInFile);
	return 0;
}