#include "stdafx.h"
// HDF5Func.cpp: implementation of the HDF5Func class.
//
//////////////////////////////////////////////////////////////////////
#include "HDF5Func.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
HDF5Func::HDF5Func()
{
	pBuf = new unsigned short[NX*NY];
	if (pBuf == NULL)
	{
		
	}
	
	pData = new char[1];
	//pData = NULL;

	pAttrTmp = new char[200*sizeof(char)];

	iDsDimension = -1;
	iDsDataType = -1;
	iOneDataSize[0]= iOneDataSize[1]=iOneDataSize[2] = 0;

	hChunk_size[0] = hChunk_size[1] = 0;
	hDsSize[0] = hDsSize[1] = 0;
	m_hFile = 0;
}

HDF5Func::~HDF5Func()
{
	if (pBuf!=NULL)
	{
		delete []pBuf;
		pBuf = NULL;
	}

	if (pData!=NULL)
	{
		delete []pData;
		pData = NULL;
	}

	if (pAttrTmp!=NULL)
	{
		delete []pAttrTmp;
		pAttrTmp = NULL;
	}
}

bool HDF5Func::CompressHDFFile(string strInFileFolder,
								   string strInFileName,
								   string strOutFileFolder )
{
	int i=0;
	string strInFilePathName = strInFileFolder+"\\"+strInFileName;
	string strOutFilePathName = strOutFileFolder +"\\"+strInFileName;

	hid_t hInFile = H5Fopen(strInFilePathName.c_str(),H5F_ACC_RDONLY,H5P_DEFAULT);
	if (hInFile ==-1)
	{
		return false;
	}

	vector<string> vDsNs;
	if (!getAllDsNames(hInFile, vDsNs))
	{
		return false;
	}

	int iDsNum = vDsNs.size();
	if (iDsNum <= 0)
	{
		return false;
	}
		
	//
	if (!createOutFile(strOutFilePathName))
	{
		return false;
	}

	hid_t hOutFile  = H5Fopen(strOutFilePathName.c_str(), H5F_ACC_RDWR, H5P_DEFAULT);	/*打开HDF文件*/
	if(hOutFile == -1)	/*打开文件失败*/
	{
		H5Fclose(hInFile);
		return false;
	}
	
	if (!getSetAttrInfo(hInFile,"/",hOutFile))
	{
			H5Fclose(hOutFile);
			H5Fclose(hInFile);
			return false;
	}
	
	for (i=0;i<iDsNum;++i)		
	{
		if (!mallocHdfBuf(hInFile,vDsNs[i].c_str()))
		{			
			H5Fclose(hOutFile);
			H5Fclose(hInFile);
			return false;
		}
		
		if (ReadHdf5FileData(hInFile,vDsNs[i].c_str(),pData))
		{
			H5Fclose(hOutFile);
			H5Fclose(hInFile);
			return false;
		}
		
		if (iDsDimension==2)
		{
			if (WriteHdf5FileData2D(hOutFile,vDsNs[i].c_str(),
				pData,iDsDataType,iOneDataSize,iOneDataSize,2,true))
			{
				H5Fclose(hOutFile);
				H5Fclose(hInFile);
				return false;
			}
		}
		else if (iDsDimension==3)
		{
			if (!WriteHdf5FileData3D(hOutFile,vDsNs[i].c_str(),
				pData,iDsDataType,iOneDataSize,iOneDataSize,3,true))
			{
				H5Fclose(hOutFile);
				H5Fclose(hInFile);
				return false;
			}		
		}
		else if (iDsDimension==1)
		{
			if (!WriteHdf5FileData1D(hOutFile,vDsNs[i].c_str(),
				pData,iDsDataType,iOneDataSize[0],iOneDataSize[0],1,true))
			{
				H5Fclose(hOutFile);
				H5Fclose(hInFile);
				return false;
			}
		}
		
		if (!getSetAttrInfo(hInFile,vDsNs[i],hOutFile))
		{
				H5Fclose(hOutFile);
				H5Fclose(hInFile);
				return false;
		}	
	}

	H5Fclose(hOutFile);
	H5Fclose(hInFile);

	return true;
}

int HDF5Func::ReadHdf5FileData(const char* strInfName,
								const char* strDsName, 
								void *pData )
{
	hid_t hInf = H5Fopen(strInfName, H5F_ACC_RDONLY, H5P_DEFAULT);

	if (hInf==-1)
	{
		return FALSE;
	}

	// 读取HDF5文件数据
	if ( ReadHdf5FileData(hInf, strDsName, pData, NULL, 0, 0, 0) )
	{
		H5Fclose(hInf);
		return FALSE;
	}

	H5Fclose(hInf);

	return SUCCESS;
}

//当数据集的深度为3维的时候，iStart[0]表示深度方向的起始位置；iStart[1]表示
//高度方向的起始位置；iStart[2]表示宽度方向的起始位置;
int HDF5Func::ReadHdf5FileData(hid_t hF,
									const char* strDsName, 
									void *pData ,
									hsize_t* iStart/* =NULL */,
									int nXNum/* =0 */, 
									int nYNum/* =0 */,
									int nZNum/* =0 */)
{
	/*变量定义及初始化*/	
	hid_t	hType;
	hsize_t	hDims[3]={1,1,1};
	hsize_t	dimsw[3] = { 0 };		// 各维数的大小
	hid_t	RANK;	
	hid_t	memspace;

	int	iDimensions=1;	/*HDF5数据维数*/
	int	iDataType=1;	/*HDF5数据类型*/
	
	/*参数检查*/
	if(strDsName==NULL)
	{
		cout << "SDS name is null"<<endl;
		return FALSE;
	}
	if (pData==NULL)
	{
		cout << "data pointer is null" << endl;
		return FALSE;
	}
	
//	if(access(strFileName,0)!=0)	/*检测文件是否存在*/
//	{
//		return false;
//	}
//
//	if(H5Fis_hdf5(strFileName)==-1)
//	{
//		return false;
//	}

	/*得到数据集信息*/
	int iNBytes;
	hStatus = GetHdfSdsParam(hF, strDsName, &iDimensions, dimsw, iDataType,iNBytes);
	if (hStatus==-1)
	{
		return FALSE;
	}

	//给全局变量赋值
	int i = 0;
	iDsDimension = iDimensions;
	iDsDataType = iDataType;
	for ( i = 0; i < 3; ++i )
	{
		iOneDataSize[i] = (int)dimsw[i];
	}

	hType = getHdfDataType(iDataType);
	if (hType == -1)
	{
		return FALSE;
	}

//	hFile  = H5Fopen(strFileName, H5F_ACC_RDONLY, H5P_DEFAULT);	/*打开HDF文件*/
//	if(hFile == -1)	/*打开文件失败*/
//	{
//		return false;
//	}

	/*打开数据集*/
	hDataSet = H5Dopen(hF, strDsName);
	if(hDataSet==-1)
	{
		return FALSE;
	}

	//将数据读入到pData中
	/*读取数据集数据*/
	if ( iStart != NULL )	/*读局部数据块*/
	{
		if ( iStart[0] < 0 || iStart[1] < 0 || iStart[2] < 0 || nXNum <= 0 || nYNum <= 0 || nZNum <= 0 )
		{			
			H5Dclose(hDataSet);
			return FALSE;			//读取科学数据参数错误，越界。
		}

		if ( iStart[0] + nXNum > dimsw[0] )		// X
			hDims[0] = dimsw[0] - iStart[0];
		else
			hDims[0] = nXNum;

		if ( iStart[1] + nYNum>dimsw[1] )		// Y
			hDims[1] = dimsw[1] - iStart[1];
		else
			hDims[1] = nYNum;

		if ( iStart[2] + nZNum > dimsw[2] )		// Z
			hDims[2] = dimsw[2] - iStart[2];
		else
			hDims[2] = nZNum;
		
		hDsSpace= H5Dget_space(hDataSet);
		RANK=H5Sget_simple_extent_ndims(hDsSpace);	//得到数据集维数
		hStatus = H5Sselect_hyperslab (hDsSpace, H5S_SELECT_SET, iStart, NULL, hDims, NULL);  
		memspace = H5Screate_simple (RANK, hDims, NULL); 
		
// 		hStatus = H5Dread (hDataSet, hType, memspace, hDsSpace, H5P_DEFAULT, pData);
		hdfReadToBuf(hDataSet, hType, memspace, hDsSpace, H5P_DEFAULT, pData, iDataType);
		
		H5Sclose(hDsSpace);
		H5Dclose(hDataSet);
		H5Sclose(memspace);
	}
	else	/*读全部数据*/
	{
//		unsigned short int* pBuf = (unsigned short int*)pData;
//		hStatus = H5Dread (hDataSet, hType, H5S_ALL, H5S_ALL, H5P_DEFAULT, pBuf);
		hdfReadToBuf(hDataSet, hType, H5S_ALL, H5S_ALL, H5P_DEFAULT, pData,iDataType);

		H5Dclose(hDataSet);				
	}

	return SUCCESS;
} // ReadHdf5FileData

