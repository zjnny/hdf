#ifndef COMMON_H
#define COMMON_H

//引用了所需要的所有标准库或系统级的头文件
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <iostream>
#include <tchar.h>
#include <vector>

#include "Windows.h"
#include <process.h>
#include <fstream>
#include <ctime>
using namespace std;

#include "hdf5.h"
// #ifdef WIN32
// #include "h5lt.h"
// #else
// #include "H5LT.h"
// #endif
#define SUCCESS 0
#define FALSE -1
#define NIGHTDATA -2
typedef unsigned short int usint;
typedef unsigned int uint;

#define L1MERSI0250DsNum 8
#define L1MERSI1000DsNum 19
#define MAX_PATH 512

int _access2(const char *strPath, int iMode);
int _IsObjExist(hid_t hFileID,char *strSDSName);
int _CheckFC(int loc_id, const char *name, void *strname);
int _IsAttrExist(hid_t hFileID,const char *strAttrName);
int _CheckAttrFC(int loc_id, const char *name, void *strname);
int _GetValue(hid_t loc_id, const char *attr_name, void *strname);
int _iRightBlanks(const char *pstring);
int nearest_fill_unshort(unsigned short *pData, int width, int height, unsigned short fill_value, int nloop,int slide_window_width);
void write_log(string run_error, string file_name, int line);

enum DataTypeEnum
{
		UNCHARTYPE = 0,
		CHARTYPE = 1,
		UNSHORTTYPE = 2,
		SHORTTYPE = 3,
		UNINTTYPE = 4,
		INTTYPE = 5,
		UNINT64TYPE = 6,
		INT64TYPE = 7,
		FLOATTYPE = 8,
		DOUBLETYPE = 9,
		STRINGTYPE = 10,
		MAXTYPENUM,
};

hid_t getHdfDataType(int iDataType);
bool getDataType(hid_t hType,int &iDataType,int &iNBytes);
int getDataTypeOfBytes(int iDataType);
#endif