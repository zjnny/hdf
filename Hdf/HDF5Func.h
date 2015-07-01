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

//����֤��ѹ����ʱ��������ݵ���ֵ�Ƚ�С����ѹ������ռ�õĿռ�Ƚ�С
//��ȡѹ�����HDF�ļ���δѹ����HDF�ļ�֮��û�в��
class HDF5Func  
{
public:
	HDF5Func();
	virtual ~HDF5Func();
public:

	//// ��ָ����HDF5�ļ�
	//bool HDF5Open(const char* strInfName);


	//// ��HDF5��ָ����һά��¼��������
	//int ReadOneDimDataSet(	const char* strDatasetName,	// ���ݼ�����
	//			void* pData,			// ����
	//			hsize_t nDepthStart,		// ��ȷ������ʼλ��
	//			hsize_t nHighStart,		// �߶ȷ������ʼλ��
	//			hsize_t nWidthStart,		// ��ȷ������ʼλ��
	//			int nLength,			// ��
	//			int nWidth,			// ��
	//			int nHeight);			// ��








	/**	ReadHdf5FileData
	 *	�ⲿ��ȡHDF5�ļ�������
	 *	@strInfName	Hdf�ļ�
	 *	@strDsName	SDS name
	 *	@pData		ָ��HDF5����ָ��
	 */
	int ReadHdf5FileData(const char* strInfName,const char* strDsName, 
		void *pData);

	/**	GetHdfSdsParam
	 *	��ȡHDF5�ļ����ݼ���صĲ���
	 *	@strInfName 	HDF5�ļ�·��
	 *	@strSDSName	���ݼ�������
	 *	@iDimensions	���ݼ���ά��(����)
	 *	@hDimValue	��ά�Ĵ�С(����)
	 *	@iDataType	ָ�����ݼ�����������(����)
	 *	@iNBytes	���ݼ��������͵Ĵ�С(�ֽ�)
	 */
	bool GetHdfSdsParam(const char* strInfName, const char* strSDSName, 
				int *iDimensions, hsize_t *hDimValue, int &iDataType,int &iNBytes);

	/**	GetHdfHW
	 *	����HDF5�ļ���ĳ�����ݼ����ƻ�ȡH�߶�/W���/D���/size
	 *	@strInfName HDF5�ļ�·��
	 *	@strSDSName HDF5�����ݼ�����
	 *	@iH	    ָ�����ݼ��ĸ߶�
	 *	@iW	    ָ�����ݼ��Ŀ��
	 *	@iD	    ָ�����ݼ������
	 *	@uDataSize	ָ�����ݼ��Ĵ�С
	 */
	int GetHdfHW(const char* strInfName, const char* strSDSName, int &iH, int &iW, int &iD, unsigned long &uDataSize);
	int GetHdfHW(hid_t hInFile,const char* strSDSName,int &iH,int &iW,int &iD,unsigned long &uDataSize);

	/**	mallocSdsBuf
	 *	����Sds���ݼ���ز������仺����
	 *	@strInfName	��Ϣ����
	 *	@strSdsName	SDS ����
	 *	@uDataSize	����ռ���ֽ���
	 *	@pReturn	���ط���ռ��ָ��
	 *	
	 */
	bool mallocSdsBuf(const char* strInfName,const char* strSdsName,
		unsigned long &uDataSize,void *&pReturn);
public:
	
	/**	CompressHDFFile
	 *	ѹ��HDF5�ļ�
	 *	@strInFileFolder	�ļ���·��
	 *	@strInFileName		�ļ�����
	 *	@strOutFileFolder	����ļ���·��
	 */
	bool CompressHDFFile(string strInFileFolder, string strInFileName,
			    string strOutFileFolder);	


	//��ȡHDF5�ļ�ȫ������
	bool getSetAttrInfo(hid_t hInFile,string strDsName,hid_t hOutFile);

	/**	��ȡHDF5�ļ�����
	 *	@hF			HDF5�ļ����
	 *	@strDsName		���ݼ�����
	 *	@pData			����ָ��
	 *	@iStart			��ʼλ��(��/��/��)
	 *	@iHeight		�߶�
	 *	@iWidth			���
	 *	@iDepth			���
	 */
	int ReadHdf5FileData(	hid_t hF, 
				const char* strDsName,
				void *pData,
				hsize_t* iStart = NULL, // iStart[0] ����ʼλ�á�iStart[1] ����ʼλ�á�iStart[2] ����ʼλ��
				int iHeight = 0,
				int iWidth = 0,
				int iDepth = 0 );

	/**	
	 *	��ȡHDF5�ļ�����(2ά���ݼ�)
	 */
	int ReadHdf5FileData2D(		hid_t hF,		// HDF�ļ���ʾ
						const char* strDsName,	// ���ݼ�����
						void *pData,		// �����ļ����ݵĻ�����
						hsize_t* iStart = NULL, // iStart[0] �߷�����ʼλ�á�iStart[1] ������ʼλ��
						int nHeight  = 0,	// �߶Ȳ���
						int nWidth = 0 );	// ��Ȳ���

	/**	
	 *	������д��ָ��HDF5�ļ���ָ�����ݼ�(2ά���ݼ�)
	 */
	int WriteHdf5FileData2D(	hid_t hF,			// HDF�ļ���ʾ
					const char* strDsName,		// ���ݼ�����
					void *pData,			// д���ļ����ݵĻ�����
					int iDataType,			// д�����������
					int iMemDimSize[2],		// �����ڴ�ռ�ά��
					int iFileDimSize[2],		// ������ݿռ�ά��(����Chunk Size)
					bool bCreate = false,		// �Ƿ񴴽��µ����ݼ�
					bool isCompress = false,	// �Ƿ�ѹ�����
					int *pMemStart = NULL,		// �ڴ����ݿռ�Ŀ�ʼƫ��
					int* pMemCount = NULL,		// �ڴ����ݿռ�ĳ���(����)
					int *pFileStart = NULL,		// ������ݿռ俪ʼ��ƫ��
					int *pFileCount = NULL		// ������ݿռ�ĳ���(����)
					);	


	//�����ݱ���ΪHDF5�ļ�(1ά)
	bool WriteHdf5FileData1D(hid_t hF,const char* strDsName,
				void *pData,int iDataType,
				int iDataSize,int iOutSize,int iDimension,
				bool isCompress = false,int *iDataStart=NULL,
				int *iDataCount=NULL,int *iOutStart=NULL);

	/**	�����ݱ���ΪHDF5�ļ�(3ά)
	 *	
	 */
	bool WriteHdf5FileData3D(	hid_t hF,			// HDF5���
					const char* strDsName,		// Ҫ���������ݼ�����
					void *pData,			// Ҫд�������
					int iDataType,			// ���ݵ�����
					int iDataSize[3],		
					int iOutSize[3],               //dim,height,width
					int iDimension,			// �������ݼ���ά��
					bool bFlag = false,		// �Ƿ�����е����ݼ���Ĭ��false�����µ����ݼ���
					bool isCompress = false,	// �Ƿ�ѹ�����ݼ�
					int *iDataStart = NULL ,
					int *iDataCount = NULL,
					int *iOutStart = NULL );
	
	/**	��������ļ�
	 *	@fileName	��ָ��λ�ô���Hdf5�ļ�
	 */
	int createOutFile(const string fileName);


	//�õ����ݼ���Ϣ(ά������С���������͵�)
	int GetHdfSdsParam(	hid_t hF,
				const char* strSDSName, 
				int *iDimensions,		// (���)
				hsize_t *hDimValue,		// ��/��/��(���)
				int &iDataType,			// (���)
				int &iNBytes );			// (���)

	/**	getAttrDsInfo
	 *	��ȡHDF5�ļ����ݼ�����
	 *	@hF		HDF5�ļ����
	 *	@strDsName	ָ��HDF5�����ݼ�����
	 *	@vAttrD		�����������ݼ�������
	 */
	bool getAttrDsInfo(hid_t hF,string strDsName,vector<attrOne> &vAttrD);


	/**	getAttrGlobeInfo
	 *	��ȡHDF5�ļ�ȫ������
	 *	@hF		ָ��HDF5�ļ�
	 *	@vAttrG		����ָ��������
	 */
	bool getAttrGlobeInfo(hid_t hF, vector<attrOne> &vAttrG);
	
	//����HDF5�ļ�ȫ������
	bool setAttrInfo(hid_t hF,string strDsName,const vector<attrOne> &vAttrD);


	//����HDF5�ļ����ݼ�����
	/**	
	 *	����HDF5�ľ����ȡ��ÿ�����ݼ�������
	 *	@hF HDF5�ľ��
	 *	@vDSNs �����������ݼ�����
	 */
	bool getAllDsNames(hid_t hF, vector<string> &vDsNs);

	/**	����hf, strDsName��ȡattrOne����
	 *	
	 */
	bool getAttrOne(hid_t hF,string strDsName,attrOne &aD);
	//bool GetNames(string strFolder,vector<string> &vFileName);

private:
	// �����ݼ���������
	bool hdfReadToBuf(hid_t dset_id, hid_t mem_type_id, hid_t mem_space_id,
				hid_t file_space_id,  hid_t plist_id, void *buf, int iDataType);
	bool mallocHdfBuf(hid_t hF, const char* strDsName);
private:
	unsigned short *pBuf;
	char *pAttrTmp;//��ȡ����ֵʱ����ʱ����
	char* pData;//
	hid_t m_hFile;		// HDF5�ļ����
	hid_t hDataSet;		// ָ�������ݼ�
	hid_t hProperties;
	hid_t hDsSpace;
	hid_t hAttr;
	hsize_t hChunk_size[2];	// Chunk �ߴ�
	hsize_t hDsSize[2];
	herr_t hStatus;
	unsigned szip_options_mask;
	unsigned szip_pixels_per_block;

private:
	int iOneDataSize[3];			// ���ݼ���ά�ȵĴ�С
	int iDsDataType;			// ���ݼ�����������
	int iDsDimension;			// ���ݼ���ά��
	vector<attrOne> vGlobeAttrs;
	//vector<vector<attrOne> > vDsAttrs; 
	vector<string> vDsNames;		
};



#endif // !defined(AFX_HDF5COMPRESS_H__5EE7F1A0_2199_44C0_BA7D_ABB55C47FC17__INCLUDED_)