// 读取某个2维数据集的某一部分（不能用来读复合数据类型）
// iStart[0]:表示高度方向的起始值，即起始的行数
// iStart[1]:表示宽度方向的起始值，即起始的列数
// iHight:要读取的数据的高度，即总行数
// iWidth:要读取的数据的宽度，即总列数
int HDF5Func::ReadHdf5FileData2D( hid_t hF,const char* strDsName,
						void *pData,
						hsize_t* iStart,
						int nXNum,
						int nYNum )
{
	/*变量定义及初始化*/	
	hid_t	hType;
	hsize_t	hDims[2] = { 0 }; // 读取的维度
	hsize_t	dimsw[3] = { 0 }; // 高/宽/深 各维大小
	hid_t	RANK;	
	hid_t	memspace;

	int	iDimensions=1;	/*HDF5数据维数*/
	int	iDataType=-1;	/*HDF5数据类型*/

	/*参数检查*/
	if(strDsName==NULL)
	{
		cout << "SDS name is null"<<endl;
		return FALSE;
	}
	if (pData==NULL)
	{
		cout << "data pointer is null" << endl;
		return FALSE;
	}
	
//	if(access(strFileName,0)!=0)	/*检测文件是否存在*/
//	{
//		return false;
//	}
//
//	if(H5Fis_hdf5(strFileName)==-1)
//	{
//		return false;
//	}

	/*得到数据集信息*/
	int iNBytes;
	hStatus = GetHdfSdsParam(hF, strDsName, &iDimensions, dimsw, iDataType, iNBytes);
	if ( hStatus == -1 )
	{
		return FALSE;
	}

	//给全局变量赋值
	int i = 0;
	iDsDimension = iDimensions;
	iDsDataType = iDataType;
	for (i=0;i<2;++i)
	{
		iOneDataSize[i] = (int)dimsw[i];
	}

	hType = getHdfDataType(iDataType); // 只能处理整形和浮点型数据，不能处理复合数据类型
	if (hType == -1)
	{
		return FALSE;
	}

	/*打开数据集*/
	hDataSet = H5Dopen(hF, strDsName);
	if(hDataSet==-1)
	{
		return FALSE;
	}

	//将数据读入到pData中
	/*读取数据集数据*/
	if ( iStart != NULL )	/*读数据块的子集*/
	{
		if ( iStart[0] < 0 || iStart[1] < 0 || nXNum <= 0 || nYNum <= 0 )
		{			
			H5Dclose(hDataSet);
			return FALSE;			//读取科学数据参数错误，越界。
		}

		if ( iStart[0] + nXNum > dimsw[0] ) //高度/行
			hDims[0] = dimsw[0] - iStart[0];
		else
			hDims[0] = nXNum;

		if ( iStart[1] + nYNum > dimsw[1] ) //宽度/列
			hDims[1] = dimsw[1] - iStart[1];
		else
			hDims[1] = nYNum;
		
		hDsSpace= H5Dget_space(hDataSet); // H5Dget_space returns an identifier for a copy of the dataspace for a dataset. 
		RANK = H5Sget_simple_extent_ndims(hDsSpace);	// determines the dimensionality (or rank) of a dataspace

		// selects a hyperslab region to add to the current selected region for the dataspace specified by space_id
		// (Returns a non-negative value if successful; otherwise returns a negative value.)
		hStatus = H5Sselect_hyperslab (		hDsSpace,			// Identifier of dataspace selection to modify
							H5S_SELECT_SET,			// Operation to perform on current selection.
							iStart,				// Offset of start of hyperslab
							NULL,				// Number of blocks included in hyperslab.
							hDims,				// Hyperslab stride
							NULL );				// Size of block in hyperslab
		memspace = H5Screate_simple (RANK, hDims, NULL);
		
// 		hStatus = H5Dread (hDataSet, hType, memspace, hDsSpace, H5P_DEFAULT, pData);
		hdfReadToBuf(hDataSet, hType, memspace, hDsSpace, H5P_DEFAULT, pData, iDataType);
		
		H5Sclose(hDsSpace);
		H5Dclose(hDataSet);
		H5Sclose(memspace);
	}
	else	/*读全部数据*/
	{
//		unsigned short int* pBuf = (unsigned short int*)pData;
//		hStatus = H5Dread (hDataSet, hType, H5S_ALL, H5S_ALL, H5P_DEFAULT, pBuf);
		hdfReadToBuf(hDataSet, hType, H5S_ALL, H5S_ALL, H5P_DEFAULT, pData,iDataType);

		H5Dclose(hDataSet);				
	}

	return SUCCESS;
} // ReadHdf5FileData2D

