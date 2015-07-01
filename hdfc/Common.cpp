#include "stdafx.h"
#include "common.h"
#include "string.h"
/*:****************************  xxx    _IsObjExist    xxx*******************************/
/*  ������ƣ�_IsObjExist								*/
/*  ������⣺�м亯��?								*/
/*  �����ܣ��������Ƿ����?							*/
/*  ���÷�����status = int _IsObjExist(hid_t hFileID,char *strSDSName)			*/
/*  ������			I/O	��������					*/
/*   hFileID			����	HDF5�ļ���ʶ��					*/
/*   strSDSName			����	��ݼ����					*/
/*   status			���?����ֵ0��-1					*/
/*  ��    �ߣ��ﰲ��  ��Ӣ��								*/
/*  ������ڣ�?004��4��28��								*/
/***************************  xxxxxx  �������?xxxxx **********************************:*/
int _IsObjExist(hid_t hFileID,char *strSDSName)
{
	hid_t status=0;
    	status =H5Giterate(hFileID, "/", NULL, _CheckFC, strSDSName);
	return status;
}
int _CheckFC(int loc_id, const char *name, void *strname)
{
	H5G_stat_t statbuf;

	H5Gget_objinfo((hid_t)loc_id, name, 0, &statbuf);
	if(strcmp(name,(char *)strname)==0)
		return 1;
	else
		return 0;
}

/*:****************************  xxx    _IsAttrExist    xxx******************************/
/*  ������ƣ�_IsAttrExist								*/
/*  ������⣺�м亯��?								*/
/*  �����ܣ��������Ƿ����?							*/
/*  ���÷�����status = int _IsAttrExist(hid_t hFileID,char *strSDSName)			*/
/*  ������			I/O	��������					*/
/*   hFileID			����	HDF5�ļ���ʶ��					*/
/*   strSDSName		����	��ݼ����					*/
/*   status			���?����ֵ0��-1					*/
/*  ��    �ߣ��ﰲ��  ��Ӣ��								*/
/*  ������ڣ�?004��4��28��								*/
/***************************  xxxxxx  �������?xxxxx **********************************:*/
int _IsAttrExist(hid_t hFileID,char *strAttrName)
{
	hid_t status=0;
    	status =H5Giterate(hFileID, "/", NULL, _CheckAttrFC, strAttrName);
	return status;
}
int _CheckAttrFC(int loc_id, const char *name, void *strname)
{
	int iTempValue;
	H5G_stat_t statbuf;
	hid_t hDatasetID;

	H5Gget_objinfo((hid_t)loc_id, name, 0, &statbuf);
	hDatasetID = H5Dopen(loc_id, name);
	iTempValue=H5Aiterate(hDatasetID,NULL, _GetValue, strname );
	H5Dclose(hDatasetID);
	return iTempValue;
}

int _GetValue(hid_t loc_id, const char *attr_name, void *strname)
{
	int iQualityIndex;
	hid_t	hAttributeID;
	if(strcmp(attr_name,(char *)strname)==0)
	{
		hAttributeID   = H5Aopen_name(loc_id, attr_name);
		H5Aread(hAttributeID, H5T_NATIVE_INT, &iQualityIndex);
		H5Aclose(hAttributeID);

		return iQualityIndex;
	}
	else
		return 0;
}

int _access2(const char *strPath, int iMode)
{
	iMode = 0;
	FILE *fp = fopen(strPath, "rb");
	if(fp!=NULL)
		iMode = 0;
	else
		iMode = -1;
	fclose(fp);
	return iMode;
}

/************************************************************************/
/*������ƣ�_iRMRightBlanks						*/
/*���ܽ��ܣ�(���?�ַ�����Ŀո�?				*/
/*���ø�ʽ��int _iRMRightBlanks(char *pstring)				*/
/*����˵����pstring:�����ַ�						*/
/*����˵���� 0 -- �ɹ�							*/
/*          -1 -- ʧ��							*/
/************************************************************************/
int _iRightBlanks(const char *pstring)
{
	int i=0;
	if (pstring==NULL||strlen(pstring)==0)
		return -1;
	for (i=strlen(pstring)-1;i>=0;i--)
	{
		if (pstring[i]==' ')
			return -1;
	}
        return 0;
}

