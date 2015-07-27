#include "HDF5Helper.h"
#include "string.h"
//#include "stdio.h"
#include "hdf5.h"
#include "H5LT.h"
using namespace HDFTools;
std::vector<std::string > HDF5Helper::grouppath=std::vector<std::string >();
 bool HDF5Helper::IsDataSetExist(const char* file,const char *dset)
 {
	 HDF5Helper::grouppath.clear();
	 hid_t status=0;
	 hid_t hInFile = H5Fopen(file,H5F_ACC_RDONLY,H5P_DEFAULT);
	 if(hInFile==-1)
	 {
		 printf("Can not open file:%s\n",file);
		 return false;
	 }
	 status =H5Giterate(hInFile, "/", NULL, VisitHDFFile, (void*)dset);
	 H5Fclose(hInFile);
	 return status;
 }
 int HDF5Helper::VisitHDFFile(int loc_id, const char *name, void *strname)
 {
	 H5G_stat_t statbuf;

	 H5Gget_objinfo((hid_t)loc_id, name, 0, &statbuf);
	if(statbuf.type==H5G_GROUP)
	{

		int res= H5Giterate(loc_id,name,NULL,VisitHDFGroup,strname);
		if(res==1)
		{
			HDF5Helper::grouppath.push_back(name);
			HDF5Helper::grouppath.push_back((char*)strname);
		}
		return res;
	}

	 if(strcmp(name,(char *)strname)==0)
	 {
		grouppath.push_back((char*)strname);
		 return 1;
	 }
	 else
		 return 0;
 }
  int HDF5Helper::VisitHDFGroup(int loc_id, const char *name, void *strname)
  {
	  H5G_stat_t statbuf;
	  H5Gget_objinfo((hid_t)loc_id, name, 0, &statbuf);
	  if(strcmp(name,(char *)strname)==0)
	  {
		  /* hid_t hDset= H5Dopen(loc_id,(char*)strname);
		  if(hDset>=0)
		  {
		  hid_t hSpace= H5Dget_space(hDset);
		  hid_t hType = H5Dget_type(hSpace);	
		  H5T_class_t	hTypeClass = H5Tget_class(hType);
		  size_t hSize = H5Tget_size(hType);
		  H5T_sign_t  hSign= H5Tget_sign(hType);
		  int iDimensions = H5Sget_simple_extent_ndims(hSpace);
		  hsize_t hDimValue[3]={0};
		  hsize_t hMaxDimValue[3]={0};
		  H5Sget_simple_extent_dims(hSpace, hDimValue, hMaxDimValue) ;
		  H5Dclose(hDset);
		  }
		  */
		  return 1;
	  }
	  else
		  return 0;
  }

  int HDF5Helper::GetDataSetAttr( const char* file,  std::string &dset,int &iDimensions, hsize_t*hDimValue, 
	 hid_t &iDataType,size_t &iNBytes)
  {
	  hid_t hFile =H5Fopen(file,H5F_ACC_RDONLY,H5P_DEFAULT);
	  //hid_t hDSet=H5Dopen(hFile,"/Data/EV_250_RefSB_b1");

	  hid_t hDSet=H5Dopen(hFile,dset.c_str());
	  hid_t	memspace;
	  hid_t hDsSpace= H5Dget_space(hDSet);
	  int width=0,height=0;
	  hid_t	RANK;	
	  hDimValue[0]=1;
	  hDimValue[1]=1;
	  hDimValue[2]=1;
	  hsize_t	dimsw[3] = { 0 };	
	 iDimensions=H5Sget_simple_extent_ndims(hDsSpace);
	  if(H5Sget_simple_extent_dims(hDsSpace, hDimValue, dimsw)<0)
	  	return -1;
		hid_t Type= H5Dget_type(hDSet);	
		H5T_class_t hClass = H5Tget_class(Type);
		iNBytes= H5Tget_size(Type);
		H5T_sign_t	hSign = H5Tget_sign(Type);
		if(hClass==H5T_FLOAT)
		{
			if(iNBytes == 4) 
			{
				iDataType = FLOATTYPE;/*4×Ö½Ú¸¡µãÐÍ*/
			}
			else if(iNBytes==8)
			{
				iDataType = DOUBLETYPE;/*8×Ö½ÚË«¾«¶ÈÐÍ*/
			}
			else
			{
				
				//"Unknown Type";
			}
		}
		else if(hClass==H5T_INTEGER)
		{
			if(iNBytes == 1 && hSign == H5T_SGN_NONE) 
			{
				iDataType = UNCHARTYPE;/*1×Ö½ÚÎÞ·ûºÅÕûÐÍ*/
			}
			if(iNBytes == 1 && hSign == H5T_SGN_2)   
			{
				iDataType = CHARTYPE;/*1×Ö½ÚÓÐ·ûºÅÕûÐÍ*/
			}
			if(iNBytes == 2 && hSign == H5T_SGN_NONE) 
			{
				iDataType = UNSHORTTYPE;/*2×Ö½ÚÎÞ·ûºÅÕûÐÍ*/
			}
			if(iNBytes == 2 && hSign == H5T_SGN_2) 
			{
				iDataType = SHORTTYPE;/*2×Ö½ÚÓÐ·ûºÅÕûÐÍ*/
			}
			if(iNBytes == 4 && hSign == H5T_SGN_NONE) 
			{
				iDataType = INTTYPE;/*4×Ö½ÚÎÞ·ûºÅÕûÐÍ*/
			}
			if(iNBytes == 4 && hSign == H5T_SGN_2)    
			{
				iDataType = UNINTTYPE;/*4×Ö½ÚÓÐ·ûºÅÕûÐÍ*/
			}
			if(iNBytes == 8 && hSign == H5T_SGN_NONE) 
			{
				iDataType = UNINT64TYPE;/*8×Ö½ÚÎÞ·ûºÅÕûÐÍ*/
			}
			if(iNBytes == 8 && hSign == H5T_SGN_2)    
			{
				iDataType = INT64TYPE;/*8×Ö½ÚÓÐ·ûºÅÕûÐÍ*/		
			}
		}
		H5Dclose( hDSet);
	 H5Fclose(hFile);
	  return 0;
  }
  bool  HDF5Helper::GetDataType(hid_t hType,int &iDataType,int &iNBytes)
  {
	  if(hType < 0)
	  {
		  return false;
	  }
	  else
	  {
		  /*ï¿½Ãµï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ö¾ï¿½ï¿½*/
		  H5T_class_t hTypeClass = H5Tget_class(hType);
		  if(hTypeClass == H5T_NO_CLASS)/*ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Í´ï¿½ï¿½ï¿*/
		  {
			  return false;
		  }
		  if(hTypeClass == H5T_INTEGER) /*ï¿½ï¿½ï¿½ï¿½Ê±*/
		  {
			  /*ï¿½Ãµï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ö½Ú´ï¿½Ð*/
			  size_t hSize = H5Tget_size(hType);
			  if(hSize == 0)/*ï¿½ï¿½ï¿½ï¿½Ö½ï¿½Î?*/
			  {
				  return false;
			  }

			  /*ï¿½Ãµï¿½ï¿½ï¿½ï¿½ï¿½Ç·ï¿½ï¿½Ð·ï¿½ï¿½*/
			  H5T_sign_t hSign = H5Tget_sign(hType);
			  if(hSign == H5T_SGN_ERROR)/*ï¿½ï¿½ï¿½ï¿½Å´ï¿½ï¿½ï¿*/
			  {
				  return false;
			  }
			  if(hSize == 1 && hSign == H5T_SGN_NONE)
			  {
				  iDataType = UNCHARTYPE;/*1ï¿½Ö½ï¿½ï¿½Þ·ï¿½ï¿½ï¿½ï¿½ï¿½ï¿*/
				  iNBytes = 1;
			  }
			  if(hSize == 1 && hSign == H5T_SGN_2)
			  {
				  iDataType = CHARTYPE;/*1ï¿½Ö½ï¿½ï¿½Ð·ï¿½ï¿½ï¿½ï¿½ï¿½ï¿*/
				  iNBytes = 1;
			  }
			  if(hSize == 2 && hSign == H5T_SGN_NONE)
			  {
				  iDataType = UNSHORTTYPE;/*2ï¿½Ö½ï¿½ï¿½Þ·ï¿½ï¿½ï¿½ï¿½ï¿½ï¿*/
				  iNBytes = 2;
			  }
			  if(hSize == 2 && hSign == H5T_SGN_2)
			  {
				  iDataType = SHORTTYPE;/*2ï¿½Ö½ï¿½ï¿½Ð·ï¿½ï¿½ï¿½ï¿½ï¿½ï¿*/
				  iNBytes = 2;
			  }
			  if(hSize == 4 && hSign == H5T_SGN_NONE)
			  {
				  iDataType = UNINTTYPE;/*4ï¿½Ö½ï¿½ï¿½Þ·ï¿½ï¿½ï¿½ï¿½ï¿½ï¿*/
				  iNBytes = 4;
			  }
			  if(hSize == 4 && hSign == H5T_SGN_2)
			  {
				  iDataType = INTTYPE;/*4ï¿½Ö½ï¿½ï¿½Ð·ï¿½ï¿½ï¿½ï¿½ï¿½ï¿*/
				  iNBytes = 4;
			  }
			  if(hSize == 8 && hSign == H5T_SGN_NONE)
			  {
				  iDataType = UNINT64TYPE;/*8ï¿½Ö½ï¿½ï¿½Þ·ï¿½ï¿½ï¿½ï¿½ï¿½ï¿*/
				  iNBytes = 8;
			  }
			  if(hSize == 8 && hSign == H5T_SGN_2)
			  {
				  iDataType = INT64TYPE;/*8ï¿½Ö½ï¿½ï¿½Ð·ï¿½ï¿½ï¿½ï¿½ï¿½ï¿*/
				  iNBytes = 8;
			  }
		  }
		  else if(hTypeClass == H5T_FLOAT)/*ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ê±*/
		  {
			  /*ï¿½Ãµï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ö½Ú´ï¿½Ð*/
			  size_t hSize = H5Tget_size(hType);
			  if(hSize == 0)/*ï¿½ï¿½ï¿½ï¿½Ö½ï¿½Î?*/
			  {
				  return false;
			  }
			  if(hSize == 4)
			  {
				  iDataType = FLOATTYPE;/*4ï¿½Ö½Ú¸ï¿½ï¿½ï¿½ï¿½ï¿½*/
				  iNBytes = 4;
			  }
			  else
			  {
				  iDataType = DOUBLETYPE;/*8ï¿½Ö½ï¿½Ë«ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½*/
				  iNBytes = 8;
			  }
		  }
		  else if(hTypeClass == H5T_STRING)/*ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ê±*/
		  {
			  iDataType = STRINGTYPE;
			  iNBytes = 1;
		  }
		  else/*ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Í²ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½*/
		  {
			  return false;
		  }
	  }

	  return true;
  }