int HDF5Func::WriteHdf5FileData2D(	 hid_t hF,
									 const char* strDsName,
									 void *pData,
									 int iDataType,
									 int iMemDimSize[2],
									 int iFileDimSize[2],
									 bool bCreate,
									 bool isCompress,	/* =false */
									 int *pMemStart,	/* =NULL */
									 int *pMemCount,	/* =NULL */
									 int *pFileStart,	/* =NULL */
									 int *pFileCount	/* =NULL */
									)
{

	/*参数检查*/
	if ( strDsName == NULL || pData == NULL )
	{
		return FALSE;
	}

	if ( iDataType < 0 || iDataType > MAXTYPENUM )
	{
		return FALSE;
	}

	hid_t	hType;
	hid_t memspace;

	hsize_t filedim[2]	= { iFileDimSize[0], iFileDimSize[1] } ;		// 输出文件的数据空间维数
	hsize_t memorydim[2]	= { iMemDimSize[0], iMemDimSize[1] };		// 内存数据空间的维数

	hsize_t OutDataBegin[2]		= { 0, 0 };				// 输出数据空间的起始(Offset of start )
	hsize_t MemDataBegin[2]		= { 0, 0 };				// 内存数据空间的起始

	hsize_t OutDataOffset[2]	= { iFileDimSize[0], iFileDimSize[1] };	// 输出数据控件的偏移
	hsize_t MemDataOffset[2]	= { iMemDimSize[0], iMemDimSize[1] };	// 内存数据空间的偏移

	if ( pMemStart != NULL )	// 内存数据空间起始
	{
		MemDataBegin[0] = pMemStart[0];
		MemDataBegin[1] = pMemStart[1];
	}	

	if ( pMemCount != NULL )	// 内存数据空间偏移
	{
		MemDataOffset[0] = pMemCount[0];
		MemDataOffset[1] = pMemCount[1];
	}

	if ( pFileStart != NULL )	// 输出数据空间起始
	{
		OutDataBegin[0] = pFileStart[0];
		OutDataBegin[1] = pFileStart[1];
	}

	if ( pFileCount != NULL )	// 输出数据空间的偏移
	{
		OutDataOffset[0] = pFileCount[0];
		OutDataOffset[1] = pFileCount[1];
	}
	
	for ( int i = 0; i < 2; ++i )
	{
		if ( iMemDimSize[i] <= 0 || iFileDimSize[i] <= 0 )
		{			
			return FALSE;
		}

		if ( (( MemDataBegin[i] + MemDataOffset[i]) > iMemDimSize[i]) || ((OutDataBegin[i] + OutDataOffset[i]) > iFileDimSize[i]) )
		{			
			return FALSE;
		}
	}

	hDsSpace = H5Screate_simple(2, filedim, NULL); // 创建一个文件数据空间(输出)

	// Selects a hyperslab region to add to the current selected region
	hStatus = H5Sselect_hyperslab(	hDsSpace,		// 数据空间标号
					H5S_SELECT_SET,		
					OutDataBegin,			// 开始的起始
					NULL,
					OutDataOffset,			// 步进偏移
					NULL );
	
	memspace = H5Screate_simple(2, memorydim, NULL); // 创建一个内存数据空间(输入)
	
	// Selects a hyperslab region to add to the current selected region
	hStatus = H5Sselect_hyperslab(	memspace,
					H5S_SELECT_SET,
					MemDataBegin,			// 开始的偏移
					NULL,
					MemDataOffset,			// 步进
					NULL );
	
	hType = getHdfDataType(iDataType);
	if ( hType == -1 )
	{
		return FALSE;
	}


	if (isCompress) // 是否压缩
	{
		if ( iFileDimSize[0] > CPY ) // 输出数据尺寸超标否
		{
			hChunk_size[0] = CPY;		
		}
		else
		{
			hChunk_size[0] = iFileDimSize[0];
		}

		if ( iFileDimSize[1] > CPX ) // 输出数据尺寸超标否
		{
			hChunk_size[1] = CPX;
		}
		else
		{
			hChunk_size[1] = iFileDimSize[1];
		}

		// Creates a new property as an instance of a property list class
		hProperties = H5Pcreate(H5P_DATASET_CREATE);

		// Sets the size of the chunks used to store a chunked layout dataset. 
		H5Pset_chunk(	hProperties,	// Dataset creation property list identifier
				2,		// The number of dimensions of each chunk
				hChunk_size );	// An array defining the size, in dataset elements, of each chunk.

		szip_options_mask = H5_SZIP_NN_OPTION_MASK; // Selects nearest neighbor coding method
		szip_pixels_per_block = 32;

		// Sets up use of the SZIP compression filter
		H5Pset_szip (	hProperties,			// Dataset creation or dataset transfer property list identifier
				szip_options_mask,		// A bit-mask conveying the desired SZIP options
				szip_pixels_per_block );	// The number of pixels or data elements in each data block

		hDataSet = H5Dcreate(	hF,			// HDF文件标示
					strDsName,		// 要创建的数据集名称
					hType,			// 指定数据类型		
					hDsSpace,		// 数据空间
					hProperties );		// Dataset creation property list identifier
	} // if
	else // 不压缩
	{
		if ( bCreate ) 
		{
			hDataSet = H5Dcreate(hF, strDsName, hType, hDsSpace, H5P_DEFAULT); // 创建新的数据集
		}
		else
		{
			hDataSet = H5Dopen(hF, strDsName); // 修改数据集
		}
	}
	
	// Writes raw data from a buffer to a dataset //H5Dwrite(dataset, coordinate_tid, H5S_ALL, h_dataspace, H5P_DEFAULT, pPix)
	hStatus = H5Dwrite(	hDataSet,		// 数据集标识号
				hType,			// 内存数据类型标识号
				memspace,		// 内存数据空间
				hDsSpace,		// 文件数据空间
				H5P_DEFAULT,		// 属性列表
				pData );		// 内存数据

	H5Sclose(memspace);
	H5Sclose(hDsSpace);
	H5Dclose(hDataSet);		
	
	return SUCCESS;

} // WriteHdf5FileData2D

bool HDF5Func::WriteHdf5FileData1D(hid_t hF,
									 const char* strDsName, 
									 void *pData,int iDataType,
									 int iDataSize,
									 int iOutSize,
									 int iDimension,
									 bool isCompress/*=false*/,
									 int *iDataStart/* =NULL */,
									 int *iOutCount/* =NULL */,
									 int *iOutStart/* =NULL */)									 
{
// 	int i;
	/*参数检查*/
	if(strDsName==NULL||pData==NULL)
	{
		return false;
	}

	if (iDataType<0 || iDataType>MAXTYPENUM)
	{
		return false;
	}

	hid_t	hType;
	hid_t memspace;
	hsize_t dim[1] = { iOutSize} ;
	hsize_t outSt[1] = {0};
	hsize_t outCt[1] = { iDataSize};
	hsize_t fdim[1] = { iDataSize};
	hsize_t fstart[1] = { 0};
	hsize_t fcount[1] = { iDataSize};

	if (iDataStart!=NULL)
	{
		fstart[0] = iDataStart[0];		
	}	

	if (iOutCount!=NULL)
	{
		fcount[0] = iOutCount[0];		
		outCt[0] = iOutCount[0];		
	}
	
	if (iOutStart!=NULL)
	{
		outSt[0] = iOutStart[0];		
	}


	if(iDataSize<=0 || iOutSize<=0){			
		return false;
	}

	if(((fstart[0]+fcount[0]) > iDataSize) || ((outSt[0]+outCt[0])>iOutSize)){			
		return false;
	}


	hDsSpace = H5Screate_simple(1,dim,NULL);

	hStatus = H5Sselect_hyperslab(hDsSpace,H5S_SELECT_SET,outSt,NULL,outCt,NULL);
	
	memspace = H5Screate_simple(1,fdim,NULL);

	hStatus = H5Sselect_hyperslab(memspace,H5S_SELECT_SET,fstart,NULL,fcount,NULL);
	
	hType = getHdfDataType(iDataType);
	if (hType == -1)
	{
		return false;
	}

	if (isCompress)
	{
		hsize_t hChunk_size1[1];
		if (iOutSize>CPY)
		{
			hChunk_size1[0] = CPY;		
		}
		else
		{
			hChunk_size1[0] = iOutSize;
		}
		
		hProperties = H5Pcreate(H5P_DATASET_CREATE);
		H5Pset_chunk(hProperties,1,hChunk_size1);

		szip_options_mask=H5_SZIP_NN_OPTION_MASK;
		szip_pixels_per_block=32;
		H5Pset_szip (hProperties, szip_options_mask, szip_pixels_per_block);

		hDataSet = H5Dcreate(hF,strDsName,hType,hDsSpace,hProperties);	
	}
	else
	{
		hDataSet = H5Dcreate(hF,strDsName,hType,hDsSpace,H5P_DEFAULT);
	}
	
	hStatus = H5Dwrite(hDataSet,hType,memspace,hDsSpace,H5P_DEFAULT,pData);

	H5Sclose(memspace);
	H5Sclose(hDsSpace);
	H5Dclose(hDataSet);	
	
	return true;
}

