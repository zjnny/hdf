#ifndef DataSet_h_
#define DataSet_h_
#include <string>
#include "hdf5.h"
#include <vector>
#include "../HDFTools/HDF5Helper.h"
class DataSet
{
	typedef unsigned char uchar;
	typedef unsigned short ushort;
public:
	DataSet();
	~DataSet();
	void Init(void* pData,uchar Dimensions,ushort *DimVal,uchar bytes);
	void InitFromFile(const char* file,std::string grouppath);
	hsize_t GetX()
	{
		//����z*y*x ?
		if(m_Dimensions-1<0)
			return 1;
		return m_DimVal[m_Dimensions-1];
	}
	hsize_t GetY()
	{
		if(m_Dimensions-2<0)
			return 1;
		return m_DimVal[m_Dimensions-2];
	}
	hsize_t GetZ()
	{
		if(m_Dimensions-3<0)
			return 1;
		return m_DimVal[m_Dimensions-3];
	}
	int GetDimensions(){return m_Dimensions;}
	hsize_t* GetDimValPointer(){return m_DimVal;}
	bool IsPlanar();
	void * GetData(){return m_pData;}

	int GetBytes(){return m_Bytes;}
	void SetDim(uchar Dimensions,ushort *DimVal);
	void SetData(void *pData);
	std::string GetDataSetName(){return m_Name;}
	void SetDataSetName(std::string name){m_Name=name;}
	void SetDataSetGroupPath(std::string name){m_GroupPath=name;}
	std::string GetDataSetGroupPath(){return m_GroupPath;}
	void Destroy();
	void SetAttr(std::vector<HDFTools::AttrItem> &vc);
	std::vector<HDFTools::AttrItem>* GetAttr();
	void GetAttrItem(HDFTools::AttrItem &item);
	void SetFileAttr(std::vector<HDFTools::AttrItem> &vc);
	std::vector<HDFTools::AttrItem>* GetFileAttr();
private:
	int m_Dimensions;//�Ǽ�ά�����ݽṹ
	hsize_t m_DimVal[3];//ÿά�ĳ���
	void *m_pData;//��������
	size_t m_Bytes;//ÿ��λ����ռ�õ��ֽ���
	std::string m_Name;//���ݼ�����
	std::string m_GroupPath;
	hid_t m_Type;
	std::vector<HDFTools::AttrItem> m_attr;
	std::vector<HDFTools::AttrItem> m_fileattr;
};
#endif