bool HDF5Helper::GetAttrItem(hid_t fd,const char* dsname,AttrItem &aG)
{
	herr_t hStatus;
	if (fd<0 || aG.strName=="" || aG.iArraySize<=0 || aG.iDataType<0 || aG.iDataType>MAXTYPENUM )
	{
		return false;
	}

	if (aG.pData!=NULL)
	{
		delete []aG.pData;
		aG.pData = NULL;
	}

	switch(aG.iDataType) // ¸ù¾ÝÊý¾ÝÀàÐÍ±êºÅ
	{
	case UNCHARTYPE:
		{
			aG.pData = new unsigned char[aG.iArraySize];
			hStatus = H5LTget_attribute_uchar(fd,dsname,aG.strName.c_str(),(unsigned char*)aG.pData);  
			break;
		}
	case CHARTYPE:
		{
			aG.pData = new char[aG.iArraySize];
			hStatus = H5LTget_attribute_char(fd,dsname,aG.strName.c_str(),(char*)aG.pData);  
			break;
		}
	case UNSHORTTYPE:
		{
			aG.pData = new unsigned short int[aG.iArraySize];
			hStatus = H5LTget_attribute_ushort(fd,dsname,aG.strName.c_str(),(unsigned short int*)aG.pData);  
			break;
		}
	case SHORTTYPE:
		{
			aG.pData = new short int[aG.iArraySize];
			hStatus = H5LTget_attribute_short(fd,dsname,aG.strName.c_str(),(short int*)aG.pData);  
			break;
		}
	case UNINTTYPE:
		{
			aG.pData = new unsigned int[aG.iArraySize];
			hStatus = H5LTget_attribute_uint(fd,dsname,aG.strName.c_str(),(unsigned int*)aG.pData);  
			break;
		}
	case INTTYPE:
		{
			aG.pData = new int[aG.iArraySize];
			hStatus = H5LTget_attribute_int(fd,dsname,aG.strName.c_str(),(int*)aG.pData);  
			break;
		}
	case UNINT64TYPE:
		{
			aG.pData = new unsigned long[aG.iArraySize];
			hStatus = H5LTget_attribute_ulong(fd, dsname, aG.strName.c_str(), (unsigned long*)aG.pData);  
			break;
		}
	case INT64TYPE:
		{
			aG.pData = new long[aG.iArraySize];
			hStatus = H5LTget_attribute_long(fd,dsname,aG.strName.c_str(),(long*)aG.pData);  
			break;
		}
	case FLOATTYPE:
		{
			aG.pData = new float[aG.iArraySize];
			hStatus = H5LTget_attribute_float(fd,dsname,aG.strName.c_str(),(float*)aG.pData);  
			break;
		}
	case DOUBLETYPE:
		{
			aG.pData = new double[aG.iArraySize];
			hStatus = H5LTget_attribute_double(fd,dsname,aG.strName.c_str(),(double*)aG.pData);  
			break;
		}
	case STRINGTYPE:
		{
			aG.pData = new char[aG.iArraySize];
			hStatus = H5LTget_attribute_string(fd,dsname,aG.strName.c_str(),(char*)aG.pData);  			
			break;
		}
	default:
		return false;
	} // switch

	return true;
}
int HDF5Helper::GetDSAttrInfo( const char* file,const char* dsname,std::vector<AttrItem> &vc)
{
	 hid_t hFile =H5Fopen(file,H5F_ACC_RDONLY,H5P_DEFAULT);
	 if(hFile<0)
		 return -1;
	 GetDSAttrInfo(hFile,dsname,vc);
	 H5Fclose(hFile);
	 return 0;
}
 int HDF5Helper::GetDSAttrInfo(hid_t fd,const char* dsname,std::vector<AttrItem> &vc)
 {
	 //»ñÈ¡Ä³¸öÊý¾Ý¼¯µÄÊôÐÔ 
	 int i = 0;
	 hid_t hDataSet = H5Dopen(fd, dsname); // ´ò¿ªÖ¸¶¨µÄÊý¾Ý¼¯
	 if ( hDataSet < 0 )
	 {
		 return -1;
	 }

	 //¶ÔÓ¦µÄÊý¾Ý¼¯ÊôÐÔµÄ¸öÊý
	 int  _iNumAttr =  H5Aget_num_attrs(hDataSet);
	 if ( _iNumAttr <= 0 ) // Êý¾Ý¼¯ÊôÐÔÐ¡ÓÚ0
	 {
		 return -1;
	 }

	 for ( i = 0; i < (int)_iNumAttr; ++i ) // ±éÀúÃ¿¸öÊý¾Ý¼¯ÊôÐÔ
	 {
		 AttrItem aTmp; // Êý¾Ý¼¯ÊôÐÔ
		 hid_t hAttr;
		 char pAttrTmp[200];
		 hAttr = H5Aopen_idx(hDataSet, i);
		 ssize_t _hName =  H5Aget_name(hAttr , 100, pAttrTmp);		
		 aTmp.strName = pAttrTmp;		

		 hsize_t _hSize =  H5Aget_storage_size(hAttr);
		 hid_t _hType =  H5Aget_type(hAttr);

		 int _iNBytes;
		 if (!GetDataType(_hType,aTmp.iDataType,_iNBytes))
		 {
			 return -1;
		 }

		 aTmp.iByteLen = (int)_hSize;
		 aTmp.iArraySize = (int)_hSize/_iNBytes;

		 if (!GetAttrItem(fd,dsname,aTmp))
		 {
			 return -1;
		 }

		 //×¢Òâ£ºvector<attrOne>: ÒªÇóÈÝÆ÷ÔªËØÒª¾ßÓÐ¡°ÖµÓïÒå¡±
		 //attrOneÖÐ¾ßÓÐÖ¸Õë£¬ËùÒÔ£¬ÐèÒªÖØÐÂ¹¹Ôì¿½±´¹¹Ôìº¯ÊýºÍÖØÔØÔËËã·û
		 vc.push_back(aTmp);

		 H5Aclose(hAttr);
	 } // for
	 H5Dclose(hDataSet);
	 return 0;
 }