bool HDF5Func::WriteHdf5FileData3D(hid_t hF,
									 const char* strDsName, 
									 void *pData,int iDataType,
									 int iDataSize[3],
									 int iOutSize[3],
									 int iDimension,
									 bool bFlag,
									 bool isCompress/* =false */,
									 int *iDataStart/* =NULL */,
									 int *iOutCount/* =NULL */,
									 int *iOutStart/* =NULL */)									 
{
	int i;
	/*参数检查*/
	if ( strDsName == NULL || pData == NULL )
	{
		return false;
	}

	if ( iDataType < 0 || iDataType > MAXTYPENUM ) // 给定的数据类型不正确
	{
		return false;
	}

	hid_t	hType;
	hid_t memspace;
	hsize_t dim[3] = { iOutSize[0], iOutSize[1],iOutSize[2] } ;	// 内存空间各维大小
	hsize_t outSt[3] = {0,0,0};
	hsize_t outCt[3] = { iDataSize[0], iDataSize[1],iDataSize[2]};
	hsize_t fdim[3] = { iDataSize[0], iDataSize[1],iDataSize[2] };	// 文件空间各维大小
	hsize_t fstart[3] = { 0, 0,0};
	hsize_t fcount[3] = { iDataSize[0], iDataSize[1],iDataSize[2]};

	if ( iDataStart != NULL )
	{
		fstart[0] = iDataStart[0];
		fstart[1] = iDataStart[1];
		fstart[2] = iDataStart[2];
	}	

	if ( iOutCount != NULL )
	{
		fcount[0] = iOutCount[0];
		fcount[1] = iOutCount[1];
		fcount[2] = iOutCount[2];

		outCt[0] = iOutCount[0];
		outCt[1] = iOutCount[1];
		outCt[2] = iOutCount[2];
	}
	
	if ( iOutStart != NULL )
	{
		outSt[0] = iOutStart[0];
		outSt[1] = iOutStart[1];
		outSt[2] = iOutStart[2];
	}

	for ( i = 0; i < 3; ++i )
	{
		if(iDataSize[i]<=0 || iOutSize[i]<=0)
		{			
			return false;
		}

		if(((fstart[i]+fcount[i]) > iDataSize[i]) || ((outSt[i]+outCt[i])>iOutSize[i])){			
			return false;
		}
	}
	// 创建一个数据集的步骤
	// 1) 定义一个数据类型
	// 2) 定义一个数据空间
	// 3) 创建一个属性列表
	// 4) 创建一个数据集

	// 创建数据空间
	hDsSpace = H5Screate_simple(	3,		// 数据集空间维数
					dim,		// 各维的长度
					NULL );		// 数据集大小的上限(指定为NULL则与dim指定的大小相同)

	hStatus = H5Sselect_hyperslab(hDsSpace, H5S_SELECT_SET, outSt, NULL, outCt, NULL);
	
	memspace = H5Screate_simple(3, fdim, NULL); // 创建内存数据空间

	hStatus = H5Sselect_hyperslab(memspace, H5S_SELECT_SET, fstart, NULL, fcount, NULL);
	
	hType = getHdfDataType(iDataType); // 获取数据类型
	if (hType == -1)
	{
		return false;
	}

	if (isCompress) // 需要将数据集压缩
	{
		hsize_t hChunk_size3[3];
		if (iOutSize[0]>CPZ)
		{
			hChunk_size3[0] = CPZ;
		}
		else
		{
			hChunk_size3[0] =iOutSize[0];
		}
		

		if (iOutSize[1]>CPY)
		{
			hChunk_size3[1] = CPY;
		}
		else
		{
			hChunk_size3[1] = iOutSize[1];
		}

		if (iOutSize[2]>CPX)
		{
			hChunk_size3[2] = CPX;
		}
		else
		{
			hChunk_size3[2] = iOutSize[2];
		}
		hProperties = H5Pcreate(H5P_DATASET_CREATE); // 创建属性
		H5Pset_chunk(hProperties, 3, hChunk_size3);
		//hStatus = H5Pset_deflate( hProperties, 6);

		szip_options_mask = H5_SZIP_NN_OPTION_MASK;
		szip_pixels_per_block = 32;
		H5Pset_szip (hProperties, szip_options_mask, szip_pixels_per_block);

		if ( bFlag )
		{
			hDataSet = H5Dopen(hF, strDsName); 
		}
		else
		{
			hDataSet = H5Dcreate(hF, strDsName, hType, hDsSpace, hProperties);	
		}
	} // if 
	else // 不需要将数据集压缩
	{
		if ( bFlag )
		{
			hDataSet = H5Dopen(hF, strDsName); // 打开指定数据集
		}
		else
		{	// 创建指定数据集
			hDataSet = H5Dcreate(	hF,			// HDF5文件
						strDsName,		// 数据集名称
						hType,			// 数据类型
						hDsSpace,		// 数据空间
						H5P_DEFAULT);		// 默认属性列表
		}
	}
	
	// 要完成写操作需要指定

	// 向指定数据集写入数据
	hStatus = H5Dwrite(	hDataSet,		// 数据集标识号
				hType,			// 内存数据类型
				memspace,		// 内存数据空间
				hDsSpace,		// 文件数据空间
				H5P_DEFAULT,		// 默认属性列表
				pData );		// 数据缓存

	H5Sclose(memspace);
	H5Sclose(hDsSpace); // 关闭数据空间
	H5Dclose(hDataSet); // 关闭数据集	
	
	return true;
} // WriteHdf5FileData3D

//外部使用：分配读取数据集的指针所用的内存
bool HDF5Func::mallocSdsBuf(const char* strInfName,const char* strSdsName,
							unsigned long &uDataSize,void *&pReturn)
{
	hsize_t	hDims[]={1,1,1};
	hsize_t	dimsw[3];	
	
	int	iDimensions=1;	/*HDF5数据维数*/
	int iDataType = -1;
	int iNBytes = -1;

	hid_t hFs = H5Fopen(strInfName,H5F_ACC_RDONLY,H5P_DEFAULT);
	if (hFs ==-1)
	{
		return false;
	}

	/*得到数据集信息*/
	hStatus = GetHdfSdsParam(hFs, strSdsName, &iDimensions, dimsw, iDataType,iNBytes);
	if (hStatus==-1)
	{
		H5Fclose(hFs);
		return false;
	}
	H5Fclose(hFs);

	uDataSize = 0;
	unsigned long uLen =0;
	if (iDimensions==2)
	{
		uLen = dimsw[0]*dimsw[1];		
	}
	else if (iDimensions == 3)
	{
		uLen = dimsw[1]*dimsw[2]*dimsw[0];
	}
	else if (iDimensions==1)
	{
		unsigned long uLen = dimsw[0];
	}
	else
	{
		cout << "Error hdf sds!" << endl;
		return false;
	}

	uDataSize = uLen*sizeof(char)*iNBytes;
	pReturn = new char[uDataSize];		
	if (pReturn==NULL)
	{
		cout << "Can not apply enough memory!" << endl;
		return false;
	}

	return true;
}

//内部使用：分配内存
bool HDF5Func::mallocHdfBuf(hid_t hF,const char* strDsName)
{
//	hid_t	hType;
	hsize_t	hDims[]={1,1,1};
	hsize_t	dimsw[3];	

	if(strDsName==NULL||pData==NULL)
	{
		return false;
	}
	
//	if(access(strFileName,0)!=0)	/*检测文件是否存在*/
//	{
//		return false;
//	}
//
//	if(H5Fis_hdf5(strFileName)==-1)
//	{
//		return false;
//	}

	int	iDimensions=1;	/*HDF5数据维数*/
	int iDataType = -1;
	int iNBytes = -1;
	/*得到数据集信息*/
	hStatus = GetHdfSdsParam(hF, strDsName, &iDimensions, dimsw, iDataType,iNBytes);
	if (hStatus==-1)
	{
		return false;
	}

	if (iDimensions==2)
	{
		delete []pData;
		pData = NULL;
		
		unsigned long uLen = dimsw[0]*dimsw[1];
		pData = new char[uLen*sizeof(char)*iNBytes];		
		if (pData==NULL)
		{
			cout << "Can not apply enough memory!" << endl;
			return false;
		}
	}
	else if (iDimensions == 3)
	{
		delete []pData;
		pData = NULL;

		unsigned long uLen = dimsw[1]*dimsw[2]*dimsw[0];
		pData = new char[uLen*sizeof(char)*iNBytes];	
		if (pData==NULL)
		{
			cout << "Can not apply enough memory!" << endl;
			return false;
		}
	}
	else if (iDimensions==1)
	{
		delete []pData;
		pData = NULL;

		unsigned long uLen = dimsw[0];
		pData = new char[uLen*sizeof(char)*iNBytes];
		if (pData==NULL)
		{
			cout << "Can not apply enough memory!" << endl;
			return false;
		}
	}
	else
	{
		delete []pData;
		pData = NULL;
		return false;
	}

	return true;
}