hid_t getHdfDataType(int iDataType)
{
	if (iDataType<0 || iDataType>= MAXTYPENUM)
	{
		return -1;
	}

	hid_t hType;
	switch(iDataType)
	{
		case UNSHORTTYPE:
			hType = H5T_NATIVE_USHORT;
			break;
		case UNINTTYPE:
			hType = H5T_NATIVE_UINT;
			break;
		case UNCHARTYPE:
			hType = H5T_NATIVE_UCHAR;
			break;
		case CHARTYPE:
			hType = H5T_NATIVE_CHAR;
			break;
		case SHORTTYPE:
			hType = H5T_NATIVE_SHORT;
			break;
		case INTTYPE:
			hType = H5T_NATIVE_INT;
			break;
		case FLOATTYPE:
			hType = H5T_NATIVE_FLOAT;
			break;
		case UNINT64TYPE:
			hType = H5T_NATIVE_UINT64;
			break;
		case INT64TYPE:
			hType = H5T_NATIVE_INT64;
			break;
		case DOUBLETYPE:
			hType = H5T_NATIVE_DOUBLE;
			break;
		default:
			return -1;
			break;
	}

	return hType;
}

bool getDataType(hid_t hType,int &iDataType,int &iNBytes)
{
	if(hType < 0)
	{
		return false;
	}
	else
	{
		/*�õ�����������־��*/
		H5T_class_t hTypeClass = H5Tget_class(hType);
		if(hTypeClass == H5T_NO_CLASS)/*������ʹ���*/
		{
			return false;
		}
		if(hTypeClass == H5T_INTEGER) /*����ʱ*/
		{
			/*�õ���������ֽڴ��*/
			size_t hSize = H5Tget_size(hType);
			if(hSize == 0)/*����ֽ��?*/
			{
				return false;
			}

			/*�õ�����Ƿ��з��*/
			H5T_sign_t hSign = H5Tget_sign(hType);
			if(hSign == H5T_SGN_ERROR)/*����Ŵ���*/
			{
				return false;
			}
			if(hSize == 1 && hSign == H5T_SGN_NONE)
			{
				iDataType = UNCHARTYPE;/*1�ֽ��޷������*/
				iNBytes = 1;
			}
			if(hSize == 1 && hSign == H5T_SGN_2)
			{
				iDataType = CHARTYPE;/*1�ֽ��з������*/
				iNBytes = 1;
			}
			if(hSize == 2 && hSign == H5T_SGN_NONE)
			{
				iDataType = UNSHORTTYPE;/*2�ֽ��޷������*/
				iNBytes = 2;
			}
			if(hSize == 2 && hSign == H5T_SGN_2)
			{
				iDataType = SHORTTYPE;/*2�ֽ��з������*/
				iNBytes = 2;
			}
			if(hSize == 4 && hSign == H5T_SGN_NONE)
			{
				iDataType = UNINTTYPE;/*4�ֽ��޷������*/
				iNBytes = 4;
			}
			if(hSize == 4 && hSign == H5T_SGN_2)
			{
				iDataType = INTTYPE;/*4�ֽ��з������*/
				iNBytes = 4;
			}
			if(hSize == 8 && hSign == H5T_SGN_NONE)
			{
				iDataType = UNINT64TYPE;/*8�ֽ��޷������*/
				iNBytes = 8;
			}
			if(hSize == 8 && hSign == H5T_SGN_2)
			{
				iDataType = INT64TYPE;/*8�ֽ��з������*/
				iNBytes = 8;
			}
		}
		else if(hTypeClass == H5T_FLOAT)/*������ʱ*/
		{
			/*�õ���������ֽڴ��*/
			size_t hSize = H5Tget_size(hType);
			if(hSize == 0)/*����ֽ��?*/
			{
				return false;
			}
			if(hSize == 4)
			{
				iDataType = FLOATTYPE;/*4�ֽڸ�����*/
				iNBytes = 4;
			}
			else
			{
				iDataType = DOUBLETYPE;/*8�ֽ�˫������*/
				iNBytes = 8;
			}
		}
		else if(hTypeClass == H5T_STRING)/*������ʱ*/
		{
			iDataType = STRINGTYPE;
			iNBytes = 1;
		}
		else/*���������Ͳ�������*/
		{
			return false;
		}
	}

	return true;
}
int getDataTypeOfBytes(int iDataType)
{
	int iNBytes = -1;
	if (iDataType<0 || iDataType>=MAXTYPENUM)
	{
		return -1;
	}

	switch(iDataType) {
	case CHARTYPE:
	case UNCHARTYPE:
		iNBytes = 1;
		break;
	case UNSHORTTYPE:
	case SHORTTYPE:
		iNBytes = 2;
		break;
	case INTTYPE:
	case UNINTTYPE:
		iNBytes = 4;
	default:
		iNBytes = -1;
	}

	return iNBytes;
}