void HDF5Helper::GetFileDayOrNightFLag(const char*file,std::string flagname,std::string &data)
{
	hid_t hFile =H5Fopen(file,H5F_ACC_RDONLY,H5P_DEFAULT);
	if(hFile<0)
		return ;
	hid_t hAttr;
	for (int i=0;i<100;++i)
	{
		hAttr = H5Aopen_idx(hFile,i);
		if (hAttr<0)
		{
			break;
			return ;
		}
		char pAttrTmp[200];
		//»ñÈ¡ÊôÐÔµÄÃû×Ö 
		ssize_t _hName =  H5Aget_name(hAttr , 100, pAttrTmp);		
		std::string strName = pAttrTmp;
		if(strName!=flagname)
		{
			H5Aclose(hAttr);
			continue;
		}
		//»ñÈ¡ÊôÐÔÄÚÈÝDay Or Night
		char buf[20];
		H5LTget_attribute_string(hFile,"/",flagname.c_str(),(char*)buf); 
		data=buf;
		H5Aclose(hAttr);
	}
	H5Fclose(hFile);
}
int HDF5Helper::GetFileAttrInfo(hid_t fd,std::vector<AttrItem> &vc)
{
	int i = 0;
	hid_t hAttr;
	for (i=0;i<100;++i)
	{
		hAttr = H5Aopen_idx(fd,i);
		if (hAttr<0)
		{
			break;
			return 0;
		}

		AttrItem attrTmp;
		char pAttrTmp[200];
		//»ñÈ¡ÊôÐÔµÄÃû×Ö 
		ssize_t _hName =  H5Aget_name(hAttr , 100, pAttrTmp);		
		attrTmp.strName = pAttrTmp;

		hsize_t _hSize =  H5Aget_storage_size(hAttr);//×Ü×Ö½ÚÊý

		hid_t _hType =  H5Aget_type(hAttr);	
		int _iNBytes;
		if (!GetDataType(_hType,attrTmp.iDataType,_iNBytes))
		{
			return -1;
		}
		attrTmp.iArraySize = (int)_hSize/_iNBytes;//Êý×é´óÐ¡
		attrTmp.iByteLen = (int)_hSize;

		//»ñÈ¡È«¾ÖÊôÐÔ
		if (!GetAttrItem(fd,"/",attrTmp))
		{
			return -1;
		}

		vc.push_back(attrTmp);

		H5Aclose(hAttr);
	}

	return 0;
}
  int HDF5Helper::GetDataSet(const char* file,std::string &dset,void *pData,hid_t type)
  {
	  if(pData==NULL)
		  return -1;
	  //pDataÒªÌáÇ°ÉêÇëºÃ£¬²¢Ìî³ä£¬GetDataSetAttr»ñÈ¡ÐÅÏ¢
	  hid_t hFile =H5Fopen(file,H5F_ACC_RDONLY,H5P_DEFAULT);
	  hid_t hDSet=H5Dopen(hFile,dset.c_str());
	  if(hDSet<0)
	  {
		H5Fclose(hFile);
		return -1;
	  }
	 // hid_t hDSet=H5Dopen(hFile,"/Data/EV_250_RefSB_b1");

	  hid_t	memspace;
	  hid_t hDsSpace= H5Dget_space(hDSet);
	  int width=0,height=0;
	  hid_t	RANK;	
	  hsize_t	hDims[3]={0,0,0};
	  hsize_t	dimsw[3] = { 0 };	
	  RANK=H5Sget_simple_extent_ndims(hDsSpace);
	  if(H5Sget_simple_extent_dims(hDsSpace, hDims, dimsw)<0)
	  {
		 H5Dclose( hDSet);
		 H5Fclose(hFile);
	  	return -1;
	  }
	  width=hDims[1];
	  height=hDims[0];
	  //herr_t hStatus = H5Sselect_hyperslab (hDsSpace, H5S_SELECT_SET, NULL, NULL, hDims, NULL);  
	  memspace = H5Screate_simple (RANK, hDims, NULL); 
	 // type=H5T_NATIVE_USHORT;
	  H5Dread(hDSet,type,memspace,hDsSpace,H5P_DEFAULT,pData);

	  H5Dclose( hDSet);
	  H5Fclose(hFile);
	  return 0;
  }