bool HDF5Func::getAttrOne(hid_t hF, string strDsName, attrOne &aG)
{
	if (hF<0 || aG.strName=="" || aG.iArraySize<=0 || aG.iDataType<0 || aG.iDataType>MAXTYPENUM )
	{
		return false;
	}

	if (aG.pData!=NULL)
	{
		delete []aG.pData;
		aG.pData = NULL;
	}
	
	switch(aG.iDataType) // 根据数据类型标号
	{
	case UNCHARTYPE:
		{
			aG.pData = new unsigned char[aG.iArraySize];
			hStatus = H5LTget_attribute_uchar(hF,strDsName.c_str(),aG.strName.c_str(),(unsigned char*)aG.pData);  
			break;
		}
	case CHARTYPE:
		{
			aG.pData = new char[aG.iArraySize];
			hStatus = H5LTget_attribute_char(hF,strDsName.c_str(),aG.strName.c_str(),(char*)aG.pData);  
			break;
		}
	case UNSHORTTYPE:
		{
			aG.pData = new unsigned short int[aG.iArraySize];
			hStatus = H5LTget_attribute_ushort(hF,strDsName.c_str(),aG.strName.c_str(),(unsigned short int*)aG.pData);  
			break;
		}
	case SHORTTYPE:
		{
			aG.pData = new short int[aG.iArraySize];
			hStatus = H5LTget_attribute_short(hF,strDsName.c_str(),aG.strName.c_str(),(short int*)aG.pData);  
			break;
		}
	case UNINTTYPE:
		{
			aG.pData = new unsigned int[aG.iArraySize];
			hStatus = H5LTget_attribute_uint(hF,strDsName.c_str(),aG.strName.c_str(),(unsigned int*)aG.pData);  
			break;
		}
	case INTTYPE:
		{
			aG.pData = new int[aG.iArraySize];
			hStatus = H5LTget_attribute_int(hF,strDsName.c_str(),aG.strName.c_str(),(int*)aG.pData);  
			break;
		}
	case UNINT64TYPE:
		{
			aG.pData = new unsigned long[aG.iArraySize];
			hStatus = H5LTget_attribute_ulong(hF, strDsName.c_str(), aG.strName.c_str(), (unsigned long*)aG.pData);  
			break;
		}
	case INT64TYPE:
		{
			aG.pData = new long[aG.iArraySize];
			hStatus = H5LTget_attribute_long(hF,strDsName.c_str(),aG.strName.c_str(),(long*)aG.pData);  
			break;
		}
	case FLOATTYPE:
		{
			aG.pData = new float[aG.iArraySize];
			hStatus = H5LTget_attribute_float(hF,strDsName.c_str(),aG.strName.c_str(),(float*)aG.pData);  
			break;
		}
	case DOUBLETYPE:
		{
			aG.pData = new double[aG.iArraySize];
			hStatus = H5LTget_attribute_double(hF,strDsName.c_str(),aG.strName.c_str(),(double*)aG.pData);  
			break;
		}
	case STRINGTYPE:
		{
			aG.pData = new char[aG.iArraySize];
			hStatus = H5LTget_attribute_string(hF,strDsName.c_str(),aG.strName.c_str(),(char*)aG.pData);  			
			break;
		}
	default:
		return false;
	} // switch

	return true;
}

bool HDF5Func::setAttrInfo(hid_t hF,string strDsName,const vector<attrOne> &vAttrD)
{
	if (hF<0 || vAttrD.size()<=0)
	{
		return false;
	}

	int i;
	for (i=0;i<vAttrD.size();++i)
	{
		attrOne aG = vAttrD[i];

		switch(aG.iDataType) {
		case UNCHARTYPE:
		{			
			hStatus = H5LTset_attribute_uchar(hF,strDsName.c_str(),aG.strName.c_str(),(const unsigned char*)aG.pData,aG.iArraySize);			
			break;
		}
		case CHARTYPE:
		{
			hStatus = H5LTset_attribute_char(hF,strDsName.c_str(),aG.strName.c_str(),(const char*)aG.pData,aG.iArraySize);			
			break;
		}
		case UNSHORTTYPE:
		{
			hStatus = H5LTset_attribute_ushort(hF,strDsName.c_str(),aG.strName.c_str(),(const unsigned short*)aG.pData,aG.iArraySize);			
			break;
		}
		case SHORTTYPE:
		{
			hStatus = H5LTset_attribute_short(hF,strDsName.c_str(),aG.strName.c_str(),(const short*)aG.pData,aG.iArraySize);			
			break;
		}
		case UNINTTYPE:
		{
			hStatus = H5LTset_attribute_uint(hF,strDsName.c_str(),aG.strName.c_str(),(const unsigned int*)aG.pData,aG.iArraySize);			
			break;
		}
		case INTTYPE:
		{
			hStatus = H5LTset_attribute_int(hF,strDsName.c_str(),aG.strName.c_str(),(const int*)aG.pData,aG.iArraySize);			
			break;
		}
		case UNINT64TYPE:
		{
			hStatus = H5LTset_attribute_ulong(hF,strDsName.c_str(),aG.strName.c_str(),(const unsigned long*)aG.pData,aG.iArraySize);			
			break;
		}
		case INT64TYPE:
		{
			hStatus = H5LTset_attribute_long(hF,strDsName.c_str(),aG.strName.c_str(),(const long*)aG.pData,aG.iArraySize);			
			break;
		}
		case FLOATTYPE:
		{
			hStatus = H5LTset_attribute_float(hF,strDsName.c_str(),aG.strName.c_str(),(const float*)aG.pData,aG.iArraySize);			
			break;
		}
		case DOUBLETYPE:
		{
			hStatus = H5LTset_attribute_double(hF,strDsName.c_str(),aG.strName.c_str(),(const double*)aG.pData,aG.iArraySize);			
			break;
		}
		case STRINGTYPE:
		{
			hStatus = H5LTset_attribute_string(hF,strDsName.c_str(),aG.strName.c_str(),(const char*)aG.pData);			
			break;
		}
		default:
			return false;
		}
	}

	return true;
}

bool HDF5Func::getAllDsNames(hid_t hF,vector<string> &vDsNs)
{
	hsize_t hDsCount;

	//返回整个文件中的数据集的个数
	hStatus = H5Gget_num_objs(hF, &hDsCount);
	
	int iDsCount=(int)hDsCount;	
	int i = 0;
	for (i=0;i<iDsCount;++i)
	{
		//获取各个数据集的名字
		// H5G_obj_t type=H5Gget_objtype_by_idx(hF,i);
		ssize_t sSizet =  H5Gget_objname_by_idx(hF,i,pAttrTmp,100);
		string strDsTmp = pAttrTmp;
		vDsNs.push_back(strDsTmp);
	}

	return true;
}

bool HDF5Func::getAttrGlobeInfo(hid_t hF,vector<attrOne> &vAttrG)
{
	int i = 0;
	
	for (i=0;i<MAXATTR;++i)
	{
		hAttr = H5Aopen_idx(hF,i);
		if (hAttr<0)
		{
			break;
			return true;
		}
		
		attrOne attrTmp;

		//获取属性的名字 
		ssize_t _hName =  H5Aget_name(hAttr , 100, pAttrTmp);		
		attrTmp.strName = pAttrTmp;

		hsize_t _hSize =  H5Aget_storage_size(hAttr);//总字节数

		hid_t _hType =  H5Aget_type(hAttr);	
		int _iNBytes;
		if (!getDataType(_hType,attrTmp.iDataType,_iNBytes))
		{
			return false;
		}
		attrTmp.iArraySize = (int)_hSize/_iNBytes;//数组大小
		attrTmp.iByteLen = (int)_hSize;

		//获取全局属性
		if (!getAttrOne(hF,"/",attrTmp))
		{
			return false;
		}
				
		vAttrG.push_back(attrTmp);

		H5Aclose(hAttr);
	}
	
	return true;
}