int nearest_fill_unshort(unsigned short *pData, int width, int height, unsigned short fill_value, int nloop, int slide_window_width)
{
#ifndef NOT_DEBUG
	cout << "Process Fill Nearest Unshort!" << endl;
#endif
	int rtn=0;
	int i = 0;
	int j = 0;
	int k = 0;
	int window_row = 0;
	int window_col = 0;
	unsigned int data_row = 0;
	unsigned int data_col = 0;
	unsigned int window_index = 0;	
	unsigned int data_index = 0;
	vector <float> coeff;
	vector <float> distance;
	vector <float> window_data;
	float distance_sum = 0.0;
	for(k=0; k<nloop; k++) 
	{
#ifndef NOT_DEBUG
		cout << "Filling Loop " << k << " !" << endl;
#endif
		//�û������ڷ������Ȩƽ��ֵ�����в�ֵ
// 		for(data_row = 0; data_row < height; ++data_row)
// 		{
// 			for(data_col = 0; data_col < width; ++data_col)
// 			{
// 				//���������±�
// 				data_index = data_row * width + data_col;
// 				//����ֵ�Ĳ�����ֵ
// 				if (pData[data_index] != 0)
// 				{
// 					continue;
// 				}
// 				//���㻬�������������е���ʼ�к�
// 				window_row = data_row - (slide_window_width/2);
// 				int obs_row_temp = window_row;	
// 				//˫��ѭ����ɨ���������Χslide_window_width��Χ�ڵ��������е�ÿһ����
// 				for(; window_row < obs_row_temp + slide_window_width; ++window_row)
// 				{
// 					
// 					//�޳���Ч����
// 					if(window_row < 0 || window_row >= height)
// 					{
// 						continue;
// 					}
// 					//���㻬�������ڵ��������е���ʼ�к�
// 					window_col = data_col - (slide_window_width/2);
// 					int obs_col_temp = window_col;
// 					for(; window_col < obs_col_temp + slide_window_width; ++window_col)
// 					{					
// 						//�޳���Ч����
// 						if(window_col < 0 || window_col >= width)
// 						{
// 							continue;
// 						}
// 						//���㴰�������±�
// 						window_index = window_row * width + window_col;
// 						//�޳���Чֵ
// 						if(pData[window_index] == 0)
// 						{
// 							continue;
// 						}
// 						else
// 						{	 
// 							int temp_1= abs((int)(data_col - window_col));
// 							int temp_2 = abs((int)(data_row - window_row));
// 							int temp_distance = temp_1*temp_1 +temp_2*temp_2;
// 							if(temp_distance != 0)
// 							{
// 								distance.push_back(1.0/temp_distance);
// 								window_data.push_back(pData[window_index]);
// 							}
// 							else
// 							{
// 								continue;
// 							}
// 							
// 						}
// 						
// 					}
// 				}
// 				if(distance.size() > 0)
// 				{
// 					for(i = 0; i < distance.size(); ++i)
// 					{
// 						distance_sum += distance[i];
// 					}
// 					for(i = 0; i < distance.size(); ++i)
// 					{
// 						coeff.push_back(distance[i]/distance_sum);
// 					}
// 					float IDDI_temp_zero = 0.0;
// 					for(i = 0; i < distance.size(); ++i)
// 					{
// 						IDDI_temp_zero += coeff[i] * window_data[i];
// 					}
// 					pData[data_index] = IDDI_temp_zero;
// 				}
// 				//��������
// 				distance_sum = 0;
// 				distance.clear();
// 				window_data.clear();
// 				coeff.clear();
// 			}
// 		}

//�ٽ�ֵ��ֵ��
	{

// 		// Upward
		for(i=height-1; i>0; i--) {
			for(j=0; j<width; j++) {				
				if(pData[i*width+j]==fill_value && pData[(i-1)*width+j]!=fill_value) {
					pData[i*width+j] = pData[(i-1)*width+j];
				}
			}
		}
		
		// Rightward
		for(i=0; i<height; i++) {
			for(j=0; j<width-1; j++) {			
				if(pData[i*width+j]==fill_value && pData[i*width+j+1]!=fill_value) {
					pData[i*width+j] = pData[i*width+j+1];
				}
			}
		}
		
		// Downward
		for(i=0; i<height-1; i++) {
			for(j=0; j<width; j++) {			
				if(pData[i*width+j]==fill_value && pData[(i+1)*width+j]!=fill_value) {
					pData[i*width+j] = pData[(i+1)*width+j];
				}
			}
		}
		
		// Leftward
		for(i=0; i<height; i++) {
			for(j=width-1; j>0; j--) {			
				if(pData[i*width+j]==fill_value && pData[i*width+j-1]!=fill_value) {
					pData[i*width+j] = pData[i*width+j-1];
				}
			}
		}
	}

	}
#ifndef NOT_DEBUG
	cout << "Fill Nearest Value Success!" << endl;
#endif
	return SUCCESS;
}