int HDF5Helper::SaveDataSet(const char *file,std::string &dset,void *pData,int dimens,hsize_t *hDimValue)
{
	bool bCompress=true;
	hid_t hFile =H5Fopen(file,H5F_ACC_RDWR,H5P_DEFAULT);
	if(hFile<0)
	{
		//±¾µØÃ»ÓÐÎÄ¼þ
		hFile=H5Fcreate(file,H5F_ACC_TRUNC,H5P_DEFAULT,H5P_DEFAULT);
		if(hFile<0)
			return -1;
	}
	hid_t hDataspace=H5Screate_simple(dimens,hDimValue,NULL);
	hsize_t OutDataBegin[2]		= { 0, 0 };				// Êä³öÊý¾Ý¿Õ¼äµÄÆðÊ¼(Offset of start )
	hsize_t MemDataBegin[2]		= { 0, 0 };				// ÄÚ´æÊý¾Ý¿Õ¼äµÄÆðÊ¼

	//herr_t hStatus = H5Sselect_hyperslab(	hDataspace,		// Êý¾Ý¿Õ¼ä±êºÅ
	//	H5S_SELECT_SET,		
	//	OutDataBegin,			// ¿ªÊ¼µÄÆðÊ¼
	//	NULL,
	//	OutDataOffset,			// ²½½øÆ«ÒÆ
	//	NULL );
	hid_t hDataset=0;
	if(bCompress)
	{
		hid_t hProperties = H5Pcreate(H5P_DATASET_CREATE);
		hsize_t	hChunk_size[3] ={400,400,400};		
		// Sets the size of the chunks used to store a chunked layout dataset. 
		H5Pset_chunk(	hProperties,	// Dataset creation property list identifier
			dimens,		// The number of dimensions of each chunk
			hChunk_size );	// An array defining the size, in dataset elements, of each chunk.

		unsigned int szip_options_mask = H5_SZIP_NN_OPTION_MASK; // Selects nearest neighbor coding method
		unsigned int szip_pixels_per_block = 32;

		// Sets up use of the SZIP compression filter
		H5Pset_szip (	hProperties,			// Dataset creation or dataset transfer property list identifier
			szip_options_mask,		// A bit-mask conveying the desired SZIP options
			szip_pixels_per_block );	// The number of pixels or data elements in each data block
		hDataset = H5Dopen(hFile,dset.c_str()); 
		if(hDataset<0)
			hDataset=H5Dcreate(hFile,dset.c_str(),H5T_NATIVE_USHORT,hDataspace,hProperties);
	}
	else
	{
		hDataset = H5Dopen(hFile,dset.c_str()); 
		if(hDataset<0)
			hDataset=H5Dcreate(hFile,dset.c_str(),H5T_NATIVE_USHORT,hDataspace,H5P_DEFAULT);
	}
	if(hDataset<0)
	{
		H5Sclose(hDataspace);
		H5Fclose(hFile);
		return -1;
	}
	herr_t res=H5Dwrite(hDataset,H5T_NATIVE_USHORT,H5S_ALL,H5S_ALL,H5P_DEFAULT,pData);
	res=H5Dclose(hDataset);
	res=H5Sclose(hDataspace);
	res=H5Fclose(hFile);
	return 0;
}
bool HDF5Helper::SetAttrInfo(const char* file,const char* strDsName,const std::vector<AttrItem> &vAttrD)
{
	hid_t hFile =H5Fopen(file,H5F_ACC_RDWR,H5P_DEFAULT);
	if(hFile<0)
		return false;
	SetAttrInfo(hFile,strDsName,vAttrD);
	H5Fclose(hFile);
	return true;
}
 bool HDF5Helper::SetAttrInfo(hid_t hF,const char* strDsName,const std::vector<AttrItem> &vAttrD)
 {
	 if (hF<0 || vAttrD.size()<=0)
	 {
		 return false;
	 }
	 herr_t hStatus;
	 int i;
	 for (i=0;i<vAttrD.size();++i)
	 {
		 AttrItem aG = vAttrD[i];

		 switch(aG.iDataType) {
		 case UNCHARTYPE:
			 {			
				 hStatus = H5LTset_attribute_uchar(hF,strDsName,aG.strName.c_str(),(const unsigned char*)aG.pData,aG.iArraySize);			
				 break;
			 }
		 case CHARTYPE:
			 {
				 hStatus = H5LTset_attribute_char(hF,strDsName,aG.strName.c_str(),(const char*)aG.pData,aG.iArraySize);			
				 break;
			 }
		 case UNSHORTTYPE:
			 {
				 hStatus = H5LTset_attribute_ushort(hF,strDsName,aG.strName.c_str(),(const unsigned short*)aG.pData,aG.iArraySize);			
				 break;
			 }
		 case SHORTTYPE:
			 {
				 hStatus = H5LTset_attribute_short(hF,strDsName,aG.strName.c_str(),(const short*)aG.pData,aG.iArraySize);			
				 break;
			 }
		 case UNINTTYPE:
			 {
				 hStatus = H5LTset_attribute_uint(hF,strDsName,aG.strName.c_str(),(const unsigned int*)aG.pData,aG.iArraySize);			
				 break;
			 }
		 case INTTYPE:
			 {
				 hStatus = H5LTset_attribute_int(hF,strDsName,aG.strName.c_str(),(const int*)aG.pData,aG.iArraySize);			
				 break;
			 }
		 case UNINT64TYPE:
			 {
				 hStatus = H5LTset_attribute_ulong(hF,strDsName,aG.strName.c_str(),(const unsigned long*)aG.pData,aG.iArraySize);			
				 break;
			 }
		 case INT64TYPE:
			 {
				 hStatus = H5LTset_attribute_long(hF,strDsName,aG.strName.c_str(),(const long*)aG.pData,aG.iArraySize);			
				 break;
			 }
		 case FLOATTYPE:
			 {
				 hStatus = H5LTset_attribute_float(hF,strDsName,aG.strName.c_str(),(const float*)aG.pData,aG.iArraySize);			
				 break;
			 }
		 case DOUBLETYPE:
			 {
				 hStatus = H5LTset_attribute_double(hF,strDsName,aG.strName.c_str(),(const double*)aG.pData,aG.iArraySize);			
				 break;
			 }
		 case STRINGTYPE:
			 {
				 hStatus = H5LTset_attribute_string(hF,strDsName,aG.strName.c_str(),(const char*)aG.pData);			
				 break;
			 }
		 default:
			 return false;
		 }
	 }

	 return true;
 }