bool HDF5Func::getAttrDsInfo(hid_t hF,string strDsName,vector<attrOne> &vAttrD)
{
	//获取某个数据集的属性 
	int i = 0;
	hDataSet = H5Dopen(hF, strDsName.c_str()); // 打开指定的数据集
	if ( hDataSet < 0 )
	{
		return false;
	}

	//对应的数据集属性的个数
	int  _iNumAttr =  H5Aget_num_attrs(hDataSet);
	if ( _iNumAttr <= 0 ) // 数据集属性小于0
	{
		return false;
	}
	
	for ( i = 0; i < (int)_iNumAttr; ++i ) // 遍历每个数据集属性
	{
		attrOne aTmp; // 数据集属性

		hAttr = H5Aopen_idx(hDataSet, i);
		ssize_t _hName =  H5Aget_name(hAttr , 100, pAttrTmp);		
		aTmp.strName = pAttrTmp;		

		hsize_t _hSize =  H5Aget_storage_size(hAttr);
		hid_t _hType =  H5Aget_type(hAttr);
		
		int _iNBytes;
		if (!getDataType(_hType,aTmp.iDataType,_iNBytes))
		{
			return false;
		}

		aTmp.iByteLen = (int)_hSize;
		aTmp.iArraySize = (int)_hSize/_iNBytes;

		if (!getAttrOne(hF,strDsName,aTmp))
		{
			return false;
		}

		//注意：vector<attrOne>: 要求容器元素要具有“值语义”
		//attrOne中具有指针，所以，需要重新构造拷贝构造函数和重载运算符
		vAttrD.push_back(aTmp);

		H5Aclose(hAttr);
	} // for

	return true;
}

//bool HDF5Func::getAttrAllInfo(string strFileName,vector<attrOne> &vAttrG,vector<vector<attrOne> > &vAttrD )
//{
//	hFile = H5Fopen(strFileName.c_str(),H5F_ACC_RDONLY,H5P_DEFAULT);
//	if (hFile ==-1)
//	{
//		return false;
//	}
//	int i;	
//
//	if (!getAllDsNames(hFile,vDsNames))
//	{
//		return false;
//	}
//
//	if (!getAttrGlobeInfo(hFile,vAttrG))
//	{
//		return false;
//	}
//	
//	for (i=0;i<vDsNames.size();++i)
//	{
//		vector<attrOne> vDs;
//		if (!getAttrDsInfo(hFile,vDsNames[i],vDs))
//		{
//			return false;
//		}
//
//		vAttrD.push_back(vDs);
//	}	
//
//	H5Fclose(hFile);
//
//	return true;
//}
//
//bool HDF5Func::setAttrAllInfo(string strOutFile,
//								  vector<string> vDsN,
//								  const vector<attrOne> &vAttrG,
//								  const vector<vector<attrOne> > &vAttrD)
//{
//	hFile = H5Fopen(strOutFile.c_str(),H5F_ACC_RDWR,H5P_DEFAULT);
//	if (hFile ==-1)
//	{
//		return false;
//	}
//	int i;
//
//	if (!setAttrInfo(hFile,"/",vAttrG))
//	{
//		return false;
//	}	
//
//	for (i=0;i<vAttrD.size();++i)
//	{
//		if (!setAttrInfo(hFile,vDsN[i],vAttrD[i]))
//		{
//			return false;
//		}
//	}
//
//	H5Fclose(hFile);
//
//	return true;
//}

bool HDF5Func::getSetAttrInfo(hid_t hInFile,string strDsName,hid_t hOutFile)
{
	vector<attrOne> vDs;
	
	if (strDsName=="/")
	{
		if (!getAttrGlobeInfo(hInFile,vDs))
		{
			return false;
		}
	}
	else
	{
		if (!getAttrDsInfo(hInFile,strDsName,vDs))
		{
			return false;
		}
	}

	if (!setAttrInfo(hOutFile,strDsName,vDs))
	{
		return false;
	}

	return true;
}

int HDF5Func::createOutFile(const string fileName)
{
	hid_t file;
	file = H5Fcreate(fileName.c_str(),H5F_ACC_TRUNC,H5P_DEFAULT,H5P_DEFAULT);
	if (file<0)
	{
		return FALSE;
	}

	H5Fclose(file);

	return SUCCESS;
}

bool HDF5Func::hdfReadToBuf(	hid_t dset_id, 
							 hid_t mem_type_id,
							 hid_t mem_space_id,
							 hid_t file_space_id,
							 hid_t plist_id, 
							 void *buf, 
							 int iDataType)
{
	switch(iDataType) {
		case UNCHARTYPE:
			{
				unsigned char *pData = (unsigned char*)buf;
				hStatus = H5Dread (dset_id, mem_type_id, mem_space_id, file_space_id, plist_id, pData);
				break;
			}
		case CHARTYPE:
			{
				char *pData = (char*)buf;
				hStatus = H5Dread (dset_id, mem_type_id, mem_space_id, file_space_id, plist_id, pData);
				break;
			}
		case UNSHORTTYPE:
			{
				unsigned short int *pData = (unsigned short int*)buf;
				hStatus = H5Dread (dset_id, mem_type_id, mem_space_id, file_space_id, plist_id, pData);
				break;
			}
		case SHORTTYPE:
			{
				short int *pData = (short int*)buf;
				hStatus = H5Dread (dset_id, mem_type_id, mem_space_id, file_space_id, plist_id, pData);
				break;
			}
		case UNINTTYPE:
			{
				unsigned int *pData = (unsigned int*)buf;
				hStatus = H5Dread (dset_id, mem_type_id, mem_space_id, file_space_id, plist_id, pData);
				break;
			}
		case INTTYPE:
			{
				int *pData = (int*)buf;
				hStatus = H5Dread (dset_id, mem_type_id, mem_space_id, file_space_id, plist_id, pData);
				break;
			}
		case UNINT64TYPE:
			{
				unsigned long *pData = (unsigned long*)buf;
				hStatus = H5Dread (dset_id, mem_type_id, mem_space_id, file_space_id, plist_id, pData);
				break;
			}
		case INT64TYPE:
			{
				long *pData = (long*)buf;
				hStatus = H5Dread (dset_id, mem_type_id, mem_space_id, file_space_id, plist_id, pData);
				break;
			}
		case FLOATTYPE:
			{
				float *pData = (float*)buf;
				hStatus = H5Dread (dset_id, mem_type_id, mem_space_id, file_space_id, plist_id, pData);
				break;
			}
		case DOUBLETYPE:
			{
				double *pData = (double*)buf;
				hStatus = H5Dread (dset_id, mem_type_id, mem_space_id, file_space_id, plist_id, pData);
				break;
			}
	default:
		return false;
	}
	return true;
}

int HDF5Func::GetHdfHW(const char* strInfName,const char* strSDSName,int &iH,int &iW,int &iD,unsigned long &uDataSize)
{
	hid_t hInFile = H5Fopen(strInfName,H5F_ACC_RDONLY,H5P_DEFAULT);
	
	if (hInFile ==-1)
	{
		return FALSE;
	}

	int iNBytes = -1;
	int	iDimensions=1;	/*HDF5数据维数*/
	int	iDataType=1;	/*HDF5数据类型*/
	hsize_t	hDimValue[]={1,1,1};

	if (GetHdfSdsParam(hInFile, strSDSName, &iDimensions, hDimValue, iDataType, iNBytes)==-1)
	{
		H5Fclose(hInFile);
		return FALSE;
	}
	// 根据返回的数据集维数获取其宽/高/深
	if (iDimensions==2) // 2维
	{
		iH = hDimValue[0];
		iW = hDimValue[1];
		iD = 1;
		uDataSize = iH*iW*sizeof(char)*iNBytes;
	}
	else if ( iDimensions==3 ) // 3维
	{
		iD = hDimValue[0];
		iH = hDimValue[1];
		iW = hDimValue[2];
		uDataSize = iH*iW*iD*sizeof(char)*iNBytes;
	}
	else if (iDimensions==1) // 1维
	{
		iD = 1;
		iH = hDimValue[0];
		iW = 1;
		uDataSize = iH*iW*iD*sizeof(char)*iNBytes;
	}
	else // 其余维数
	{
		iH = iW = iD = 0;
	}	
//	H5Fclose(hInFile);
	return SUCCESS;
}