void write_log(string run_error, string file_name, int line)
{
	time_t current_time = time(NULL);
	string log_file_name;
	char month[8] = {0};
	char date[16] = {0};
	char current_exe_file_name[MAX_PATH] = {0};
	strftime(date,  sizeof(date), "%Y%m%d",localtime(&current_time));
	strftime(month, sizeof(month), "%Y%m", localtime(&current_time));
	GetModuleFileName(NULL, current_exe_file_name, MAX_PATH);
	log_file_name = current_exe_file_name;
	int log_file_name_index = log_file_name.rfind("\\");
	log_file_name = log_file_name.substr(0, log_file_name_index+1);
	log_file_name = log_file_name + "log\\" + month + "\\";
	system(("mkdir " + log_file_name).c_str());
	log_file_name = log_file_name + date + ".log";
	string temp_mutex = log_file_name;
	for (string::iterator str_itr=temp_mutex.begin(); str_itr < temp_mutex.end(); ++str_itr)
	{
		if (*str_itr == '\\')
		{
			*str_itr = '_';
		}
	}
	HANDLE hMutex = CreateMutex(NULL, FALSE, temp_mutex.c_str());
	DWORD dw = WaitForSingleObject(hMutex, INFINITE);
	fstream log_file_out(log_file_name.c_str(), ios::out|ios::app);
	log_file_out << "------------------------------------------------------" << endl;
	log_file_out << run_error << endl;
	log_file_out << "Error File Name ====> " << file_name << endl;
	log_file_out << "Error Line ====> " << line << endl;
	log_file_out.close();
	ReleaseMutex(hMutex);
	CloseHandle(hMutex);
}