#ifndef HDF5Helper_h_
#define HDF5Helper_h_
#include <vector>
#include <string>
#include "hdf5.h"
namespace HDFTools
{
	typedef struct  AttrItem{
		AttrItem()
		{
			strName = "";
			iDataType = iArraySize = iByteLen = -1;
			pData = NULL;
		}

		~AttrItem()
		{
			if (pData!=NULL)
			{
				delete []pData;
				pData = NULL;
			}
		}

		AttrItem(const AttrItem &f)
		{
			if (this == &f) return ; 

			if(f.pData!=NULL)
			{
				//			if (pData!=NULL)
				//			{
				//				delete []pData;
				//			}

				int iLen = f.iByteLen;			
				pData = new char[iLen];			
				memcpy(pData,f.pData,iLen);
			}

			this->iArraySize = f.iArraySize;
			this->iDataType = f.iDataType;
			this->iByteLen = f.iByteLen;
			this->strName = f.strName;
		}

		AttrItem& operator = (const AttrItem &f)
		{
			if (this == &f) return *this; 

			if(f.pData!=NULL)
			{
				delete[] pData;

				int iLen = f.iByteLen;
				pData = new char[iLen];			
				memcpy(pData,f.pData,iLen);
			}

			this->iArraySize = f.iArraySize;
			this->iDataType = f.iDataType;
			this->strName = f.strName;
			this->iByteLen = f.iByteLen;

			return *this;
		}

		std::string strName;		// ������������(Name)
		int iDataType;		// �������͵ı��(Type)
		int iArraySize;		// ���ݼ�������ĸ���
		int iByteLen;		// ���ֽ���
		void *pData;		// ָ�����ݼ�ָ��
	}AttrItem;

	enum DataType
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
	class HDF5Helper
	{
	public:
		//int GetObjNum();
		//�ļ����Ƿ���ڸ����ݼ�
		//�ļ�ȫ·���������ݼ���������·����
		static bool IsDataSetExist(const char* file,const char *dset);
		//bool IsGroupExist(const char *GroupName);
		//int GetDataSet();
		//�ļ�ȫ·���������ݼ�������·���磺��/Data/aa�������ݼ�ά����2or3ά��ÿά�ȵĴ�С���������ͣ����ݴ�С
		static int GetDataSetAttr( const char* file, std::string &dset,int &iDimensions, hsize_t*hDimValue, 
			hid_t &iDataType,size_t &iNBytes);
		static int GetDSAttrInfo(hid_t fd,const char* dsname,std::vector<AttrItem> &vc);
		static int GetDSAttrInfo( const char* file,const char* dsname,std::vector<AttrItem> &vc);
		static int GetFileAttrInfo(hid_t fd,std::vector<AttrItem> &vc);
		static int GetFileAttrInfo(const char* file,std::vector<AttrItem> &vc);
		static int SetFileAttrInfo(hid_t fd,std::vector<AttrItem> &vc);
		static int SetFileAttrInfo(const char* file,std::vector<AttrItem> &vc);


		static void GetFileDayOrNightFLag(const char*file,std::string flagname,std::string &data);
		static bool SetAttrInfo(hid_t hF,const char* strDsName,const std::vector<AttrItem> &vAttrD);
		static bool SetAttrInfo(const char* file,const char* strDsName,const std::vector<AttrItem> &vAttrD);
		static bool GetAttrItem(hid_t fd,const char* dsname,AttrItem &aG);
		static bool GetDataType(hid_t hType,int &iDataType,int &iNBytes);
		//���ݼ�������·���磺��/Data/aa��
		static int GetDataSet(const char* file,std::string &dset,void *pData,hid_t type);
		//bool IsObject();
		static int SaveDataSet(const char *file,std::string &dset,void *pData,int dimens,hsize_t *hDimValue);


		static std::vector<std::string > grouppath;
	protected:
		static int VisitHDFFile(int loc_id, const char *name, void *strname);
		static int VisitHDFGroup(int loc_id, const char *name, void *strname);

	};
}

#endif