int HDF5Func::GetHdfHW(hid_t hInFile,const char* strSDSName,int &iH,int &iW,int &iD,unsigned long &uDataSize)
{	
	int iNBytes = -1;
	int	iDimensions=1;	/*HDF5数据维数*/
	int	iDataType=1;	/*HDF5数据类型*/
	hsize_t	hDimValue[]={1,1,1};
	
	if (GetHdfSdsParam(hInFile, strSDSName, &iDimensions, hDimValue, iDataType, iNBytes)==-1)
	{
		H5Fclose(hInFile);
		return FALSE;
	}
	
//	H5Fclose(hInFile);
	
	// 根据返回的数据集维数获取其宽/高/深
	if (iDimensions==2) // 2维
	{
		iH = hDimValue[0];
		iW = hDimValue[1];
		iD = 1;
		uDataSize = iH*iW*sizeof(char)*iNBytes;
	}
	else if ( iDimensions==3 ) // 3维
	{
		iD = hDimValue[0];
		iH = hDimValue[1];
		iW = hDimValue[2];
		uDataSize = iH*iW*iD*sizeof(char)*iNBytes;
	}
	else if (iDimensions==1) // 1维
	{
		iD = 1;
		iH = hDimValue[0];
		iW = 1;
		uDataSize = iH*iW*iD*sizeof(char)*iNBytes;
	}
	else // 其余维数
	{
		iH = iW = iD = 0;
	}	
	
	return SUCCESS;
}

bool HDF5Func::GetHdfSdsParam(const char* strInfName, const char* strSDSName,
							  int *iDimensions, hsize_t *hDimValue, 
							  int &iDataType,int &iNBytes)
{
	hid_t hInFile = H5Fopen(strInfName,H5F_ACC_RDONLY,H5P_DEFAULT);

	if (hInFile ==-1)
	{
		return false;
	}

	if ( GetHdfSdsParam(hInFile, strSDSName, iDimensions, hDimValue, iDataType, iNBytes) == -1 )
	{
		H5Fclose(hInFile);
		return false;
	}

	H5Fclose(hInFile);

	return true;
}
/*:****************************xxx   GetHdfSdsParam   xxx*******************************/
/*  函数名称：GetHdfSdsParam								*/
/*  函数标题：中间函数									*/
/*  函数功能：得到数据集信息（维数、大小、数据类型）					*/
/*  调用方法：status = int GetHdfSdsParam(char *strHDFFile, 				*/
/*					char* strSDSName, 				*/
/*					int *iDimensions, 				*/
/*					hsize_t *hDimValue,				*/
/*					int *iDataType)					*/
/*  参数名			I/O	参数描述					*/
/*   strHDFFile			输入	HDF5文件名					*/
/*   strSDSName			输入	数据集名称					*/
/*   iDimensions		输出	维数						*/
/*   hDimValue			输出	各维大小					*/
/*   iDataType			输出	数据类型					*/
/*   status			输出	返回值0和-1					*/
/*  作    者：刘炜  								*/
/*  完成日期：2008年4月8日								*/
/***************************  xxxxxx  函数代码 xxxxx **********************************:*/
int HDF5Func::GetHdfSdsParam(	  hid_t hF,
								  const char* strSDSName, 
								  int *iDimensions, 
								  hsize_t *hDimValue, 
								  int &iDataType,
								  int &iNBytes)
{
	hid_t		hFileID = hF;         /*HDF5文件标志符*/
	hid_t		hDSpaceID;       /*HDF5数据空间对象标志符*/
	hid_t		hDatasetID;      /*HDF5科学数据集标志符*/	
	hid_t		hType;           /*HDF5数据类型标志符*/
	H5T_class_t	hTypeClass;      /*HDF5数据类型对象标志符*/
	size_t		hSize;           /*HDF5数据类型字节大小*/
	H5T_sign_t	hSign;           /*HDF5数据类型有无符号*/
	hsize_t		hMaxDimValue[3]; /*每维最大的大小*/
	
	hDimValue[0]=1;
	hDimValue[1]=1;
	hDimValue[2]=1;
	
	if (strSDSName==NULL)
		return -1;
		
//	if (access(strHDFFile,0)!=0)
//		return -1;
//
//	/*判断是否为HDF5文件*/
//	if(H5Fis_hdf5(strHDFFile)==-1)
//		return -1;

//	/*打开HDF文件*/
//	hFileID  = H5Fopen(strHDFFile, H5F_ACC_RDONLY, H5P_DEFAULT);
//	if(hFileID < 0)	/*打开文件失败*/		
//	{
//		return -1;
//	}

	/*判断数据集是否存在*/
	if(_IsObjExist(hFileID,(char*)strSDSName) == 0)
	{
		/*如果不是hdf5文件*/		
		*iDimensions=-1;
		return -1;
	}
	/*打开科学数据集*/
	hDatasetID = H5Dopen(hFileID, strSDSName);
	if(hDatasetID < 0) /*打开失败*/
	{		
		return -1;
	}

	/*取得科学数据集的空间对象*/
	hDSpaceID= H5Dget_space(hDatasetID);
	if(hDSpaceID < 0)
	{
		H5Dclose(hDatasetID);		
		return -1;
	}
	
	/*取得科学数据集的数据类型*/
	//if (iDataType!=NULL)
	//{
		hType = H5Dget_type(hDatasetID);		
		if(hType < 0)
		{
			H5Dclose(hDatasetID);			
			return -1;
		}
		else
		{
			/*得到数据类型类标志符*/
			hTypeClass = H5Tget_class(hType);
			if(hTypeClass == H5T_NO_CLASS)/*如果类型错误*/
			{
				H5Dclose(hDatasetID);				
				return -1;
			}
			if(hTypeClass == H5T_INTEGER) /*整型时*/
			{
				/*得到数据类型字节大小*/
				hSize = H5Tget_size(hType);
				if(hSize == 0)/*如果字节为0*/
				{
					H5Dclose(hDatasetID);					
					return -1;
				}
				/*得到数据是否有符号*/
				hSign = H5Tget_sign(hType);
				if(hSign == H5T_SGN_ERROR)/*如果符号错误*/
				{
					H5Dclose(hDatasetID);					
					return -1;
				}
				if(hSize == 1 && hSign == H5T_SGN_NONE) 
				{
					iDataType = UNCHARTYPE;/*1字节无符号整型*/
					iNBytes = 1;
				}
				if(hSize == 1 && hSign == H5T_SGN_2)   
				{
					iDataType = CHARTYPE;/*1字节有符号整型*/
					iNBytes = 1;
				}
				if(hSize == 2 && hSign == H5T_SGN_NONE) 
				{
					iDataType = UNSHORTTYPE;/*2字节无符号整型*/
					iNBytes = 2;
				}
				if(hSize == 2 && hSign == H5T_SGN_2) 
				{
					iDataType = SHORTTYPE;/*2字节有符号整型*/
					iNBytes = 2;
				}
				if(hSize == 4 && hSign == H5T_SGN_NONE) 
				{
					iDataType = INTTYPE;/*4字节无符号整型*/
					iNBytes = 4;
				}
				if(hSize == 4 && hSign == H5T_SGN_2)    
				{
					iDataType = UNINTTYPE;/*4字节有符号整型*/
					iNBytes = 4;
				}
				if(hSize == 8 && hSign == H5T_SGN_NONE) 
				{
					iDataType = UNINT64TYPE;/*8字节无符号整型*/
					iNBytes = 8;
				}
				if(hSize == 8 && hSign == H5T_SGN_2)    
				{
					iDataType = INT64TYPE;/*8字节有符号整型*/		
					iNBytes = 8;
				}
			}
			else if(hTypeClass == H5T_FLOAT)/*浮点型时*/
			{
				/*得到数据类型字节大小*/
				hSize = H5Tget_size(hType);
				if(hSize == 0)/*如果字节为0*/
				{
					H5Dclose(hDatasetID);					
					return -1;
				}
				if(hSize == 4) 
				{
					iDataType = FLOATTYPE;/*4字节浮点型*/
					iNBytes = 4;
				}
				else
				{
					iDataType = DOUBLETYPE;/*8字节双精度型*/
					iNBytes = 8;
				}
			}		
			else/*对其他类型不作处理*/
			{
				H5Dclose(hDatasetID);				
				return -1;
			}
	
		}
		H5Tclose(hType);
	//}

	/*取得空间对象的数据维数*/
	if (iDimensions!=NULL)
	{
		*iDimensions = H5Sget_simple_extent_ndims(hDSpaceID);
		if(*iDimensions < 0)
		{
			H5Dclose(hDatasetID);			
			return -1;
		}
	}
	/*取得空间对象的每维大小*/
	if (hDimValue!=NULL)
	{
		if(H5Sget_simple_extent_dims(hDSpaceID, hDimValue, hMaxDimValue) <0)
		{
			H5Dclose(hDatasetID);			
			return -1;
		}
	}

	/*关闭所有打开的对象*/
	H5Dclose(hDatasetID);	

	return 0;
}

