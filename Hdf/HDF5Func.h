// HDF5Compress.h: interface for the HDF5Func class.
//
//////////////////////////////////////////////////////////////////////
#ifndef _HDF5_FUNC_H
#define _HDF5_FUNC_H

#include "Common.h"
#include "SatelliteInfo.h"
//#include "Enhance.h"

#define NX 6000
#define NY 4000
//#define CPX 600
//#define CPY 400
#define CPX 400
#define CPY 400
#define CPZ 400
#define MAXATTR 100

//经验证，压缩的时候，如果数据的数值比较小，则压缩后所占用的空间比较小
//读取压缩后的HDF文件和未压缩的HDF文件之间没有差别
class HDF5Func  
{
public:
	HDF5Func();
	virtual ~HDF5Func();
public:

	//// 打开指定的HDF5文件
	//bool HDF5Open(const char* strInfName);


	//// 读HDF5中指定的一维记录集的数据
	//int ReadOneDimDataSet(	const char* strDatasetName,	// 数据集名称
	//			void* pData,			// 数据
	//			hsize_t nDepthStart,		// 深度方向的起始位置
	//			hsize_t nHighStart,		// 高度方向的起始位置
	//			hsize_t nWidthStart,		// 宽度方向的起始位置
	//			int nLength,			// 长
	//			int nWidth,			// 宽
	//			int nHeight);			// 高








	/**	ReadHdf5FileData
	 *	外部读取HDF5文件的数据
	 *	@strInfName	Hdf文件
	 *	@strDsName	SDS name
	 *	@pData		指向HDF5数据指针
	 */
	int ReadHdf5FileData(const char* strInfName,const char* strDsName, 
		void *pData);

	/**	GetHdfSdsParam
	 *	获取HDF5文件数据集相关的参数
	 *	@strInfName 	HDF5文件路径
	 *	@strSDSName	数据集的名称
	 *	@iDimensions	数据集的维数(返回)
	 *	@hDimValue	各维的大小(返回)
	 *	@iDataType	指定数据集的数据类型(返回)
	 *	@iNBytes	数据集数据类型的大小(字节)
	 */
	bool GetHdfSdsParam(const char* strInfName, const char* strSDSName, 
				int *iDimensions, hsize_t *hDimValue, int &iDataType,int &iNBytes);

	/**	GetHdfHW
	 *	根据HDF5文件和某个数据集名称获取H高度/W宽度/D深度/size
	 *	@strInfName HDF5文件路径
	 *	@strSDSName HDF5的数据集名称
	 *	@iH	    指定数据集的高度
	 *	@iW	    指定数据集的宽度
	 *	@iD	    指定数据集的深度
	 *	@uDataSize	指定数据集的大小
	 */
	int GetHdfHW(const char* strInfName, const char* strSDSName, int &iH, int &iW, int &iD, unsigned long &uDataSize);
	int GetHdfHW(hid_t hInFile,const char* strSDSName,int &iH,int &iW,int &iD,unsigned long &uDataSize);

	/**	mallocSdsBuf
	 *	根据Sds数据集相关参数分配缓冲区
	 *	@strInfName	信息名称
	 *	@strSdsName	SDS 名称
	 *	@uDataSize	分配空间的字节数
	 *	@pReturn	返回分配空间的指针
	 *	
	 */
	bool mallocSdsBuf(const char* strInfName,const char* strSdsName,
		unsigned long &uDataSize,void *&pReturn);
public:
	
	/**	CompressHDFFile
	 *	压缩HDF5文件
	 *	@strInFileFolder	文件夹路径
	 *	@strInFileName		文件名称
	 *	@strOutFileFolder	输出文件夹路径
	 */
	bool CompressHDFFile(string strInFileFolder, string strInFileName,
			    string strOutFileFolder);	


	//获取HDF5文件全局属性
	bool getSetAttrInfo(hid_t hInFile,string strDsName,hid_t hOutFile);

	/**	读取HDF5文件数据
	 *	@hF			HDF5文件句柄
	 *	@strDsName		数据集名称
	 *	@pData			数据指针
	 *	@iStart			开始位置(高/宽/深)
	 *	@iHeight		高度
	 *	@iWidth			宽度
	 *	@iDepth			深度
	 */
	int ReadHdf5FileData(	hid_t hF, 
				const char* strDsName,
				void *pData,
				hsize_t* iStart = NULL, // iStart[0] 高起始位置、iStart[1] 宽起始位置、iStart[2] 深起始位置
				int iHeight = 0,
				int iWidth = 0,
				int iDepth = 0 );

	/**	
	 *	读取HDF5文件数据(2维数据集)
	 */
	int ReadHdf5FileData2D(		hid_t hF,		// HDF文件标示
						const char* strDsName,	// 数据集名称
						void *pData,		// 接受文件数据的缓冲区
						hsize_t* iStart = NULL, // iStart[0] 高方向起始位置、iStart[1] 宽方向起始位置
						int nHeight  = 0,	// 高度步进
						int nWidth = 0 );	// 宽度步进