/*#include <string.h>
#include <afx.h>
bool HDF5Func::GetNames(string strFolder,vector<string> &vFileName)
{
	WIN32_FIND_DATA   FindData;   
	string strFilter = strFolder+"\\*.HDF";	
	HANDLE   FileHandle=FindFirstFile(strFilter.c_str(),&FindData);//   查找第一个   
	
	if(INVALID_HANDLE_VALUE!=FileHandle)//   查找成功   
	{   
	  if(16!=FindData.dwFileAttributes)//   不是目录是文件   
	  {   
		  //   在这利用文件名FindData.cFileName对文件操作   
		  string strTmp =  FindData.cFileName;
		  vFileName.push_back(strTmp);
	  }   

	  while(FindNextFile(FileHandle,&FindData))//   继续查找所有的   
	  {   
		  if(16!=FindData.dwFileAttributes)//   不是目录是文件   
		  {   
			  //   在这利用文件名FindData.cFileName对文件操作   
			  string strTmp =  FindData.cFileName;
			  vFileName.push_back(strTmp);
		  }   
	  }   
	  FindClose(FileHandle);//   关闭   
	}

	return true;
}
*/

//bool HDF5Func::HDF5Open(const char* strInfName)
//{
//	m_hFile = H5Fopen(strInfName, H5F_ACC_RDONLY, H5P_DEFAULT);
//
//	if ( m_hFile == -1 )
//	{
//		return false;
//	}
//
//	return true;
//
//} // HDF5Open
//
//int HDF5Func::ReadOneDimDataSet(const char* strDatasetName,
//				void* pData,
//				hsize_t nDepthStart,		// 
//				hsize_t nHighStart,		// 
//				hsize_t nWidthStart,		//
//				int nLength,			// 长
//				int nWidth,			// 宽
//				int nHeight )
//{
//	if ( NULL == strDatasetName )
//	{
//		return -1;
//	}
//
//	if ( NULL == pData )
//	{
//		return -2;
//	}
//	
//	//bool HDF5Func::ReadHdf5FileData(hid_t hF,
//	//	const char* strDsName, 
//	//	void *pData ,
//	//	hsize_t* iStart/* =NULL */,
//	//	int iLength/* =0 */, 
//	//	int iWidth/* =0 */,
//	//	int iHeight/* =0 */)
//	//{
//		/*变量定义及初始化*/	
//		hid_t	hType;
//		hsize_t	hDims[]={1,1,1};
//		hsize_t	dimsw[3];
//		hid_t	RANK;	
//		hid_t	memspace;
//
//		int	iDimensions = 1;	/*HDF5一维数据集*/
//		int	iDataType = 1;		/*HDF5数据类型*/
//
//		///*参数检查*/
//		//if(strDsName==NULL)
//		//{
//		//	cout << "SDS name is null"<<endl;
//		//	return false;
//		//}
//		//if (pData==NULL)
//		//{
//		//	cout << "data pointer is null" << endl;
//		//	return false;
//		//}
//
//		//	if(access(strFileName,0)!=0)	/*检测文件是否存在*/
//		//	{
//		//		return false;
//		//	}
//		//
//		//	if(H5Fis_hdf5(strFileName)==-1)
//		//	{
//		//		return false;
//		//	}
//
//		/*得到指定数据集信息*/
//		int iNBytes = 0;
//		hStatus = GetHdfSdsParam(	m_hFile,		// HDF5文件句柄
//						strDatasetName,		// 数据集名称
//						&iDimensions,		// 数据维数(输出)
//						dimsw,			// 各维大小(输出)
//						iDataType,		// 数据类型(输出)
//						iNBytes );
//		if ( hStatus == -1 )
//		{
//			return false;
//		}
//
//		//给全局变量赋值
//		int i = 0;
//		iDsDimension = iDimensions;
//		iDsDataType = iDataType;
//		for ( i = 0; i < 3; ++i )
//		{
//			iOneDataSize[i] = (int)dimsw[i];
//		}
//
//		hType = getHdfDataType(iDataType); // 根据数据类型返回
//		if (hType == -1)
//		{
//			return false;
//		}
//
//		/*打开指定的数据集*/
//		hDataSet = H5Dopen(m_hFile, strDatasetName);
//		if ( hDataSet == -1 )
//		{
//			return false;
//		}
//
//		if ( )
//
//		hsize_t iStart[3] = { 0 };
//
//		//将数据读入到pData中
//		/*读取数据集数据*/
//		if ( iStart != NULL )	/*读局部数据块*/
//		{
//			if (iStart[0]<0||iStart[1]<0||iStart[2]<0||iLength<=0||iWidth<=0||iHeight<=0)
//			{			
//				H5Dclose(hDataSet);
//				return false;			//读取科学数据参数错误，越界。
//			}
//
//			if(iStart[0]+iLength>dimsw[0])//深度
//				hDims[0]=dimsw[0]-iStart[0];
//			else
//				hDims[0]=iLength;
//
//			if(iStart[2]+iWidth>dimsw[2])//宽度
//				hDims[2]=dimsw[2]-iStart[2];
//			else
//				hDims[2]=iWidth;
//
//			if (iStart[1]+iHeight>dimsw[1])//高度
//				hDims[1]=dimsw[1]-iStart[1];
//			else
//				hDims[1]=iHeight;
//
//			hDsSpace= H5Dget_space(hDataSet);
//			RANK=H5Sget_simple_extent_ndims(hDsSpace);	//得到数据集维数
//			hStatus = H5Sselect_hyperslab (hDsSpace, H5S_SELECT_SET, iStart, NULL,hDims, NULL);  
//			memspace = H5Screate_simple (RANK, hDims, NULL); 
//
//			// 		hStatus = H5Dread (hDataSet, hType, memspace, hDsSpace, H5P_DEFAULT, pData);
//			hdfReadToBuf(hDataSet, hType, memspace, hDsSpace, H5P_DEFAULT, pData,iDataType);
//
//			H5Sclose(hDsSpace);
//			H5Dclose(hDataSet);
//			H5Sclose(memspace);
//		}
//		else	/*读全部数据*/
//		{
//			//		unsigned short int* pBuf = (unsigned short int*)pData;
//			//		hStatus = H5Dread (hDataSet, hType, H5S_ALL, H5S_ALL, H5P_DEFAULT, pBuf);
//			hdfReadToBuf(hDataSet, hType, H5S_ALL, H5S_ALL, H5P_DEFAULT, pData,iDataType);
//
//			H5Dclose(hDataSet);				
//		}
//
//} // ReadDataSet