	/**	
	 *	将数据写入指定HDF5文件的指定数据集(2维数据集)
	 */
	int WriteHdf5FileData2D(	hid_t hF,			// HDF文件标示
					const char* strDsName,		// 数据集名称
					void *pData,			// 写入文件数据的缓冲区
					int iDataType,			// 写入的数据类型
					int iMemDimSize[2],		// 输入内存空间维数
					int iFileDimSize[2],		// 输出数据空间维数(或者Chunk Size)
					bool bCreate = false,		// 是否创建新的数据集
					bool isCompress = false,	// 是否压缩存放
					int *pMemStart = NULL,		// 内存数据空间的开始偏移
					int* pMemCount = NULL,		// 内存数据空间的长度(步进)
					int *pFileStart = NULL,		// 输出数据空间开始的偏移
					int *pFileCount = NULL		// 输出数据空间的长度(步进)
					);	


	//将数据保存为HDF5文件(1维)
	bool WriteHdf5FileData1D(hid_t hF,const char* strDsName,
				void *pData,int iDataType,
				int iDataSize,int iOutSize,int iDimension,
				bool isCompress = false,int *iDataStart=NULL,
				int *iDataCount=NULL,int *iOutStart=NULL);

	/**	将数据保存为HDF5文件(3维)
	 *	
	 */
	bool WriteHdf5FileData3D(	hid_t hF,			// HDF5句柄
					const char* strDsName,		// 要创建的数据集名称
					void *pData,			// 要写入的数据
					int iDataType,			// 数据的类型
					int iDataSize[3],		
					int iOutSize[3],               //dim,height,width
					int iDimension,			// 创建数据集的维数
					bool bFlag = false,		// 是否打开已有的数据集（默认false创建新的数据集）
					bool isCompress = false,	// 是否压缩数据集
					int *iDataStart = NULL ,
					int *iDataCount = NULL,
					int *iOutStart = NULL );
	
	/**	创建输出文件
	 *	@fileName	在指定位置创建Hdf5文件
	 */
	int createOutFile(const string fileName);


	//得到数据集信息(维数及大小、数据类型等)
	int GetHdfSdsParam(	hid_t hF,
				const char* strSDSName, 
				int *iDimensions,		// (输出)
				hsize_t *hDimValue,		// 高/宽/深(输出)
				int &iDataType,			// (输出)
				int &iNBytes );			// (输出)

	/**	getAttrDsInfo
	 *	获取HDF5文件数据集属性
	 *	@hF		HDF5文件句柄
	 *	@strDsName	指定HDF5的数据集名称
	 *	@vAttrD		返回所有数据集的属性
	 */
	bool getAttrDsInfo(hid_t hF,string strDsName,vector<attrOne> &vAttrD);


	/**	getAttrGlobeInfo
	 *	获取HDF5文件全局属性
	 *	@hF		指定HDF5文件
	 *	@vAttrG		返回指定的属性
	 */
	bool getAttrGlobeInfo(hid_t hF, vector<attrOne> &vAttrG);
	
	//设置HDF5文件全局属性
	bool setAttrInfo(hid_t hF,string strDsName,const vector<attrOne> &vAttrD);


	//设置HDF5文件数据集属性
	/**	
	 *	根据HDF5的句柄获取其每个数据集的名称
	 *	@hF HDF5的句柄
	 *	@vDSNs 返回所有数据集名称
	 */
	bool getAllDsNames(hid_t hF, vector<string> &vDsNs);

	/**	根据hf, strDsName获取attrOne属性
	 *	
	 */
	bool getAttrOne(hid_t hF,string strDsName,attrOne &aD);
	//bool GetNames(string strFolder,vector<string> &vFileName);

private:
	// 读数据集到缓冲区
	bool hdfReadToBuf(hid_t dset_id, hid_t mem_type_id, hid_t mem_space_id,
				hid_t file_space_id,  hid_t plist_id, void *buf, int iDataType);
	bool mallocHdfBuf(hid_t hF, const char* strDsName);
private:
	unsigned short *pBuf;
	char *pAttrTmp;//读取属性值时的临时变量
	char* pData;//
	hid_t m_hFile;		// HDF5文件句柄
	hid_t hDataSet;		// 指定的数据集
	hid_t hProperties;
	hid_t hDsSpace;
	hid_t hAttr;
	hsize_t hChunk_size[2];	// Chunk 尺寸
	hsize_t hDsSize[2];
	herr_t hStatus;
	unsigned szip_options_mask;
	unsigned szip_pixels_per_block;

private:
	int iOneDataSize[3];			// 数据集各维度的大小
	int iDsDataType;			// 数据集的数据类型
	int iDsDimension;			// 数据集的维数
	vector<attrOne> vGlobeAttrs;
	//vector<vector<attrOne> > vDsAttrs; 
	vector<string> vDsNames;		
};



#endif // !defined(AFX_HDF5COMPRESS_H__5EE7F1A0_2199_44C0_BA7D_ABB55C47FC17__INCLUDED_)
